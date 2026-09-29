# Estrutura do Repositório de Treinamento

## Objetivo

Padronizar organização e facilitar o onboarding dos novos membros.

## Estrutura completa

```
treinamento-telemetria/
├── docs/
│   ├── roadmap-8-semanas.md
│   ├── estrutura-repo.md
│   ├── calendario.md
│   ├── index.md
│   ├── guia-instrutor.md
│   ├── boas-praticas-pr.md
│   ├── padrao-branches-prs.md
│   ├── guia-do-aluno.md
│   ├── faq.md
│   ├── visao-geral.md
│   ├── checklist-release-candidate.md
│   ├── issues-semanais/
│   │   ├── README.md
│   │   └── issue-semana-*.md
│   ├── semanas/
│   │   ├── semana-00.md
│   │   ├── semana-01.md
│   │   ├── ...
│   │   └── semana-08.md
│   ├── trilhas/
│   │   ├── README.md
│   │   ├── python.md
│   │   ├── arduino-ide.md
│   │   ├── platformio.md
│   │   ├── kicad.md
│   │   ├── github.md
│   │   ├── firmware-architecture.md  # NOVA
│   │   └── sensors.md               # NOVA
│   ├── examples/
│   │   ├── README.md
│   │   ├── python-exemplo.md
│   │   ├── arduino-ide-exemplo.md
│   │   ├── platformio-exemplo.md
│   │   ├── kicad-exemplo.md
│   │   └── github-exemplo.md
│   ├── slides/
│   └── quiz/
├── templates/
│   ├── issue-template.md
│   ├── issue-semanal-modelo.md
│   └── pr-template.md
├── examples/
│   ├── python/
│   ├── arduino/
│   └── platformio/
├── README.md
├── README-alunos.md
└── _config.yml
```

## Trilhas de Conteúdo

| Trilha | Arquivo | Descrição |
|--------|---------|-----------|
| Python | `trilhas/python.md` | Análise de dados, parsers, matplotlib |
| Arduino IDE | `trilhas/arduino-ide.md` | GPIO, I2C, SPI, serial, LoRa |
| PlatformIO | `trilhas/platformio.md` | Organização, testes nativos, multi-env |
| GitHub | `trilhas/github.md` | Git, branches, issues, PRs |
| KiCad | `trilhas/kicad.md` | Esquemático, PCB, ERC |
| Arquitetura de Firmware | `trilhas/firmware-architecture.md` | FreeRTOS, FSM, OOP |
| Sensores e Protocolos | `trilhas/sensors.md` | I2C, SPI, sensores, LoRa |

## Observações

- As trilhas podem ter links para material externo, mas as tarefas ficam nas issues.
- Cada semana referencia uma trilha principal e uma entrega verificável.
- A pasta `examples/` deve conter códigos simples e testáveis.
- As trilhas de Arquitetura de Firmware e Sensores são complementares e conectam diretamente com os projetos reais (Flight Computer e Helike).
