# Roadmap Treinamento Telemetria (8 semanas)

## Formato semanal (fixo)

- **Mini-aula curta** (20–30 min)
- **Issues da semana** (1–3 tarefas)
- **PR até o fim da semana**
- **Sessão semanal de revisão de PR** (30–40 min)

---

## Semana 0 — Onboarding & GitHub

**Objetivo**: alinhar ferramentas e fluxo de trabalho.

**Entregas (issues)**:
- Setup (VSCode, Python, Arduino, PlatformIO, KiCad)
- Clonar repo + criar branch `nome-sobrenome`
- PR “hello world” + README pessoal

**Extra (opcional)**:
- Checklist de leitura rápida (documentação do time)

---

## Semana 1 — Python básico (telemetria)

**Entregas (issues)**:
- Script lê CSV de telemetria e imprime estatísticas
- Salvar resumo em arquivo

**Extra (opcional)**:
- Gráfico simples (matplotlib)

---

## Semana 2 — Python aplicado (pacotes)

**Entregas (issues)**:
- Parser de pacote (string → dict)
- Validação simples (checksum ou tamanho)

**Extra (opcional)**:
- Exportar JSON

---

## Semana 3 — Arduino IDE (fundamentos)

**Entregas (issues)**:
- Blink + leitura de sensor simulado
- Envio serial com pacote formatado

**Extra (opcional)**:
- Checksum no payload

---

## Semana 4 — PlatformIO (organização)

**Entregas (issues)**:
- Mesmo projeto da Semana 3 em PlatformIO
- Organização de libs e estrutura de código

**Extra (opcional)**:
- README de setup

---

## Semana 5 — Integração Arduino ↔ Python

**Entregas (issues)**:
- Arduino envia pacotes periódicos
- Python recebe, parseia e loga

**Extra (opcional)**:
- Detecção de pacote corrompido

---

## Semana 6 — KiCad básico

**Entregas (issues)**:
- Esquemático simples (sensor + MCU)
- Export PDF + checklist ERC

**Extra (opcional)**:
- PCB simples

---

## Semana 7 — Contexto dos projetos reais (FC + Helike)

**Entregas (issues)**:
- Resumo técnico do Flight Computer
- Resumo técnico do Helike
- Mapa conceitual (FSM + Telemetria + Power)

---

## Semana 8 — Mini-projeto final (capstone)

**Entregas (issues)**:
- Pipeline end-to-end: gerar → transmitir → parsear → logar
- PR final com README + lições aprendidas
**Roteiro detalhado**: [Semana 0](semanas/semana-00.md)
**Roteiro detalhado**: [Semana 1](semanas/semana-01.md)
**Roteiro detalhado**: [Semana 2](semanas/semana-02.md)
**Roteiro detalhado**: [Semana 3](semanas/semana-03.md)
**Roteiro detalhado**: [Semana 4](semanas/semana-04.md)
**Roteiro detalhado**: [Semana 5](semanas/semana-05.md)
**Roteiro detalhado**: [Semana 6](semanas/semana-06.md)
**Roteiro detalhado**: [Semana 7](semanas/semana-07.md)
**Roteiro detalhado**: [Semana 8](semanas/semana-08.md)
