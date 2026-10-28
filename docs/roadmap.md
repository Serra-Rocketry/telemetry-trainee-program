# Roadmap Treinamento Telemetria (9 semanas)

## Formato semanal (fixo)

- Mini-aula curta (20–30 min)
- Issues da semana (1–3 tarefas)
- PR até o fim da semana
- Sessão semanal de revisão de PR (30–40 min)

---

## Semana 0 - Onboarding & GitHub

Objetivo: alinhar ferramentas e fluxo de trabalho.

Trilha principal: [GitHub](trilhas/github.md)

Entregas (issues):
- Setup (VSCode, Python, Arduino, PlatformIO, KiCad)
- Fork do repo + clone do fork
- Criar a estrutura `entregas/<nome-sobrenome>/semana-00/`
- PR "hello world" + README pessoal (dentro de `entregas/`)

Extra (opcional):
- Checklist de leitura rápida (documentação do time)

---

## Semana 1 - Python básico (telemetria)

Objetivo: Processar dados de telemetria em CSV no Google Colab.

Trilha principal: [Python](trilhas/python.md)

Entregas (issues):
- Notebook `semana-01` no Colab com o CSV tratado em pandas
- Gráficos de aceleração, velocidade e altitude
- Link público do Colab

Extra (opcional):
- Norma da aceleração e gráficos de temperatura/pressão

Conexão com projetos reais: Análise pós-voo do Flight Computer e Helike.

---

## Semana 2 - Python aplicado (pacotes)

Objetivo: Parser e validação de dados.

Trilha principal: [Python](trilhas/python.md)

Entregas (issues):
- Parser de pacote (string → dict)
- Validação simples (checksum ou tamanho)

Extra (opcional):
- Exportar JSON

Conexão com projetos reais: Formato de pacote do Helike (18 campos + '#').

---

## Semana 3 - Arduino IDE (fundamentos)

Objetivo: Programação básica de microcontroladores.

Trilha principal: [Arduino IDE](trilhas/arduino-ide.md)

Entregas (issues):
- Blink + leitura de sensor simulado
- Envio serial com pacote formatado

Extra (opcional):
- Checksum no payload

Conexão com projetos reais: GPIO, I2C, serial usados em ambos os projetos.

---

## Semana 4 - PlatformIO (organização)

Objetivo: Organização profissional de projetos.

Trilha principal: [PlatformIO](trilhas/platformio.md)

Entregas (issues):
- Mesmo projeto da Semana 3 em PlatformIO
- Organização de libs e estrutura de código

Extra (opcional):
- README de setup

Conexão com projetos reais: Helike usa PlatformIO com testes nativos.

---

## Semana 5 - Integração Arduino ↔ Python

Objetivo: Comunicação entre hardware e software.

Trilhas relacionadas: [Arduino IDE](trilhas/arduino-ide.md), [Python](trilhas/python.md)

Entregas (issues):
- Arduino envia pacotes periódicos
- Python recebe, parseia e loga

Extra (opcional):
- Detecção de pacote corrompido

Conexão com projetos reais: Telemetria LoRa → estação terrestre Python.

---

## Semana 6 - KiCad básico

Objetivo: Design de circuitos eletrônicos.

Trilha principal: [KiCad](trilhas/kicad.md)

Entregas (issues):
- Esquemático simples (sensor + MCU)
- Export PDF + checklist ERC

Extra (opcional):
- PCB simples

Conexão com projetos reais: PCB do CDB (Helike) e placa do Flight Computer.

---

## Semana 7 - Contexto dos projetos reais (FC + Helike)

Objetivo: Entender a arquitetura dos projetos reais.

Trilhas recomendadas:
- [Arquitetura de Firmware](trilhas/firmware-architecture.md)
- [Sensores e Protocolos](trilhas/sensors.md)

Entregas (issues):
- Resumo técnico do Flight Computer
- Resumo técnico do Helike
- Mapa conceitual (FSM + Telemetria + Power)
- Tabela comparativa entre os projetos

Roteiro detalhado: [Semana 7](semanas/semana-07.md)

---

## Semana 8 - Mini-projeto final (capstone)

Objetivo: Pipeline end-to-end de telemetria.

Trilhas relacionadas: Todas as anteriores

Entregas (issues):
- Pipeline completo: gerar → transmitir → parsear → logar → analisar
- Firmware com sensor + checksum + detecção de apogeu
- Script Python com receptor + análise + gráficos
- README completo com resultados

Roteiro detalhado: [Semana 8](semanas/semana-08.md)

---

## Semana 9 - Chips personalizados no Wokwi (simulação)

Objetivo: Simular o computador de bordo com os custom chips do repositório e fechar o ciclo de telemetria.

Pré-requisitos: Semanas 0 a 8 (reaproveita as Semanas 1, 2 e 3)

Entregas (issues):
- Projeto no Wokwi com ESP32-S3 + BMP585 + LSM6DS3 + NEO-8M montado do zero
- Firmware lendo os três chips e enviando pacotes a 10 Hz
- `log_wokwi.txt` com 5+ minutos de simulação
- Parser adaptado gerando `dados_tratados.csv`
- Gráficos no Colab a partir do CSV tratado

Roteiro detalhado: [Semana 9](semanas/semana-09.md)

---

## Referências das Semanas

| Semana | Roteiro |
|--------|---------|
| 0 | [semana-00.md](semanas/semana-00.md) |
| 1 | [semana-01.md](semanas/semana-01.md) |
| 2 | [semana-02.md](semanas/semana-02.md) |
| 3 | [semana-03.md](semanas/semana-03.md) |
| 4 | [semana-04.md](semanas/semana-04.md) |
| 5 | [semana-05.md](semanas/semana-05.md) |
| 6 | [semana-06.md](semanas/semana-06.md) |
| 7 | [semana-07.md](semanas/semana-07.md) |
| 8 | [semana-08.md](semanas/semana-08.md) |
| 9 | [semana-09.md](semanas/semana-09.md) |
