# Semana 2 - Python aplicado (pacotes)

## Objetivo

Implementar o parser que limpa a telemetria recebida e gerar, no final, um **CSV tratado** pronto para ser analisado no Colab.

## Contexto

Durante o voo, o foguete envia os pacotes por rádio (LoRa) e o **computador da base** recebe tudo como um **log cru** - misturado com mensagens do receptor (`[LoRa] RX`, `[STATS]`, `[MESH]`, `[LOST]`), com alguns pacotes incompletos e com o formato como veio do ar.

Esse computador de base precisa **pegar esses dados e limpar** o mais rápido possível, porque o objetivo é **plotar a telemetria em tempo real** durante o voo (altitude, velocidade, aceleração...). Se ele plotar lixo, a leitura é inútil.

O parser que você vai escrever é o **primeiro estágio desse pipeline**:

```
log cru da base  ->  [parser + validador]  ->  dados limpos (CSV)  ->  gráficos em tempo real
```

Ou seja: transformar o log cru em dados limpos, tipados e confiáveis. No fim, você vai gerar um arquivo CSV tratado que pode ser aberto direto no Colab (como na Semana 1).

## Preparação

- [ ] Semana 1 concluída
- [ ] Ler a [Trilha Python](../trilhas/python.md) (seções 1.2 e 1.3)
- [ ] Ver o [Slide deck](../slides/semana-02.html)
- [ ] Fazer o [Quiz](../quiz/semana-02.html)

## Dataset

O arquivo `entregas/dados/semana-02/voo_thonyan.txt` é o **log serial cru** de uma sessão de recepção. Ele mistura vários tipos de linha:

```
[LoRa] RX (RSSI=-121): #51,2003976,34647,39.53,...    # pacote recebido via LoRa
[STATS] Primeiro pacote recebido — count=34647        # estatística do receptor
[MESH] Beacons ativos: 0/0 | ouvindo o satellite: 0  # status da malha
[LOST] 22 pacote(s) perdido(s) — count 34647 -> 34670 # pacotes perdidos
#51,2003976,34647,39.53,42.34,...                     # pacote completo (24 campos)
```

O pacote tem os **mesmos 24 campos da Semana 1**, separados por vírgula e começando com `#51`:

`ID, millis, pckt, alt, temp, humi, press, gx, gy, gz, ax, ay, az, vz, maxalt, state, hora, data, gps_alt, lat, lon, sat, pqd, rssi`

> Atenção às linhas `[LoRa] RX`: elas trazem o pacote **sem os campos `hora` e `data`** (22 campos) e com o `rssi` zerado (o valor real está no texto `RSSI=...`). Elas têm formato diferente das linhas `#51,` completas.

## Problema

Seu programa deve:

### Tarefa 1 - Parser

Função que recebe uma linha bruta e retorna um dicionário com os campos tipados.

Regras do parser:
- Aceitar apenas linhas que contenham um pacote que começa com `#51,` e tenha **24 campos**
- Retornar `None` quando a linha não for um pacote completo (`[STATS]`, `[MESH]`, `[LOST]`, vazias ou `[LoRa] RX` de 22 campos)
- Remover o `#` do campo `ID` (vira `51`)
- Rejeitar campos não numéricos (ex: letra onde deveria vir número)
- Retornar `None` para linhas inválidas (não levantar exceção)

### Tarefa 2 - Validador

Função que recebe o dicionário do parser e valida os ranges físicos:

| Campo | Range |
|---|---|
| `alt` (altitude) | -100 a 10000 m |
| `temp` | -40 a 85 °C |
| `press` | 300 a 1100 hPa |
| `lat` | -90 a 90 |
| `lon` | -180 a 180 |
| `sat` | 0 a 30 |
| `rssi` | -140 a 0 dBm |

Retorna `True`/`False`.

### Tarefa 3 - Limpar e salvar o CSV tratado

Processe o log inteiro com parser + validador e **gere um arquivo `dados_tratados.csv`** contendo **apenas os pacotes válidos**, com uma **linha de cabeçalho** com os nomes das colunas:

```
ID,millis,pckt,alt,temp,humi,press,gx,gy,gz,ax,ay,az,vz,maxalt,state,hora,data,gps_alt,lat,lon,sat,pqd,rssi
51,2003976,34647,39.53,42.34,...
```

No terminal, mostre um resumo:

```
Processando...

Resumo:
  Linhas lidas:     118
  Pacotes:           38
  Validos:           38
  Parse err:          0
  Valid rej:          0
  Taxa OK:          100%
  Ignoradas:         80   # [LoRa] RX, [STATS], [MESH], [LOST]
  Pacotes perdidos: 762   # soma dos [LOST]
CSV tratado salvo em dados_tratados.csv
```

### Tarefa 4 - Rodar no Colab

Carregue o `dados_tratados.csv` no Colab e gere os mesmos gráficos da Semana 1 (aceleração, velocidade e altitude):

```python
import pandas as pd
df = pd.read_csv("dados_tratados.csv")   # já tem cabeçalho, lê direto
df["tempo_s"] = (df["millis"] - df["millis"].min()) / 1000
df.plot(x="tempo_s", y=["ax", "ay", "az"])   # aceleração x tempo
```

## Requisitos

- Pode usar a biblioteca padrão (`csv`, `str.split`) ou `pandas`
- O CSV gerado deve ter cabeçalho (para abrir direto no Colab)
- Aceite o arquivo de entrada como argumento (`sys.argv`)
- Trate arquivo inexistente com mensagem de erro amigável

## Dicas

- Uma forma de extrair o pacote de uma linha `[LoRa] RX (...) : #51,...` é procurar a posição de `#51`
- `str.lstrip("#")` remove o `#` do ID
- `str.split(",")` separa os campos
- `try/except ValueError` em volta de cada `float()` ou `int()`
- Para escrever o CSV com cabeçalho, use `csv.DictWriter(f, fieldnames=COLUNAS)`
- Para contar pacotes perdidos, some o valor de cada linha `[LOST]`

## Extra (opcional)

- Marque no CSV uma coluna indicando se o pacote foi validado
- Compare o intervalo de `pckt` com o total de pacotes perdidos
- Reconstrua a série temporal e gere um gráfico com `matplotlib` já no seu script

## Critérios de aceite

- [ ] Parser rejeita a linha `[LoRa] RX` (22 campos) sem quebrar
- [ ] Parser ignora `[STATS]`, `[MESH]` e `[LOST]`
- [ ] Validador rejeita valores fora do range
- [ ] `dados_tratados.csv` gerado com cabeçalho e apenas pacotes válidos
- [ ] CSV abre no Colab e mostra os gráficos da Semana 1
- [ ] Commit com tipo semântico

## Entrega

- O script do parser (`.py`) ou o notebook, dentro de `entregas/<seu-nome-sobrenome>/semana-02/`
- O arquivo `dados_tratados.csv` gerado, na mesma pasta
- (Opcional) o link do Colab que usa o CSV tratado

## Perguntas para reflexão

- Por que separar parser e validador em funções diferentes?
- O que acontece se um campo de altitude vier como `"NaN"`?
- Por que plotar em tempo real exige que a limpeza aconteça o mais rápido possível?

## Referências

- [Google Colab](https://colab.research.google.com/)
- [Trilha Python](../trilhas/python.md)
- [Exemplo Python (Colab)](../examples/python-exemplo.md)
- [Slide deck](../slides/semana-02.html)
- [Quiz](../quiz/semana-02.html)
