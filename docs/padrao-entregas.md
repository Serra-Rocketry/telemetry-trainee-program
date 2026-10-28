# Padrão de Entregas

Este documento define **onde** e **como** cada trainee entrega as tarefas do treinamento. Siga ele à risca: a estrutura é padronizada para facilitar a sua organização, a revisão dos mentores e o acompanhamento do seu progresso.

## Regra de ouro

> **Nunca apague o material do repositório** (`docs/`, `assets/`, `templates/`, `_layouts/`, `README.md`, etc.).
> Você só **adiciona** arquivos dentro da sua pasta em `entregas/`. O material do programa é a referência de todos e precisa continuar intacto.

## Estrutura de pastas

Cada trainee tem uma pasta com o seu nome, e dentro dela uma subpasta por semana:

```
entregas/
└── seu-nome-sobrenome/
    ├── README.md                     # índice pessoal (quem você é + status das semanas)
    ├── semana-00/
    │   ├── README.md                 # o que foi feito + como executar
    │   ├── hello.py
    │   └── evidencias/
    │       └── saida-hello.txt
    ├── semana-01/
    │   ├── README.md
    │   ├── leitor.py
    │   └── evidencias/
    ├── semana-02/
    │   └── ...
    ├── semana-08/
    │   └── ...
    └── semana-09/
        └── ...
```

### Regras da estrutura

1. **Uma pasta por trainee**: `entregas/<seu-nome-sobrenome>/`, em minúsculas e com hífen (ex.: `maria-silva`).
2. **Uma subpasta por semana**: `semana-00`, `semana-01`, ..., `semana-09` (sempre com dois dígitos).
3. **`README.md` obrigatório** em cada pasta de semana, explicando o que foi feito e como executar.
4. **Não misture semanas**: cada entrega fica apenas na pasta da semana correspondente.
5. **Evidências** (logs, prints, saídas de scripts) vão em uma subpasta `evidencias/` dentro da semana.
6. **Nada de arquivos na raiz**: o único lugar onde você escreve é dentro de `entregas/<seu-nome-sobrenome>/`.

## O que cada semana deve conter

| Semana | Pasta | Conteúdo mínimo |
|---|---|---|
| 0 | `semana-00/` | `README.md` + `hello.py` |
| 1 | `semana-01/` | `README.md` com o link do notebook Colab (pandas + gráficos) |
| 2 | `semana-02/` | `README.md` + script do parser + `dados_tratados.csv` |
| 3 | `semana-03/` | `README.md` + sketch Arduino |
| 4 | `semana-04/` | `README.md` + projeto PlatformIO + testes |
| 5 | `semana-05/` | `README.md` + código de integração Arduino ↔ Python |
| 6 | `semana-06/` | `README.md` + arquivos KiCad + PDF do esquemático |
| 7 | `semana-07/` | `README.md` + resumos/mapa conceitual |
| 8 | `semana-08/` | `README.md` + projeto final (capstone) |
| 9 | `semana-09/` | `README.md` + `sketch.ino` + `log_wokwi.txt` + `parser.py` + `dados_tratados.csv` |

> Os detalhes exatos de cada entrega estão na semana correspondente em [`docs/semanas/`](semanas/) e na issue semanal.

## Dados compartilhados

A pasta [`entregas/dados/`](../entregas/dados/) contém os arquivos de entrada do treinamento, iguais para todos e **apenas para leitura**:

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

Não edite nem apague esses arquivos - apenas leia-os nos seus scripts (ou use os chips no Wokwi).

## Rotina semanal do aluno

Você trabalha direto na branch `main` do **seu fork** e abre um PR para o repositório original a cada semana.

### Primeira semana (Semana 0)

```bash
# 1. Clone o SEU fork (não o repositório original)
git clone https://github.com/<seu-usuario>/telemetry-trainee-program.git
cd telemetry-trainee-program

# 2. Adicione o repositório original como "upstream" (para sincronizar depois)
git remote add upstream https://github.com/Serra-Rocketry/telemetry-trainee-program.git

# 3. Crie a estrutura de entregas da semana
mkdir -p entregas/seu-nome-sobrenome/semana-00/evidencias
# ... crie os arquivos (README.md, hello.py) dentro dela ...

# 4. Commit e push (na main do seu fork)
git add entregas/seu-nome-sobrenome
git commit -m "docs: adiciona README pessoal e hello world da semana 00"
git push origin main
```

Depois, no GitHub: abra um **Pull Request** da `main` do seu fork para a `main` do repositório original (`Serra-Rocketry:main`) e siga o [template de PR](../templates/pr-template.md).

### Próximas semanas (repetir a cada semana)

Quando o PR da semana anterior for **mergeado**, a `main` do seu fork fica desatualizada em relação ao repositório original. Antes de começar a nova semana, sincronize:

```bash
# 1. Atualize a main do seu fork com o repositório original
git checkout main
git pull upstream main
git push origin main

# 2. Crie a pasta da nova semana
mkdir -p entregas/seu-nome-sobrenome/semana-01/evidencias
# ... faça as tarefas da semana ...

# 3. Commit e push
git add entregas/seu-nome-sobrenome/semana-01
git commit -m "feat: adiciona leitura e analise do CSV da semana 01"
git push origin main
```

Em seguida, **abra uma nova Pull Request** da `main` do seu fork para a `main` original. Como os commits da semana anterior já foram mergeados, o novo PR mostrará apenas os commits da nova semana.

> Alternativa sem terminal: use o botão **"Sync fork" → "Update branch"** no GitHub antes de começar a semana, e abra o PR com os commits novos.

### Resumo do ciclo

```
Sincronizar fork  →  criar pasta da semana  →  fazer as tarefas
        →  commit  →  push  →  abrir PR  →  mentor revisa  →  merge
```

E então o ciclo recomeça na próxima semana.

## Rotina do mentor

1. O trainee abre o PR da semana.
2. O mentor revisa `entregas/<nome>/semana-XX/` usando a checklist "Na revisão" da semana no [Guia do Instrutor](guia-instrutor.md).
3. Se estiver correto, o mentor faz o **merge**.
4. O status do trainee é atualizado automaticamente (veja abaixo).

## Página de status

O progresso de todos os trainees fica em [`status.md`](status.md), gerado a partir das pastas em `entregas/`:

```bash
python scripts/gerar-status.py
```

O script varre `entregas/*/semana-*/`, monta a tabela **aluno × semana** e reescreve `docs/status.md`. O workflow `.github/workflows/status.yml` regenera a página automaticamente a cada push na `main`.

## Referências

- [Guia do Aluno](guia-do-aluno.md)
- [Guia do Instrutor](guia-instrutor.md)
- [Estrutura do Repositório](estrutura-repo.md)
- [Trilha GitHub](trilhas/github.md)
