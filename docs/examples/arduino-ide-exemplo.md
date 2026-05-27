# Exemplo — Arduino IDE: Sensor + Serial

## Objetivo

Ler um sensor BMP280 via I2C e enviar pacotes formatados pela serial a 20 Hz.

## Código completo

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define LED_PIN     1
#define I2C_SDA     4
#define I2C_SCL     5
#define SEALEVEL    1013.25

Adafruit_BMP280 bmp;
unsigned long last_sample = 0;
const unsigned long INTERVAL = 50; // 20 Hz

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!bmp.begin(0x76)) {
        Serial.println("ERRO:SENSOR#");
        while (1);
    }
    Serial.println("BOOT:OK#");
}

void loop() {
    unsigned long now = millis();

    // LED heartbeat — 1 Hz
    digitalWrite(LED_PIN, (now / 500) % 2);

    // Amostragem a 20 Hz
    if (now - last_sample >= INTERVAL) {
        last_sample = now;

        float alt = bmp.readAltitude(SEALEVEL);
        float temp = bmp.readTemperature();
        float press = bmp.readPressure() / 100.0;

        char buf[96];
        snprintf(buf, sizeof(buf),
            "#%lu;%.2f;%.2f;%.2f#",
            now, alt, temp, press);
        Serial.println(buf);
    }
}
```

## Como testar

1. Conecte o BMP280 ao ESP32-C3:
   - VCC → 3.3V
   - GND → GND
   - SDA → GPIO4
   - SCL → GPIO5

2. Faça upload do código

3. Abra o Serial Monitor (115200 baud)

4. Saída esperada:
```
BOOT:OK#
#0;0.00;25.00;1013.25#
#50;0.05;24.99;1013.20#
#100;0.12;24.98;1013.15#
...
```
