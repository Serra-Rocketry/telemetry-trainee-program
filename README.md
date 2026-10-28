# Treinamento Telemetria - Serra Rocketry

Repositório de treinamento do time de telemetria, com trilhas, semanas e entregas semanais via issues e PRs.

## Comece aqui

- [Guia do Aluno](docs/guia-do-aluno.md)
- [Padrão de Entregas](docs/padrao-entregas.md)
- [Roadmap do Treinamento (9 semanas)](docs/roadmap.md)
- [Calendario](docs/calendario.md)
- [Pagina inicial (GitHub Pages)](docs/index.md)
- [Mapa Geral](docs/mapa-geral.md)
- [Status dos trainees](docs/status.md)
- [Exemplos Visuais](docs/exemplos-visuais.md)

## Trilhas de Conteúdo

| Trilha | Descrição |
|--------|-----------|
| [Python](docs/trilhas/python.md) | Análise de dados, parsers, visualização |
| [Arduino IDE](docs/trilhas/arduino-ide.md) | GPIO, I2C, SPI, serial, LoRa |
| [PlatformIO](docs/trilhas/platformio.md) | Organização de projetos, testes nativos |
| [GitHub](docs/trilhas/github.md) | Git, branches, issues, PRs |
| [KiCad](docs/trilhas/kicad.md) | Esquemático, PCB, ERC |
| [Arquitetura de Firmware](docs/trilhas/firmware-architecture.md) | FreeRTOS, FSM, padrões OOP |
| [Sensores e Protocolos](docs/trilhas/sensors.md) | I2C, SPI, sensores, LoRa |

## Semanas do Treinamento

| Semana | Tema | Entregas |
|--------|------|----------|
| 0 | Setup & GitHub | PR "hello world" |
| 1 | Python básico (Colab) | pandas + gráficos |
| 2 | Python aplicado | Parser + validação |
| 3 | Arduino IDE | Blink + sensor + serial |
| 4 | PlatformIO | Migração + testes |
| 5 | Integração | Arduino ↔ Python |
| 6 | KiCad | Esquemático + ERC |
| 7 | Projetos reais | Resumo FC + Helike |
| 8 | Capstone | Pipeline end-to-end |
| 9 | Chips personalizados no Wokwi | Simulação do computador de bordo |

## Padrões de entrega

- Fork do repositório (trabalhe na `main` do seu fork)
- Entrega em `entregas/<nome-sobrenome>/semana-XX/` (sem apagar o material do repo)
- PR com template preenchido
- README da semana com instruções de execução
- Commits com tipos semânticos (`feat:`, `fix:`, `docs:`, `test:`)

Detalhes completos no [Padrão de Entregas](docs/padrao-entregas.md).

## Estrutura

Consulte [estrutura-repo](docs/estrutura-repo.md) para a estrutura completa.

## Projetos de Referência

Este treinamento é baseado em dois projetos reais da Serra Rocketry:

| Projeto | Descrição | Tecnologias |
|---------|-----------|-------------|
| [Flight Computer](https://github.com/ViniciusCMB/flight-computer) | Computador de bordo para foguete | ESP32-S3, FreeRTOS, BMP585, LoRa |
| [Helike (Satellite)](https://github.com/ViniciusCMB/satellite) | Satélite PocketQube | ESP32-C3, PlatformIO, BME280, LoRa |


