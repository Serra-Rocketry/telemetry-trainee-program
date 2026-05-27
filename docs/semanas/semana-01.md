# Semana 1 — Python básico

## Objetivo

Ler um CSV de telemetria, calcular estatísticas e salvar um resumo.

## Preparação

- [ ] Python 3.10+ instalado
- [ ] Ler a [Trilha Python](../trilhas/python.md) (seções 1.1 e 2.1)
- [ ] Ver o [Slide deck](../slides/semana-01.html)
- [ ] Fazer o [Quiz](../quiz/semana-01.html)

## Problema

Você recebeu um arquivo CSV com dados de telemetria de um voo de teste. Seu script deve:

1. **Ler** o arquivo CSV — cada linha é uma amostra
2. **Calcular** para o campo `altitude_m`: média, máximo, mínimo, número de amostras
3. **Exibir** na tela de forma legível
4. **Salvar** o resumo em um arquivo `.txt`

## Requisitos

- Use `csv.DictReader` da biblioteca padrão (sem pandas)
- O caminho do CSV deve ser recebido como argumento (`sys.argv`)
- Trate arquivo inexistente com mensagem de erro amigável
- Trate CSV vazio (0 linhas) sem quebrar

## Formato de saída

```
Arquivo: dados.csv
Amostras: 5
Altitude media: 1.36 m
Altitude max:   3.10 m
Altitude min:   0.00 m
Duracao:        0.20 s
```

## Dicas

- `csv.DictReader` retorna um iterador de dicionários — converta para lista se precisar re-percorrer
- `float()` lança `ValueError` se o valor não for numérico — decida como tratar
- `sys.argv[1]` pega o primeiro argumento depois do nome do script
- `pathlib.Path.exists()` ou `os.path.exists()` para verificar se o arquivo existe

## Extra (opcional)

- Gere um gráfico de altitude × tempo com `matplotlib` e salve como PNG
- Adicione mais campos: `temperatura_C`, `pressao_hPa`
- Calcule a duração total a partir do timestamp (último - primeiro, em segundos)

## Critérios de aceite

- [ ] Script roda com `python3 leitor.py dados.csv` sem erros
- [ ] Estatísticas corretas (confira na mão com 3 linhas)
- [ ] Resumo salvo em `.txt`
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- O que acontece se o CSV tiver uma linha com letras no lugar de números?
- Por que `csv.DictReader` é melhor que `split(",")`?
- `with open(...) as f`: o que o `with` garante?

## Referências

- [Trilha Python](../trilhas/python.md)
- [Exemplo Python](../examples/python-exemplo.md)
- [Slide deck](../slides/semana-01.html)
- [Quiz](../quiz/semana-01.html)
