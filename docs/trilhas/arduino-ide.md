# Trilha Arduino IDE

O Arduino IDE é a porta de entrada para programação de microcontroladores. Tanto o Flight Computer quanto o Helike usam ESP32-C3 com sensores I2C, comunicação serial e LoRa. Esta trilha ensina os fundamentos que você vai aplicar nos projetos reais.

## O que você vai aprender

- Programar GPIO (digital e PWM)
- Ler sensores via I2C
- Transmitir dados pela serial
- Controlar servomotores
- Estruturar um sketch para voo

## Pré-requisitos

- Arduino IDE instalado (recomendado: versão 2.x)
- ESP32-C3 Super Mini (ou placa compatível)
- Cabo USB-C
- Jumpers e protoboard

---

## 1. GPIO básico — Blink

O "Hello World" dos microcontroladores. O Flight Computer usa o LED (GPIO 1) como indicador de status.

```cpp
#define LED_PIN 1

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
```

### Buzzer (indicação sonora)

O Flight Computer usa buzzer no GPIO 0 para indicar estados (boot, erro, apogeu):

```cpp
#define BUZZER_PIN 0

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    tone(BUZZER_PIN, 500); // 500 Hz
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
}
```

---

## 2. Botão com debounce

O Helike tem um botão no GPIO 2 para interação básica:

```cpp
#define BUTTON_PIN 2
#define LED_PIN 1

bool last_state = HIGH;
bool led_state = LOW;

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    bool current = digitalRead(BUTTON_PIN);
    if (last_state == HIGH && current == LOW) {
        delay(50); // debounce
        led_state = !led_state;
        digitalWrite(LED_PIN, led_state);
    }
    last_state = current;
}
```

---

## 3. Serial — comunicação com o PC

A serial é o principal canal de debug e coleta de dados.

### 3.1 Enviando dados formatados

```cpp
#define LED_PIN 1

unsigned long last_time = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    unsigned long now = millis();

    // Pisca o LED a 1 Hz
    digitalWrite(LED_PIN, (now / 500) % 2);

    // Envia pacote a 20 Hz
    if (now - last_time >= 50) {
        last_time = now;
        char buffer[64];
        snprintf(buffer, sizeof(buffer),
            "#%lu;%.2f;%.2f;%.2f;%.2f#",
            now,  // timestamp
            100.0 + 10 * sin(now / 1000.0),  // altitude simulada
            0.01,   // ax
            0.02,   // ay
            9.81    // az
        );
        Serial.println(buffer);
    }
}
```

Este formato (`#campos#`) é o mesmo usado nos testes do Helike. O Python consegue fazer o parsing direto.

### 3.2 Taxa de amostragem constante

Em sistemas de voo, manter uma taxa de amostragem fixa é crítico. Use `millis()` em vez de `delay()`:

```cpp
unsigned long last_sample = 0;
const unsigned long INTERVAL = 50; // 20 Hz

void loop() {
    unsigned long now = millis();
    if (now - last_sample >= INTERVAL) {
        last_sample = now;
        // ler sensores e enviar dados
    }
    // outras tarefas podem rodar aqui
}
```

---

## 4. I2C — leitura de sensores

O I2C é o barramento mais comum nos projetos da Serra Rocketry. Todos os sensores do Flight Computer e Helike usam I2C.

### 4.1 Endereços I2C comuns nos projetos

| Componente | Endereço I2C | Projeto |
|---|---|---|
| BMP280 (barômetro) | 0x76 ou 0x77 | Flight Computer |
| BMP585 (barômetro) | Endereço configurável | Flight Computer v2 |
| BME280 (barômetro + umidade) | 0x76 | Helike |
| ICM-20602 (IMU) | 0x68 ou 0x69 | Helike |
| MPU6050 (IMU) | 0x68 | Flight Computer v1 |
| LSM6DS3 (IMU) | Endereço configurável | Flight Computer v2 |

### 4.2 Scan de barramento I2C

```cpp
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin(4, 5); // SDA=GPIO4, SCL=GPIO5 (Flight Computer)
    // Helike usa SDA=GPIO8, SCL=GPIO9

    Serial.println("Escaneando I2C...");
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Dispositivo em 0x");
            Serial.println(addr, HEX);
        }
    }
}

void loop() {}
```

### 4.3 Lendo BMP280 (barômetro)

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define I2C_SDA 4
#define I2C_SCL 5

Adafruit_BMP280 bmp;

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!bmp.begin(0x76)) {
        Serial.println("BMP280 não encontrado!");
        while (1);
    }
}

void loop() {
    Serial.print("#");
    Serial.print(millis());
    Serial.print(";");
    Serial.print(bmp.readAltitude(1013.25)); // altitude em metros
    Serial.print(";");
    Serial.print(bmp.readTemperature());
    Serial.println("#");
    delay(50); // 20 Hz
}
```

**No Flight Computer** — O BMP280 é usado para medir altitude e detectar apogeu. O sensor fica no barramento I2C com SDA=GPIO4, SCL=GPIO5.

**No Helike** — O BME280 (mesmo protocolo) é usado no barramento I2C com SDA=GPIO8, SCL=GPIO9.

---

## 5. Servo motor

O Flight Computer usa um servo no GPIO 3 para acionar o paraquedas:

```cpp
#include <ESP32Servo.h>

#define SERVO_PIN 3
Servo servo;

void setup() {
    servo.attach(SERVO_PIN);
    servo.write(0); // posição inicial (recolhido)
}

void loop() {
    servo.write(90); // posição de disparo
    delay(1000);
    servo.write(0);  // recolhe
    delay(1000);
}
```

---

## 6. LoRa — telemetria sem fio

O RFM95W (LoRa) é usado nos dois projetos para transmissão de telemetria.

```cpp
#include <SPI.h>
#include <LoRa.h>

#define SS_PIN    7
#define RST_PIN   1
#define DIO0_PIN  2

void setup() {
    LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
    if (!LoRa.begin(915E6)) { // 915 MHz
        Serial.println("LoRa falhou!");
        while (1);
    }
    LoRa.setSyncWord(0xF3);
}

void loop() {
    LoRa.beginPacket();
    LoRa.print("#");
    LoRa.print(millis());
    LoRa.print(";");
    LoRa.print(100.0); // altitude simulada
    LoRa.println("#");
    LoRa.endPacket();
    delay(1000);
}
```

**No Helike** — O LoRa está em 915 MHz com SPI nos pinos MOSI=GPIO7, MISO=GPIO5, SCK=GPIO6, CS=GPIO10, RST=GPIO4, DIO0=GPIO3.

**No Flight Computer** — SPI nos pinos MOSI=GPIO6, MISO=GPIO8, CLK=GPIO9, CS=GPIO7, RST=GPIO1, DIO0=GPIO2.

---

## 7. Projeto integrado — sensor + serial + LED

Combine tudo que aprendeu:

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define LED_PIN     1
#define I2C_SDA     4
#define I2C_SCL     5

Adafruit_BMP280 bmp;
unsigned long last_sample = 0;
const unsigned long INTERVAL = 50; // 20 Hz

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!bmp.begin(0x76)) {
        Serial.println("ERRO:BMP280#");
        while (1);
    }
}

void loop() {
    unsigned long now = millis();

    // LED pisca a 1 Hz
    digitalWrite(LED_PIN, (now / 500) % 2);

    // Amostragem a 20 Hz
    if (now - last_sample >= INTERVAL) {
        last_sample = now;
        float alt = bmp.readAltitude(1013.25);
        float temp = bmp.readTemperature();

        char buf[64];
        snprintf(buf, sizeof(buf),
            "#%lu;%.2f;%.2f#", now, alt, temp);
        Serial.println(buf);
    }
}
```

---

## Exercícios práticos

### Nível 1 — GPIO e serial

1. Faça um LED piscar a 2 Hz.
2. Adicione um buzzer que toque quando o botão for pressionado.
3. Envie "BOOT OK" pela serial ao iniciar.

### Nível 2 — Sensor e aquisição

1. Faça scan I2C e liste os endereços encontrados.
2. Leia BMP280 e envie pacotes formatados a 10 Hz.
3. Adicione validação: não envie pacotes se a altitude for inválida (ex: > 10000 m).

### Nível 3 — Integração

1. Combine LED (status), BMP280 (dados) e serial (saída) em um único sketch.
2. Use `millis()` para amostragem a 20 Hz sem bloquear.
3. Formate a saída como pacote delimitado por `#`.

### Nível 4 — Preparação para projetos reais

1. Leia o pinout do Flight Computer e mapeie os pinos dos sensores no config.h.
2. Simule a FSM de voo: acenda o LED em taxa rápida durante ascensão, lenta durante descida.
3. Implemente uma lógica simples de detecção de apogeu: se altitude caiu por 5 amostras consecutivas, acione o servo.

---

## Conexão com os projetos reais

| Conceito | Projeto | Onde é usado |
|---|---|---|
| GPIO (LED, buzzer) | Flight Computer | Indicadores de status |
| I2C | Ambos | Todos os sensores |
| Serial (115200 baud) | Ambos | Debug e coleta de dados |
| Servo PWM | Flight Computer | Disparo do paraquedas |
| LoRa SPI | Ambos | Telemetria sem fio |
| millis() sem delay | Ambos | Loop não-bloqueante |

## Entregas relacionadas

- [Semana 3](../semanas/semana-03.md): Blink + sensor + pacote serial
- [Semana 5](../semanas/semana-05.md): Integração Arduino ↔ Python

## Critérios de aceite

- Código compila e faz upload sem erros
- Serial envia pacotes no formato definido
- Taxa de amostragem consistente
- README com instruções de hardware e software
