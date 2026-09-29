# Semana 7 - Contexto dos projetos (Flight Computer + Helike)

## Objetivo

Entender a arquitetura dos dois projetos principais do setor e identificar onde cada semana do treinamento se aplica.

## Preparação

- [ ] Semanas 0 a 6 concluídas
- [ ] Acesso aos repositórios `flight-computer` e `satellite`
- [ ] Ver o [Slide deck](../slides/semana-07.html)
- [ ] Fazer o [Quiz](../quiz/semana-07.html)
- [ ] Ler as trilhas de [Arquitetura de Firmware](../trilhas/firmware-architecture.md) e [Sensores](../trilhas/sensors.md)

## Problema

### Tarefa 1 - Resumo do Flight Computer

Explore o repositório do Flight Computer e responda por escrito:

Arquitetura:
- Qual o microcontrolador? (ESP32-S3, dual-core 240MHz)
- Quais sensores? (BMP585, LSM6DS3, NEO-8M)
- Quais interfaces? (I2C para sensores, SPI para LoRa+SD, UART para GPS)
- Como o FreeRTOS organiza as tasks? (3 tasks em 2 cores)

FSM de voo:
- Quais os 4 estados? (IDLE → ASCENT → DESCENT → LANDED)
- Quais os 7 sub-event flags? (liftoff, burnout, apogeu, freefall, parachute)
- Como o paraquedas é disparado? (apogeu + Vz negativa confirmada)

Arquivo de configuração:
- Estude `firmware/config.h` - quais parâmetros estão definidos?
- Entenda os thresholds: `LIFTOFF_THRESHOLD`, `BURNOUT_THRESHOLD`, `PARACHUTE_MIN_ALTITUDE`

Entregue um resumo de no máximo 1 página com:
- Diagrama da arquitetura (Mermaid)
- Tabela de hardware (componente, interface, função)
- Fluxo de dados: sensor → processamento → armazenamento → transmissão

### Tarefa 2 - Resumo do Helike (satélite)

Explore o repositório `satellite` e responda:

Plataforma:
- Qual o formato? (PocketQube 1P)
- Qual o microcontrolador? (ESP32-C3, single-core RISC-V)
- Por que não usa FreeRTOS? (Satélite liga já descendo, loop simples)

Organização:
- Como o PlatformIO está configurado? (2 ambientes: helike_esp32c3 + native)
- O que tem em `lib/calc/`? (VerticalVelocity, ApogeeDetection, DataValidation)
- Como são os testes? (25 testes Unity em 3 módulos)

Sensores e comunicação:
- Quais sensores? (BME280, ICM-20602, NEO-8M)
- Qual a frequência do LoRa? (915 MHz, SF7, 125 kHz)
- Como é o formato do pacote? (18 campos CSV + terminador '#')

Entregue um resumo de no máximo 1 página com:
- Diagrama do satélite (componentes principais)
- Fluxo de dados: sensor → SD → LoRa → estação terrestre
- Tabela de componentes

### Tarefa 3 - Mapa conceitual

Desenhe um mapa (Mermaid, diagrama de blocos ou mesmo ASCII) que mostre:

```
SENSORES (I2C/SPI/UART)
    ↓
FIRMWARE (ESP32)
    ├─→ FreeRTOS (Flight Computer) ou Loop simples (Helike)
    ├─→ FSM de voo (apenas FC)
    ├─→ Validação de dados (ambos)
    ↓
ARMAZENAMENTO (SD/LittleFS)  ──→  LORA (RFM95W)
                                    ↓
                               ESTAÇÃO TERRESTRE (Python)
```

Indique onde cada semana do treinamento se aplica:

| Semana | Tema | Onde aparece nos projetos |
|--------|------|---------------------------|
| 0 | Setup & GitHub | Repositórios, issues, PRs |
| 1 | Python CSV | Análise pós-voo, logs |
| 2 | Python parser | Validação de pacotes |
| 3 | Arduino GPIO | LED, buzzer, servo |
| 4 | PlatformIO | Projeto Helike (completo) |
| 5 | Integração Arduino↔Python | Telemetria em tempo real |
| 6 | KiCad | PCB do CDB (Helike) |
| 7 | Contexto reais | Esta semana |
| 8 | Capstone | Pipeline end-to-end |

### Tarefa 4 - Comparação técnica

Crie uma tabela comparando os dois projetos:

| Aspecto | Flight Computer | Helike |
|---------|----------------|--------|
| MCU | ESP32-S3 (dual-core) | ESP32-C3 (single-core) |
| SO | FreeRTOS | Loop simples |
| FSM | 4 estados + 7 sub-events | Não tem |
| Barômetro | BMP585 | BME280 |
| IMU | LSM6DS3 | ICM-20602 |
| GPS | NEO-8M | NEO-8M |
| LoRa | 915 MHz, +17 dBm | 915 MHz, +20 dBm |
| Storage | SD → LittleFS | SD → LittleFS |
| Telemetria | 22 campos CSV | 18 campos CSV + '#' |
| Testes | Hardware validation | 25 testes nativos |
| Build | Arduino IDE | PlatformIO |

## Dicas

- Foque na arquitetura e no fluxo de dados, não em cada linha de código
- Os `docs/` de cada repositório têm `software.md` e `hardware.md` com diagramas prontos
- Use Mermaid para diagramas: é suportado nativamente pelo GitHub em markdown
- Links úteis:
  - Flight Computer: `firmware/config.h`, `firmware/REFAITORING_PLAN.md`
  - Helike: `lib/calc/`, `test/`, `platformio.ini`
  - Trilhas: [Arquitetura de Firmware](../trilhas/firmware-architecture.md), [Sensores](../trilhas/sensors.md)

## Critérios de aceite

- [ ] Resumo do Flight Computer com diagrama e tabela
- [ ] Resumo do Helike com diagrama e fluxo
- [ ] Mapa conceitual conectando semanas aos projetos
- [ ] Tabela comparativa entre os dois projetos
- [ ] Commit com tipo semântico (`docs:`)

## Perguntas para reflexão

- Quais componentes são comuns entre o Flight Computer e o Helike?
- O que muda na estratégia de logging entre um foguete (FC) e um satélite (Helike)?
- Por que o Helike usa PlatformIO e o FC usa Arduino IDE?
- Como o FreeRTOS ajuda no Flight Computer? Seria necessário no Helike?
- Por que o Helike tem um terminador '#' no pacote e o FC não?

## Referências
- [Repositório Flight Computer](https://github.com/Serra-Rocketry/flight-computer)

- [Repositório Helike](https://github.com/Serra-Rocketry/satellite)
- [Slide deck](../slides/semana-07.html)
- [Quiz](../quiz/semana-07.html)
- [Trilha Arquitetura de Firmware](../trilhas/firmware-architecture.md)
- [Trilha Sensores](../trilhas/sensors.md)
