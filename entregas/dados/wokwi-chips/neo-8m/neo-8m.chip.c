/*
 * NEO-8M - GPS module (Wokwi custom chip)
 *
 * Simula um modulo GPS u-blox NEO-8M pela UART a 9600 baud, emitindo
 * sentencas NMEA 0183 (GPGGA + GPRMC) a cada 1 s.
 *
 * Se a latitude/longitude nao forem definidas (attr = 0), uma localizacao
 * aleatoria e sorteada no boot. O fix "anda" alguns metros por segundo e
 * ha um pequeno jitter, simulando o movimento.
 *
 * Pinos: VCC, GND, TX (saida NMEA -> RX do MCU), RX (entrada de comandos).
 */
#include "wokwi-api.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  uart_dev_t uart;
  timer_t timer;
  uint32_t attr_lat;
  uint32_t attr_lon;
  uint32_t attr_alt;
  double lat;
  double lon;
  double alt;
  unsigned seconds;
  char pending[256];
  uint32_t pending_len;
} chip_state_t;

static uint32_t rng_state;

static uint32_t rng_next(void) {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

static double rng_range(double lo, double hi) {
  return lo + (hi - lo) * (rng_next() / 4294967296.0);
}

/* Monta "$<body>*<checksum>\r\n" em out. */
static void nmea_sentence(char *out, const char *body) {
  uint8_t cs = 0;
  for (const char *p = body; *p; p++) {
    cs ^= (uint8_t)*p;
  }
  sprintf(out, "$%s*%02X\r\n", body, cs);
}

static void format_lat(double v, char *out, char *dir) {
  *dir = (v < 0) ? 'S' : 'N';
  const double a = fabs(v);
  const int deg = (int)a;
  const double min = (a - deg) * 60.0;
  sprintf(out, "%02d%07.4f", deg, min);
}

static void format_lon(double v, char *out, char *dir) {
  *dir = (v < 0) ? 'W' : 'E';
  const double a = fabs(v);
  const int deg = (int)a;
  const double min = (a - deg) * 60.0;
  sprintf(out, "%03d%07.4f", deg, min);
}

static void send_pending(chip_state_t *chip) {
  if (chip->pending_len == 0) {
    return;
  }
  if (uart_write(chip->uart, (uint8_t *)chip->pending, chip->pending_len)) {
    chip->pending_len = 0;
  }
}

static void on_timer(void *user_data) {
  chip_state_t *chip = user_data;

  chip->seconds += 1;
  /* pequeno deslocamento + jitter */
  chip->lat += rng_range(-2e-5, 2e-5) + ((int)(rng_next() % 20) - 10) * 1e-7;
  chip->lon += rng_range(-2e-5, 2e-5) + ((int)(rng_next() % 20) - 10) * 1e-7;

  const unsigned hh = (chip->seconds / 3600) % 24;
  const unsigned mm = (chip->seconds / 60) % 60;
  const unsigned ss = chip->seconds % 60;

  char timebuf[16];
  sprintf(timebuf, "%02u%02u%02u.00", hh, mm, ss);

  char latbuf[16], lonbuf[16], latdir, londir;
  format_lat(chip->lat, latbuf, &latdir);
  format_lon(chip->lon, lonbuf, &londir);

  char body[160], sentence[200];

  /* GGA: fix, satelites, altitude */
  sprintf(body,
          "GPGGA,%s,%s,%c,%s,%c,1,09,0.9,%.1f,M,0.0,M,,",
          timebuf, latbuf, latdir, lonbuf, londir, chip->alt);
  nmea_sentence(sentence, body);
  strcpy(chip->pending, sentence);

  /* RMC: validade, velocidade, curso, data */
  sprintf(body,
          "GPRMC,%s,A,%s,%c,%s,%c,0.0,0.0,171026,,,A",
          timebuf, latbuf, latdir, lonbuf, londir);
  nmea_sentence(sentence, body);
  strcat(chip->pending, sentence);

  chip->pending_len = strlen(chip->pending);
  send_pending(chip);
}

static uint8_t on_uart_write_done(void *user_data) {
  chip_state_t *chip = user_data;
  send_pending(chip);
  return 0;
}

void chip_init(void) {
  chip_state_t *chip = calloc(1, sizeof(chip_state_t));

  chip->attr_lat = attr_init_float("latitude", 0.0f);
  chip->attr_lon = attr_init_float("longitude", 0.0f);
  chip->attr_alt = attr_init_float("altitude", 0.0f);

  rng_state = (uint32_t)(get_sim_nanos() & 0xFFFFFFFF) | 1u;

  chip->lat = attr_read_float(chip->attr_lat);
  chip->lon = attr_read_float(chip->attr_lon);
  chip->alt = attr_read_float(chip->attr_alt);

  if (chip->lat == 0.0) {
    chip->lat = rng_range(-30.0, -15.0); /* Brasil */
  }
  if (chip->lon == 0.0) {
    chip->lon = rng_range(-55.0, -38.0);
  }
  if (chip->alt == 0.0) {
    chip->alt = rng_range(600.0, 900.0);
  }

  const uart_config_t uart_config = {
      .tx = pin_init("TX", INPUT_PULLUP),
      .rx = pin_init("RX", INPUT),
      .baud_rate = 9600,
      .write_done = on_uart_write_done,
      .user_data = chip,
  };
  chip->uart = uart_init(&uart_config);

  const timer_config_t timer_config = {
      .callback = on_timer,
      .user_data = chip,
  };
  chip->timer = timer_init(&timer_config);
  timer_start(chip->timer, 1000000, true); /* 1 s */

  printf("NEO-8M pronto: lat=%.5f lon=%.5f alt=%.1f\n", chip->lat, chip->lon, chip->alt);
}
