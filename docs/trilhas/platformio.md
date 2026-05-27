# Trilha PlatformIO

PlatformIO é o ecossistema de desenvolvimento profissional usado no satélite Helike. Ele substitui o Arduino IDE com gerenciamento de bibliotecas, múltiplos ambientes de build e suporte a testes nativos.

## O que você vai aprender

- Estruturar um projeto PlatformIO
- Configurar `platformio.ini` para múltiplos alvos
- Gerenciar bibliotecas declarativamente
- Escrever e rodar testes nativos (PC)
- Separar código em módulos (`lib/`, `src/`, `include/`)

## Pré-requisitos

- VS Code + PlatformIO extension
- Projeto da Semana 3 (Arduino IDE) concluído

---

## 1. Por que PlatformIO?

O Arduino IDE é ótimo para aprender, mas projetos reais precisam de:

| Funcionalidade | Arduino IDE | PlatformIO |
|---|---|---|
| Gerenciamento de bibliotecas | Manual (baixar ZIP) | Declarativo (`lib_deps`) |
| Múltiplos ambientes | Não suporta | Nativo (ex: `helike_esp32c3`, `native`) |
| Testes nativos | Não suporta | Sim (Unity framework) |
| Organização modular | Tudo num `.ino` | `src/`, `lib/`, `include/` |
| CI/CD | Manual | Integração com GitHub Actions |

**No Helike** — O projeto usa PlatformIO com dois ambientes: `helike_esp32c3` (firmware do ESP32-C3) e `native` (testes unitários no PC).

---

## 2. Estrutura de projeto

```
projeto/
├── platformio.ini          # Configuração do projeto
├── src/                    # Código principal
│   └── main.cpp            # Entry point (setup + loop)
├── lib/                    # Bibliotecas do projeto
│   ├── calc/               # Módulo de cálculo (header-only)
│   │   ├── SensorData.h
│   │   ├── VerticalVelocity.h
│   │   └── ApogeeDetection.h
│   └── ...                 # Outros módulos
├── include/                # Headers públicos (opcional)
├── test/                   # Testes nativos
│   ├── test_vz/
│   │   └── test_vz.cpp
│   └── test_apogee/
│       └── test_apogee.cpp
└── test_hardware/           # Sketches de teste de hardware
    └── sensor/
        └── bmp280/
            └── bmp280.ino
```

**Exemplo real** — O Helike segue exatamente esta estrutura: `lib/calc/` com módulos header-only testáveis no PC, `test/` com testes Unity, e `test_hardware/` com sketches de validação.

---

## 3. Configuração — platformio.ini

### 3.1 Projeto básico

```ini
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
```

### 3.2 Múltiplos ambientes (como no Helike)

```ini
[env:helike_esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
board_build.mcu = esp32c3
board_build.f_cpu = 160000000L
monitor_speed = 115200
upload_speed = 921600
build_flags =
    -DCORE_DEBUG_LEVEL=4
    -DCONFIG_FREERTOS_UNICORE=1
    -DARDUINO_USB_CDC_ON_BOOT=1
lib_deps =
    sandeepmistry/LoRa @ ^0.8.0
    adafruit/Adafruit BME280 Library @ ^2.2.4
    mikalhart/TinyGPSPlus @ ^1.0.3

[env:native]
platform = native
framework = unity
build_flags = -I lib
```

O ambiente `native` compila e roda **no seu PC**, perfeito para testar lógicas sem precisar de hardware.

---

## 4. Gerenciamento de bibliotecas

No PlatformIO, bibliotecas são declaradas no `platformio.ini`:

```ini
lib_deps =
    sandeepmistry/LoRa
    adafruit/Adafruit BME280 Library
    mikalhart/TinyGPSPlus
    adafruit/Adafruit BMP280 Library
    esp32servo/ESP32Servo
```

O PlatformIO baixa e gerencia as versões automaticamente no `pio run`.

---

## 5. Organização modular — lib/

Módulos em `lib/` devem ser independentes de hardware para serem testáveis no PC.

### Exemplo: módulo de detecção de apogeu (inspirado no Helike)

`lib/calc/ApogeeDetection.h`:

```cpp
#pragma once

class ApogeeDetection {
public:
    ApogeeDetection(float threshold = -2.0f, int confirm_samples = 3)
        : m_threshold(threshold), m_confirm_samples(confirm_samples) {}

    bool update(float vertical_velocity) {
        if (vertical_velocity < m_threshold) {
            m_count++;
        } else {
            m_count = 0;
        }
        return m_count >= m_confirm_samples;
    }

    void reset() { m_count = 0; }

private:
    float m_threshold;
    int m_confirm_samples;
    int m_count = 0;
};
```

Este módulo:
- Não depende de nenhuma biblioteca Arduino
- Pode ser testado no PC (ambiente `native`)
- Usa a mesma lógica do Flight Computer

---

## 6. Testes nativos (Unity)

Os testes rodam no seu PC, sem precisar de placa.

`test/test_apogee/test_apogee.cpp`:

```cpp
#include <unity.h>
#include "ApogeeDetection.h"

ApogeeDetection detector;

void setUp(void) {
    detector.reset();
}

void test_apogee_not_detected_with_positive_vz(void) {
    TEST_ASSERT_FALSE(detector.update(5.0f));
    TEST_ASSERT_FALSE(detector.update(3.0f));
}

void test_apogee_detected_after_consecutive_negative_vz(void) {
    TEST_ASSERT_FALSE(detector.update(-1.0f));
    TEST_ASSERT_FALSE(detector.update(-2.5f));
    TEST_ASSERT_TRUE(detector.update(-3.0f));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_apogee_not_detected_with_positive_vz);
    RUN_TEST(test_apogee_detected_after_consecutive_negative_vz);
    UNITY_END();
}

void loop() {}
```

Para rodar:

```bash
pio test -e native
```

**No Helike** — São 25 testes unitários entre 3 módulos (Vz, apogeu, validação). Todos rodam via `pio test -e native`.

---

## 7. Compilando e fazendo upload

```bash
# Build do firmware
pio run

# Build para ambiente específico
pio run -e helike_esp32c3

# Upload
pio run -e helike_esp32c3 -t upload --upload-port /dev/ttyACM0

# Monitor serial
pio device monitor -b 115200

# Testes nativos
pio test -e native

# Testes com verbose
pio test -e native -v
```

---

## 8. Compilando sketches de hardware

Para compilar sketches individuais (como os `test_hardware/` do Helike):

```bash
pio run -e helike_esp32c3 \
    --project-option="src_dir=test_hardware/sensor/bmp280"
```

---

## 9. Boas práticas

1. **Cabeçalhos em `lib/`**: sejam header-only e sem dependência de Arduino (use apenas C++ padrão).
2. **`lib_deps` no .ini**: nunca baixe bibliotecas manualmente.
3. **Commits sem `pio run`**: `.pio/` e `.piolibdeps/` no `.gitignore`.
4. **Testes antes do firmware**: implemente e teste a lógica em `native` antes de subir pra placa.
5. **Constantes no `platformio.ini`**: use `build_flags` para definir pinos e thresholds (evite hardcoded).

---

## 10. Migrando do Arduino IDE para PlatformIO

1. Crie a estrutura de diretórios (`src/`, `lib/`, `test/`).
2. Copie o `.ino` para `src/main.cpp` e adicione `#include <Arduino.h>`.
3. Crie `platformio.ini` com as configurações da sua placa.
4. Declare as bibliotecas em `lib_deps`.
5. Separe o código em módulos em `lib/`.
6. Adicione testes em `test/`.

---

## Exercícios práticos

### Nível 1 — Setup e build

1. Crie um projeto PlatformIO do zero para ESP32-C3.
2. Configure `monitor_speed = 115200` e faça um blink.
3. Compile e faça upload.

### Nível 2 — Múltiplos ambientes

1. Adicione um ambiente `native` para testes.
2. Crie um módulo simples em `lib/` (ex: `CalculadoraMedia`) e teste no PC.

### Nível 3 — Migração

1. Pegue o sketch da Semana 3 (Arduino IDE) e migre para PlatformIO.
2. Separe sensores, comunicação e lógica em módulos dentro de `lib/`.
3. Extraia constantes (pinos, thresholds) para `build_flags` no `platformio.ini`.

### Nível 4 — Testes

1. Escreva testes para a lógica de detecção de apogeu do Flight Computer.
2. Adicione um módulo de validação de dados em `lib/` com testes.
3. Rode `pio test -e native` e verifique todos os testes passando.

---

## Conexão com os projetos reais

| Conceito | Projeto | Onde é usado |
|---|---|---|
| `platformio.ini` multi-env | Helike | `helike_esp32c3` + `native` |
| Módulos header-only | Helike | `lib/calc/` (Vz, apogeu, validação) |
| Testes Unity | Helike | 25 testes em 3 módulos |
| `build_flags` | Helike | Define pinos e thresholds |
| `lib_deps` | Ambos | LoRa, BME280, TinyGPS++, ESP32Servo |

## Entregas relacionadas

- [Semana 4](../semanas/semana-04.md): Migração Arduino IDE → PlatformIO

## Critérios de aceite

- Build sem erros em todos os ambientes
- `pio test -e native` passa
- Código organizado em `src/`, `lib/`, `test/`
- README com instruções de setup e build
