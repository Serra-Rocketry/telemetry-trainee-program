# Semana 4 — PlatformIO

## Objetivo

Migrar o projeto da Semana 3 do Arduino IDE para PlatformIO com estrutura modular e testes.

## Preparação

- [ ] Semana 3 concluída (código funcional no Arduino IDE)
- [ ] VS Code + PlatformIO extension
- [ ] Ler a [Trilha PlatformIO](../trilhas/platformio.md) (seções 1 a 5)
- [ ] Ver o [Slide deck](../slides/semana-04.html)
- [ ] Fazer o [Quiz](../quiz/semana-04.html)

## Problema

### Tarefa 1 — Criar projeto

Crie um projeto PlatformIO para ESP32-C3 com a estrutura:

```
projeto/
├── platformio.ini
├── src/
│   └── main.cpp
├── lib/
│   └── calc/
│       └── SensorData.h
└── test/
    └── test_sensor/
        └── test_sensor.cpp
```

### Tarefa 2 — Configurar platformio.ini

Crie dois ambientes:
- `esp32c3`: para o firmware (board `esp32-c3-devkitm-1`, framework arduino)
- `native`: para testes no PC (framework unity)

Adicione em `build_flags`: os pinos I2C como defines (`I2C_SDA=4`, `I2C_SCL=5`).
Adicione em `lib_deps`: `Adafruit BMP280 Library`.

### Tarefa 3 — Migrar o código

Transfira o código da Semana 3 para `src/main.cpp`:
- Adicione `#include <Arduino.h>` (no PlatformIO, .ino não existe mais)
- Use os defines do `build_flags` em vez de números hardcoded
- O código deve compilar com `pio run -e esp32c3`

### Tarefa 4 — Módulo SensorData

Crie `lib/calc/SensorData.h` com:
- Uma `struct SensorData` com os campos do pacote
- Um método `bool isValid()` que valida os ranges físicos (reaproveite a lógica da Semana 2)
- Header-only (sem `.cpp`), sem dependência de bibliotecas Arduino

### Tarefa 5 — Teste nativo

Crie `test/test_sensor/test_sensor.cpp` que testa:
- Um `SensorData` válido retorna `true`
- Um com altitude > 10000 retorna `false`
- Um com temperatura < -40 retorna `false`

Use Unity framework (`TEST_ASSERT_TRUE`, `TEST_ASSERT_FALSE`).

Rode: `pio test -e native`

## Dicas

- Em `lib/calc/SensorData.h`, use apenas C++ padrão — nada de `Arduino.h`, `Wire.h`, etc.
- `build_flags` no platformio.ini: por exemplo `-DI2C_SDA=4` — no código use `I2C_SDA` diretamente
- `lib_deps` aceita formato `autor/nome @ versão`
- Comando pra compilar sketch de teste: `pio run -e esp32c3 --project-option="src_dir=caminho"`

## Extra (opcional)

- Adicione um módulo `VerticalVelocity.h` que calcula Vz por diferença finita
- Adicione um módulo `ApogeeDetection.h` que detecta apogeu por threshold de Vz
- Escreva testes para ambos

## Critérios de aceite

- [ ] `pio run -e esp32c3` compila sem erros
- [ ] Upload funciona e serial exibe dados
- [ ] `pio test -e native` passa
- [ ] Código organizado em `src/`, `lib/`, `test/`
- [ ] `SensorData.h` sem dependência Arduino
- [ ] README com instruções de build
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Por que separar a lógica em `lib/` e mantê-la sem dependência Arduino?
- Qual a vantagem de ter um ambiente `native` além do `esp32c3`?
- O que `build_flags` resolve que #define no código não resolve?

## Referências

- [Trilha PlatformIO](../trilhas/platformio.md)
- [Exemplo PlatformIO](../examples/platformio-exemplo.md)
- [Slide deck](../slides/semana-04.html)
- [Quiz](../quiz/semana-04.html)
