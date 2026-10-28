/*
 * BMP585 - Barometric pressure sensor (Wokwi custom chip)
 *
 * Simula um BMP585 (Bosch) no barramento I2C. A pressao e definida
 * diretamente por um controle (em hPa) e a temperatura por outro. Assim,
 * a transformacao pressao -> altitude fica para o firmware/aluno fazer.
 *
 * Ajuste os valores pelos controles na simulacao (ou pelos attrs no
 * diagram.json): "pressure", "temperature", "address".
 *
 * Registradores implementados (subconjunto do datasheet):
 *   0x01 CHIP_ID       (0x51 = BMP585)
 *   0x02 REV_ID
 *   0x1D..0x1F TEMP    XLSB, LSB, MSB  (24-bit, LSB = 1/2^16 C)
 *   0x20..0x22 PRESS   XLSB, LSB, MSB  (24-bit, LSB = 1/2^6 Pa)
 *   0x27 INT_STATUS    (0x10 = por)
 *   0x28 STATUS        (0x01 = nvm_rdy)
 *   0x30 DSP / 0x31 DSP_IIR / 0x36 OSR / 0x37 ODR / 0x7E CMD (guardados)
 */
#include "wokwi-api.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define REG_CHIP_ID    0x01
#define REG_REV_ID     0x02
#define REG_TEMP_XLSB  0x1D
#define REG_PRESS_XLSB 0x20
#define REG_INT_STATUS 0x27
#define REG_STATUS     0x28
#define REG_DSP        0x30
#define REG_DSP_IIR    0x31
#define REG_OSR        0x36
#define REG_ODR        0x37
#define REG_CMD        0x7E

typedef struct {
  i2c_dev_t i2c;
  uint32_t attr_pressure;
  uint32_t attr_temperature;
  uint8_t regs[256];
  uint8_t reg_ptr;
  bool first_byte;
} chip_state_t;

/* Preenche os registradores de dados a partir da pressao (hPa) e temperatura (C). */
static void update_measurements(chip_state_t *chip) {
  const float pressure_hpa = attr_read_float(chip->attr_pressure);
  const float temperature = attr_read_float(chip->attr_temperature);
  const float pressure_pa = pressure_hpa * 100.0f;

  const int32_t raw_t = (int32_t)lroundf(temperature * 65536.0f);
  const int32_t raw_p = (int32_t)lroundf(pressure_pa * 64.0f);

  chip->regs[REG_TEMP_XLSB + 0] = raw_t & 0xFF;
  chip->regs[REG_TEMP_XLSB + 1] = (raw_t >> 8) & 0xFF;
  chip->regs[REG_TEMP_XLSB + 2] = (raw_t >> 16) & 0xFF;

  chip->regs[REG_PRESS_XLSB + 0] = raw_p & 0xFF;
  chip->regs[REG_PRESS_XLSB + 1] = (raw_p >> 8) & 0xFF;
  chip->regs[REG_PRESS_XLSB + 2] = (raw_p >> 16) & 0xFF;
}

static bool on_i2c_connect(void *user_data, uint32_t address, bool read) {
  chip_state_t *chip = user_data;
  chip->first_byte = !read; /* na escrita, o 1o byte e o endereco do registrador */
  if (read) {
    update_measurements(chip);
  }
  return true; /* ACK */
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
  return true; /* ACK */
}

void chip_init(void) {
  chip_state_t *chip = calloc(1, sizeof(chip_state_t));

  chip->attr_pressure = attr_init_float("pressure", 1013.25f);
  chip->attr_temperature = attr_init_float("temperature", 25.0f);

  chip->regs[REG_CHIP_ID] = 0x51;    /* BMP585 ASIC id */
  chip->regs[REG_REV_ID] = 0x32;
  chip->regs[REG_INT_STATUS] = 0x10; /* power-on reset */
  chip->regs[REG_STATUS] = 0x01;     /* NVM ready */
  chip->regs[REG_OSR] = 0x15;
  chip->regs[REG_ODR] = 0x21;
  update_measurements(chip);

  const uint32_t address_attr = attr_init("address", 0x7E);

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

  printf("BMP585 pronto (I2C 0x%02X)\n", attr_read(address_attr));
}
