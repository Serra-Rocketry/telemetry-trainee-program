# Exemplo — GitHub: Fluxo de PR

## Objetivo

Mostrar o fluxo completo de contribuição com branches e PRs.

## Passo a passo

```bash
# 1. Clonar o repositório
git clone https://github.com/Serra-Rocketry/telemetry-trainee-program.git
cd telemetry-trainee-program

# 2. Criar branch pessoal
git checkout -b maria-silva

# 3. Fazer alterações
echo "# Maria Silva - Trainee Telemetria" > README.md
echo "Objetivo: aprender Python e Arduino para atuar no Helike" >> README.md

# 4. Commitar (use o prefixo da tecnologia)
git add README.md
git commit -m "docs: adiciona README pessoal com objetivo do treinamento"

# 5. Enviar para o GitHub
git push -u origin maria-silva

# 6. Abrir PR no GitHub:
#    - Ir até o repositório
#    - Clicar em "Pull Request"
#    - Base: main ← Compare: maria-silva
#    - Preencher template

# 7. Após aprovação, fazer merge
```

## Template de PR

```markdown
## Semana 0 — Onboarding & GitHub

## O que foi feito
- [x] README pessoal criado
- [x] Branch `maria-silva` criada

## Como testar
1. `git checkout maria-silva`
2. Verificar README.md na raiz

## Critérios de aceite
- [x] Branch nomeada corretamente
- [x] PR segue template

## Dificuldades
- Nenhuma — tranquilo!
```
