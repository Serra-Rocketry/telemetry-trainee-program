# Estrutura do Repositório de Treinamento

## Objetivo

Padronizar organização e facilitar o onboarding dos novos membros.

## Estrutura sugerida

```
treinamento-telemetria/
├── docs/
│ ├── roadmap-8-semanas.md
│ ├── estrutura-repo.md
│ ├── calendario.md
│ ├── index.md
│ ├── guia-instrutor.md
│ ├── boas-praticas-pr.md
│ ├── padrao-branches-prs.md
│ ├── guia-do-aluno.md
│ ├── faq.md
│ ├── visao-geral.md
│ ├── checklist-release-candidate.md
│ ├── issues-semanais/
│ │ ├── README.md
│ │ ├── issue-semana-00.md
│ │ ├── issue-semana-01.md
│ │ ├── issue-semana-02.md
│ │ ├── issue-semana-03.md
│ │ ├── issue-semana-04.md
│ │ ├── issue-semana-05.md
│ │ ├── issue-semana-06.md
│ │ ├── issue-semana-07.md
│ │ └── issue-semana-08.md
│ ├── semanas/
│ │ ├── semana-00.md
│ │ ├── semana-01.md
│ │ ├── semana-02.md
│ │ ├── semana-03.md
│ │ ├── semana-04.md
│ │ ├── semana-05.md
│ │ ├── semana-06.md
│ │ ├── semana-07.md
│ │ └── semana-08.md
│ └── trilhas/
│ ├── python.md
│ ├── arduino-ide.md
│ ├── platformio.md
│ ├── kicad.md
│ └── github.md
├── docs/examples/
│ ├── README.md
│ ├── python-exemplo.md
│ ├── arduino-ide-exemplo.md
│ ├── platformio-exemplo.md
│ ├── kicad-exemplo.md
│ └── github-exemplo.md
├── templates/
│ ├── issue-template.md
│ ├── issue-semanal-modelo.md
│ └── pr-template.md
├── examples/
│ ├── python/
│ ├── arduino/
│ └── platformio/
└── README.md
```

## Observacoes

- As trilhas podem ter **links para material externo**, mas as tarefas ficam nas issues.
- Cada semana referencia uma trilha principal e uma entrega verificavel.
- A pasta `examples/` deve conter códigos simples e testáveis.
