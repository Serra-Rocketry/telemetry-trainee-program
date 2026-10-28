# Entregas

Esta pasta guarda as entregas dos trainees e os dados compartilhados.

## Entregas dos alunos

Cada aluno tem a **sua própria pasta**, com o seu nome (`nome-sobrenome`), e dentro dela uma subpasta por semana.

```
entregas/
└── seu-nome-sobrenome/
    ├── README.md
    └── semana-00/
        ├── README.md
        └── hello.py
```

Regras:

- **Nunca apague o material do repositório** — você só adiciona arquivos na sua pasta aqui.
- Nomeie a pasta com o seu nome no formato `nome-sobrenome`.
- Uma subpasta por semana: `semana-00`, `semana-01`, ..., `semana-09`.
- Cada pasta de semana precisa de um `README.md`.

O passo a passo completo (primeira semana e rotina das próximas) está no [Padrão de Entregas](../docs/padrao-entregas.md).

## Dados compartilhados

A pasta `dados/` contém os arquivos de entrada do treinamento, iguais para todos os alunos (apenas para leitura):

```
entregas/dados/
├── semana-01/
│   └── cleaned.csv
├── semana-02/
│   └── voo_thonyan.txt
└── wokwi-chips/              # custom chips (sensores) para simular no Wokwi
    ├── bmp585/
    ├── lsm6ds3/
    └── neo-8m/
```

O progresso de todos fica em [docs/status.md](../docs/status.md), gerado por `scripts/gerar-status.py`.
