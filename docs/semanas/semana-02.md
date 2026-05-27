# Semana 2 — Python aplicado (pacotes)

## Objetivo

Implementar um parser de pacotes de telemetria e um validador de dados.

## Preparação

- [ ] Semana 1 concluída
- [ ] Ler a [Trilha Python](../trilhas/python.md) (seções 1.2 e 1.3)
- [ ] Ver o [Slide deck](../slides/semana-02.html)
- [ ] Fazer o [Quiz](../quiz/semana-02.html)

## Problema

Dados de telemetria chegam como strings cruas pela serial no formato:

```
#timestamp_ms;altitude_m;temperatura_C;pressao_hPa;ax;ay;az#
```

Exemplo real:
```
#0;0.0;25.0;1013.25;0.01;0.02;9.81#
#50;0.5;24.9;1012.80;0.02;0.01;9.80#
```

Seu script deve:

### Tarefa 1 — Parser

Função que recebe uma string bruta e retorna um dicionário com os campos tipados.

**Regras do parser:**
- Rejeitar linhas que não começam/terminam com `#`
- Rejeitar linhas com número de campos diferente do esperado
- Rejeitar campos não numéricos (ex: letra onde deveria vir número)
- Retornar `None` para linhas inválidas (não levantar exceção)

### Tarefa 2 — Validador

Função que recebe o dicionário do parser e valida os ranges físicos:

- Altitude: -100 a 10000 m
- Temperatura: -40 a 85 °C
- Pressão: 300 a 1100 hPa
- Norma da aceleração: 8 a 11 m/s² (deve ser ~9.81 em repouso)

Retorna `True`/`False`.

### Tarefa 3 — Processador

Leia um arquivo linha por linha, aplique parser + validador, e exiba:

```
Processando...
Linha 1: OK
Linha 2: OK
Linha 3: FALHA (parser) — formato invalido
Linha 4: OK
Linha 5: REJEITADO (validacao) — altitude fora do range

Resumo:
  Total:      100
  Validos:     78
  Parse err:   12
  Valid rej:   10
  Taxa OK:    78%
```

## Dicas

- `str.strip("#")` remove `#` do início e fim
- `str.split(";")` separa os campos
- `try/except ValueError` em volta de cada `float()` ou `int()`
- Norma da aceleração: `sqrt(ax² + ay² + az²)`

## Extra (opcional)

- Adicione checksum: o último campo é o XOR de todos os bytes entre `#...#`
- Exporte os pacotes válidos para um CSV
- Aceite o arquivo de entrada como argumento (`sys.argv`)

## Critérios de aceite

- [ ] Parser rejeita formatos inválidos sem quebrar
- [ ] Validador rejeita valores fora do range
- [ ] Estatísticas no final
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Por que separar parser e validador em funções diferentes?
- O que acontece se um campo de altitude vier como `"NaN"`?
- Como você testaria manualmente o parser com 20 linhas diferentes?

## Referências

- [Trilha Python](../trilhas/python.md)
- [Slide deck](../slides/semana-02.html)
- [Quiz](../quiz/semana-02.html)
