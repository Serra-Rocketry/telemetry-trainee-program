# Semana 7 — Contexto dos projetos (Flight Computer + Helike)

## Objetivo

Entender a arquitetura dos dois projetos principais do setor e identificar onde cada semana do treinamento se aplica.

## Preparação

- [ ] Semanas 0 a 6 concluídas
- [ ] Acesso aos repositórios `flight-computer` e `satellite`
- [ ] Ver o [Slide deck](../slides/semana-07.html)
- [ ] Fazer o [Quiz](../quiz/semana-07.html)

## Problema

### Tarefa 1 — Resumo do Flight Computer

Explore o repositório do Flight Computer e responda por escrito:

- Qual o microcontrolador? Quais sensores? Quais interfaces (I2C, SPI, UART)?
- Como a FSM de voo funciona? Quais os estados e transições?
- Como os dados são armazenados (SD, LittleFS)?
- Qual o papel do LoRa?
- O que muda da v1.0 (procedural) para a v2.0 (OOP + FreeRTOS)?

Entregue um resumo de **no máximo 1 página** com:
- Diagrama da arquitetura
- Tabela de hardware (componente, interface, função)
- Fluxo de dados: sensor → processamento → armazenamento → transmissão

### Tarefa 2 — Resumo do Helike (satélite)

Explore o repositório `satellite` e responda:

- Qual o formato do satélite (PocketQube 1P)?
- Qual o microcontrolador? Quais sensores?
- Como o projeto está organizado (PlatformIO)?
- O que tem em `lib/calc/`? Como são os testes?
- Como funciona o sistema de recuperação SRAB?
- Qual a frequência do LoRa?

Entregue um resumo de **no máximo 1 página** com:
- Diagrama do satélite (componentes principais)
- Fluxo de dados: sensor → SD → LoRa → estação terrestre
- Tabela de componentes

### Tarefa 3 — Mapa conceitual

Desenhe um mapa (Mermaid, diagrama de blocos ou mesmo ASCII) que mostre:

```
SENSORES (I2C/SPI/UART)
    ↓
FIRMWARE (ESP32-C3)
    ↓
ARMAZENAMENTO (SD/LittleFS)  ──→  LORA (RFM95W)
                                    ↓
                               ESTAÇÃO TERRESTRE (Python)
```

Indique onde cada semana do treinamento se aplica neste fluxo.

## Dicas

- Foque na arquitetura e no fluxo de dados, não em cada linha de código
- Os `docs/` de cada repositório têm `software.md` e `hardware.md` com diagramas prontos
- Use Mermaid para diagramas: é suportado nativamente pelo GitHub em markdown
- Links: busque pela pasta `test_hardware/`, `lib/calc/`, `platformio.ini`

## Critérios de aceite

- [ ] Resumo do Flight Computer com diagrama e tabela
- [ ] Resumo do Helike com diagrama e fluxo
- [ ] Mapa conceitual conectando semanas aos projetos
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Quais componentes são comuns entre o Flight Computer e o Helike?
- O que muda na estratégia de logging entre um foguete (FC) e um satélite (Helike)?
- Por que o Helike usa PlatformIO e o FC usa Arduino IDE?

## Referências
- [Repositório Flight Computer](https://github.com/Serra-Rocketry/flight-computer)

- [Repositório Helike](https://github.com/Serra-Rocketry/satellite)
- [Slide deck](../slides/semana-07.html)
- [Quiz](../quiz/semana-07.html)
