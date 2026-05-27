# Exemplo — PlatformIO: Projeto Estruturado

## Objetivo

Mostrar a estrutura completa de um projeto PlatformIO com módulo de cálculo e teste nativo.

## Estrutura do projeto

```
telemetria-pio/
├── platformio.ini
├── src/
│   └── main.cpp
├── lib/
│   └── calc/
│       ├── SensorData.h
│       ├── VerticalVelocity.h
│       └── ApogeeDetection.h
└── test/
    └── test_apogee/
        └── test_apogee.cpp
```

## platformio.ini

```ini
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
build_flags =
    -DI2C_SDA=4
    -DI2C_SCL=5
    -DSEALEVEL=1013.25

[env:native]
platform = native
framework = unity
build_flags = -I lib
```

## lib/calc/SensorData.h

```cpp
#pragma once

struct SensorData {
    unsigned long timestamp_ms;
    float altitude_m;
    float temperatura_C;
    float pressao_hPa;

    bool isValid() const {
        if (altitude_m < -100 || altitude_m > 10000) return false;
        if (temperatura_C < -40 || temperatura_C > 85) return false;
        if (pressao_hPa < 300 || pressao_hPa > 1100) return false;
        return true;
    }
};
```

## lib/calc/ApogeeDetection.h

```cpp
#pragma once

class ApogeeDetection {
public:
    ApogeeDetection(float threshold = -2.0f, int confirm = 3)
        : m_threshold(threshold), m_confirm(confirm) {}

    bool update(float vz) {
        if (vz < m_threshold) m_count++;
        else m_count = 0;
        return m_count >= m_confirm;
    }

    void reset() { m_count = 0; }

private:
    float m_threshold;
    int m_confirm;
    int m_count = 0;
};
```

## src/main.cpp

```cpp
#include <Arduino.h>
#include "SensorData.h"
#include "ApogeeDetection.h"

#define LED_PIN 1

ApogeeDetection apogeu;
unsigned long last = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    unsigned long now = millis();
    digitalWrite(LED_PIN, (now / 500) % 2);

    if (now - last >= 50) {
        last = now;
        // Simula dados
        SensorData d { now, 100.0, 25.0, 1013.25 };
        Serial.printf("#%lu;%.2f#\n", d.timestamp_ms, d.altitude_m);
    }
}
```

## test/test_apogee/test_apogee.cpp

```cpp
#include <unity.h>
#include "ApogeeDetection.h"

ApogeeDetection detector;

void setUp() { detector.reset(); }

void test_apogeu_nao_detectado_subindo() {
    TEST_ASSERT_FALSE(detector.update(5.0f));
    TEST_ASSERT_FALSE(detector.update(3.0f));
}

void test_apogeu_detectado_descendo() {
    TEST_ASSERT_FALSE(detector.update(-1.0f));
    TEST_ASSERT_FALSE(detector.update(-2.5f));
    TEST_ASSERT_TRUE(detector.update(-3.0f));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_apogeu_nao_detectado_subindo);
    RUN_TEST(test_apogeu_detectado_descendo);
    UNITY_END();
}

void loop() {}
```

## Como testar

```bash
# Build para ESP32-C3
pio run -e esp32c3

# Upload
pio run -e esp32c3 -t upload --upload-port /dev/ttyACM0

# Testes nativos (PC)
pio test -e native

# Testes com verbose
pio test -e native -v
```
