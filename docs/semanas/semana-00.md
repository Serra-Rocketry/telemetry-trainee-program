# Semana 0 - Onboarding & GitHub

## Objetivo

Configurar seu ambiente de desenvolvimento e fazer seu primeiro PR.

## Preparação

- [ ] Criar conta no GitHub
- [ ] Instalar Git, VSCode, Python, Arduino IDE, PlatformIO, KiCad
- [ ] Ler a [Trilha GitHub](../trilhas/github.md)

## Tarefas

### 1. Configurar Git

Defina nome e email global do Git.

### 2. Criar fork do repositório

Faça um fork de `Serra-Rocketry/telemetry-trainee-program` para a sua conta do GitHub. O fork é a sua cópia pessoal do repositório, onde você tem permissão de escrita para enviar commits e abrir Pull Requests.

### 3. Clonar o seu fork

Clone o **seu fork** (não o repositório original). Você vai trabalhar na branch `main` do seu fork.

```bash
git clone https://github.com/<seu-usuario>/telemetry-trainee-program.git
cd telemetry-trainee-program
git remote add upstream https://github.com/Serra-Rocketry/telemetry-trainee-program.git
```

### 4. Criar a estrutura de entregas

Todas as suas entregas ficam dentro de `entregas/<seu-nome-sobrenome>/`. Nesta semana, crie:

```
entregas/
└── seu-nome-sobrenome/
    ├── README.md                 # índice pessoal: quem você é + status das semanas
    └── semana-00/
        ├── README.md             # o que foi feito nesta semana
        └── hello.py              # script "hello world"
```

Regras:
- **Nunca apague o material do repositório** (docs, slides, quizzes, templates). Você só adiciona arquivos na sua pasta.
- Uma subpasta por semana: `semana-00`, `semana-01`, ..., `semana-09`.
- Cada pasta de semana precisa de um `README.md` explicando o que foi feito e como executar.

Veja o [Padrão de Entregas](../padrao-entregas.md) para a estrutura completa.

### 5. README pessoal

Crie `entregas/<seu-nome-sobrenome>/README.md` com:
- Seu nome
- Seus objetivos no treinamento
- Experiência prévia (faculdade, estágio, projetos)
- Um checklist das semanas (marque conforme entrega)

### 6. Script hello world

Crie `entregas/<seu-nome-sobrenome>/semana-00/hello.py`:

```python
def main():
    print("Hello from telemetry trainee!")

if __name__ == "__main__":
    main()
```

### 7. Commit e push

Commit usando tipo semântico (`docs: ...` ou `feat: ...`) e envie para o GitHub.

```bash
git add entregas/seu-nome-sobrenome
git commit -m "docs: adiciona README pessoal e hello world da semana 00"
git push origin main
```

### 8. Abrir Pull Request

Abra um PR da `main` do seu fork para a `main` do repositório original, seguindo o [template de PR](../../templates/pr-template.md).

### 9. Rotina das próximas semanas

A cada nova semana, depois que o PR anterior for mergeado:

1. Sincronize seu fork com o repositório original (`git pull upstream main` + `git push origin main`).
2. Crie a pasta `entregas/<seu-nome-sobrenome>/semana-XX/`.
3. Faça as tarefas, commit e push.
4. Abra **outra Pull Request** com os commits da nova semana.

Os comandos completos estão no [Padrão de Entregas](../padrao-entregas.md#próximas-semanas-repetir-a-cada-semana).

## Critérios de aceite

- [ ] Fork do repositório criado na sua conta do GitHub
- [ ] Pasta `entregas/<seu-nome-sobrenome>/` criada, sem apagar o material do repositório
- [ ] `entregas/<seu-nome-sobrenome>/README.md` pessoal criado
- [ ] `entregas/<seu-nome-sobrenome>/semana-00/hello.py` executando
- [ ] Commit com tipo semântico
- [ ] PR segue o template

## Perguntas para reflexão

- Qual a diferença entre `clone`, `pull` e `fetch`?
- O que acontece se duas pessoas editarem a mesma linha em branches diferentes?
- Por que usamos branches em vez de commitar direto na `main`?

## Dicas

- `git status` é seu melhor amigo - use antes de qualquer commit
- `git commit --amend` corrige a mensagem do último commit (só se ainda não deu push)
- A cada semana você abre uma **nova PR** com os commits daquela semana (veja a [rotina](../padrao-entregas.md#rotina-semanal-do-aluno))

## Referências

- [Padrão de Entregas](../padrao-entregas.md)
- [Trilha GitHub](../trilhas/github.md)
- [Exemplo GitHub](../examples/github-exemplo.md)
- [Slide deck](../slides/semana-00.html)
- [Quiz](../quiz/semana-00.html)
