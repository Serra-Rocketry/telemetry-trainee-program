/*
 * LSM6DS3 - IMU (acelerometro + giroscopio) - Wokwi custom chip
 *
 * Simula um LSM6DS3 no barramento I2C. Os valores de aceleracao (g) e
 * velocidade angular (dps) vem de controles/attrs; a sensibilidade e
 * calculada a partir dos registradores CTRL1_XL (FS_XL) e CTRL2_G (FS_G).
 *
 * Registradores implementados (subconjunto do datasheet):
 *   0x0F WHO_AM_I   (0x6A)
 *   0x10 CTRL1_XL   0x11 CTRL2_G   0x12 CTRL3_C  (guardados)
 *   0x1E STATUS_REG (0x07 = XLDA|GDA|TDA)
 *   0x20..0x21 OUT_TEMP_L/H   (LSB = 1/256 C, offset 25 C)
 *   0x22..0x27 OUTX_L/H_G     (giroscopio, X Y Z)
 *   0x28..0x2D OUTX_L/H_XL    (acelerometro, X Y Z)
 */
#include "wokwi-api.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define REG_WHO_AM_I   0x0F
#define REG_CTRL1_XL   0x10
#define REG_CTRL2_G    0x11
#define REG_CTRL3_C    0x12
#define REG_STATUS_REG 0x1E
#define REG_OUT_TEMP_L 0x20
#define REG_OUTX_L_G   0x22
#define REG_OUTX_L_XL  0x28

typedef struct {
  i2c_dev_t i2c;
  uint32_t attr_ax, attr_ay, attr_az;
  uint32_t attr_gx, attr_gy, attr_gz;
  uint32_t attr_temp;
  uint8_t regs[256];
  uint8_t reg_ptr;
  bool first_byte;
} chip_state_t;

/* Sensibilidade do acelerometro em g/LSB conforme FS_XL (CTRL1_XL[3:2]). */
static float accel_sens(chip_state_t *chip) {
  /* 00=2g, 01=16g, 10=4g, 11=8g */
  static const float sens[4] = {0.000061f, 0.000488f, 0.000122f, 0.000244f};
  return sens[(chip->regs[REG_CTRL1_XL] >> 2) & 0x03];
}

/* Sensibilidade do giroscopio em dps/LSB conforme FS_G (CTRL2_G[3:2]). */
static float gyro_sens(chip_state_t *chip) {
  /* 00=250, 01=500, 10=1000, 11=2000 dps */
  static const float sens[4] = {0.00875f, 0.01750f, 0.035f, 0.070f};
  return sens[(chip->regs[REG_CTRL2_G] >> 2) & 0x03];
}

static void put_i16(chip_state_t *chip, uint8_t reg, int16_t value) {
  chip->regs[reg] = value & 0xFF;
  chip->regs[reg + 1] = (value >> 8) & 0xFF;
}

static void update_outputs(chip_state_t *chip) {
  const float ag = accel_sens(chip);
  const float gg = gyro_sens(chip);

  const int16_t raw_ax = (int16_t)lroundf(attr_read_float(chip->attr_ax) / ag);
  const int16_t raw_ay = (int16_t)lroundf(attr_read_float(chip->attr_ay) / ag);
  const int16_t raw_az = (int16_t)lroundf(attr_read_float(chip->attr_az) / ag);
  const int16_t raw_gx = (int16_t)lroundf(attr_read_float(chip->attr_gx) / gg);
  const int16_t raw_gy = (int16_t)lroundf(attr_read_float(chip->attr_gy) / gg);
  const int16_t raw_gz = (int16_t)lroundf(attr_read_float(chip->attr_gz) / gg);
  const int16_t raw_t = (int16_t)lroundf((attr_read_float(chip->attr_temp) - 25.0f) * 256.0f);

  put_i16(chip, REG_OUT_TEMP_L, raw_t);
  put_i16(chip, REG_OUTX_L_G + 0, raw_gx);
  put_i16(chip, REG_OUTX_L_G + 2, raw_gy);
  put_i16(chip, REG_OUTX_L_G + 4, raw_gz);
  put_i16(chip, REG_OUTX_L_XL + 0, raw_ax);
  put_i16(chip, REG_OUTX_L_XL + 2, raw_ay);
  put_i16(chip, REG_OUTX_L_XL + 4, raw_az);

  chip->regs[REG_STATUS_REG] = 0x07; /* XLDA | GDA | TDA */
}

static bool on_i2c_connect(void *user_data, uint32_t address, bool read) {
  chip_state_t *chip = user_data;
  chip->first_byte = !read;
  if (read) {
    update_outputs(chip);
  }
  return true;
}

static uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = user_data;
  return chip->regs[chip->reg_ptr++];
}

static bool on_i2c_write(void *user_data, uint8_t data) {
  chip_state_t *chip = user_data;
  if (chip->first_byte) {
    chip->reg_ptr = data;
    chip->first_byte = false;
  } else {
    chip->regs[chip->reg_ptr++] = data;
  }
  return true;
}

void chip_init(void) {
  chip_state_t *chip = calloc(1, sizeof(chip_state_t));

  chip->attr_ax = attr_init_float("accelX", 0.0f);
  chip->attr_ay = attr_init_float("accelY", 0.0f);
  chip->attr_az = attr_init_float("accelZ", 1.0f);   /* repouso: 1 g no eixo Z */
  chip->attr_gx = attr_init_float("gyroX", 0.0f);
  chip->attr_gy = attr_init_float("gyroY", 0.0f);
  chip->attr_gz = attr_init_float("gyroZ", 0.0f);
  chip->attr_temp = attr_init_float("temperature", 25.0f);

  chip->regs[REG_WHO_AM_I] = 0x6A;   /* LSM6DS3 */
  chip->regs[REG_CTRL1_XL] = 0x60;   /* 416 Hz, +-2 g */
  chip->regs[REG_CTRL2_G] = 0x60;    /* 416 Hz, 250 dps */
  update_outputs(chip);

  const uint32_t address_attr = attr_init("address", 0x6B);

  const i2c_config_t i2c_config = {
      .address = attr_read(address_attr),
      .scl = pin_init("SCL", INPUT_PULLUP),
      .sda = pin_init("SDA", INPUT_PULLUP),
      .connect = on_i2c_connect,
      .read = on_i2c_read,
      .write = on_i2c_write,
      .user_data = chip,
  };
  chip->i2c = i2c_init(&i2c_config);

  printf("LSM6DS3 pronto (I2C 0x%02X)\n", attr_read(address_attr));
}
