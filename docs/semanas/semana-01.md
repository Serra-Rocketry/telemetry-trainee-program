# Semana 1 - Python básico (Google Colab)

## Objetivo

Analisar o CSV de telemetria de um voo no Google Colab, usando `pandas`, e gerar gráficos de aceleração, velocidade e altitude.

## Preparação

- [ ] Ter uma conta Google
- [ ] Abrir o [Google Colab](https://colab.research.google.com/) e entrar com a conta Google
- [ ] Baixar o arquivo de dados: `entregas/dados/semana-01/cleaned.csv`
- [ ] Ler a [Trilha Python](../trilhas/python.md) (seções 1.1 e 2.1)

## Passo a passo no Colab

1. Acesse **[https://colab.research.google.com/](https://colab.research.google.com/)** e entre com a sua conta Google.
2. Clique em **Novo notebook**.
3. Renomeie o notebook para **`semana-01`** (clique no título no topo).
4. Importe o arquivo `cleaned.csv` para o Colab:
   - Clique no ícone de **pasta** (arquivos) na barra lateral → **Upload** → selecione o `cleaned.csv` (baixado do repositório), **ou**
   - Rode em uma célula:
     ```python
     from google.colab import files
     files.upload()  # selecione o cleaned.csv
     ```
5. Faça a análise (pandas + gráficos) nas células abaixo.
6. No final, clique em **Compartilhar** → **Qualquer pessoa com o link** → **Copiar link**.
7. Cole o link no arquivo `entregas/<seu-nome-sobrenome>/semana-01/README.md` (veja a entrega).

> O Colab já vem com `pandas` e `matplotlib` instalados - não precisa instalar nada.

## Dataset

O arquivo `cleaned.csv` **não tem cabeçalho** e cada linha começa com o prefixo `#51` (o `ID` do pacote). São 24 campos separados por vírgula, nesta ordem:

| # | campo | descrição |
|---|---|---|
| 0 | `ID` | identificador do pacote (`#51`) |
| 1 | `millis` | timestamp em milissegundos |
| 2 | `pckt` | contador de pacotes |
| 3 | `alt` | altitude (m) |
| 4 | `temp` | temperatura (°C) |
| 5 | `humi` | umidade |
| 6 | `press` | pressão (hPa) |
| 7 | `gx` | giroscópio X |
| 8 | `gy` | giroscópio Y |
| 9 | `gz` | giroscópio Z |
| 10 | `ax` | acelerômetro X |
| 11 | `ay` | acelerômetro Y |
| 12 | `az` | acelerômetro Z |
| 13 | `vz` | velocidade vertical |
| 14 | `maxalt` | altitude máxima atingida |
| 15 | `state` | estado do voo |
| 16 | `hora` | hora |
| 17 | `data` | data |
| 18 | `gps_alt` | altitude do GPS |
| 19 | `lat` | latitude |
| 20 | `lon` | longitude |
| 21 | `sat` | satélites no fix do GPS |
| 22 | `pqd` | qualidade do pacote |
| 23 | `rssi` | potência do sinal recebido (dBm) |

Exemplo de linha:

```
#51,2003976,34647,39.53,42.34,0.00,957.29,1.87,0.26,-0.34,-39.20,-0.82,2.53,0.00,39.53,2,0,2000,487.80,-21.942293,-48.951912,12,0,-121
```

## Tarefas

1. **Ler e tratar com pandas**: carregue o `cleaned.csv` com `pd.read_csv("cleaned.csv", header=None, names=COLUNAS)`, remova o `#` da coluna `ID` e garanta que os campos numéricos estejam como número.
2. **Explorar os dados**: mostre as primeiras linhas (`df.head()`), os tipos (`df.dtypes`) e um resumo (`df.describe()`).
3. **Estatísticas de `alt`**: média, máximo, mínimo e número de amostras.
4. **Gráfico de aceleração × tempo**: plote `ax`, `ay` e `az` contra o tempo (`millis`) no mesmo gráfico.
5. **Gráfico de velocidade e altitude**: plote `vz` (velocidade vertical) e `alt` (altitude) ao longo do tempo.
6. **Compartilhar**: gere o link público do notebook e coloque no README da semana.

## Entrega

- O link do notebook do Colab, no formato `https://colab.research.google.com/drive/...`, **público** ("Qualquer pessoa com o link pode visualizar").
- Cole o link em `entregas/<seu-nome-sobrenome>/semana-01/README.md`, junto com uma frase do que você fez.
- (Opcional) Baixe o notebook (`Arquivo → Fazer download → .ipynb`) e salve também na pasta da semana.

## Dicas

- `pd.read_csv("cleaned.csv", header=None, names=COLUNAS)` lê um CSV sem cabeçalho
- Remova o `#` do ID com `df["ID"].str.lstrip("#")`
- Converta colunas com `pd.to_numeric(df[campo], errors="coerce")`
- Crie uma coluna de tempo em segundos: `df["tempo_s"] = (df["millis"] - df["millis"].min()) / 1000`
- Gráficos com pandas: `df.plot(x="tempo_s", y=["ax", "ay", "az"])`
- Todos os passos em uma célula do Colab aparecem no notebook - comente o que cada parte faz

## Extra (opcional)

- Calcule a norma da aceleração: `(df["ax"]**2 + df["ay"]**2 + df["az"]**2) ** 0.5`
- Adicione um gráfico de `temp` e `press`
- Marque no gráfico de altitude o instante de `maxalt`

## Critérios de aceite

- [ ] Notebook `semana-01` criado no Colab
- [ ] CSV carregado e tratado com pandas
- [ ] Gráfico de aceleração (`ax`, `ay`, `az`) ao longo do tempo
- [ ] Gráfico de velocidade (`vz`) e altitude (`alt`)
- [ ] Link do Colab público colado no `README.md` da semana

## Perguntas para reflexão

- O que acontece se uma linha tiver um campo não numérico?
- Por que ler com `header=None`? O que muda se você não passar os nomes das colunas?
- O que a aceleração e a velocidade dizem sobre a fase do voo?

## Referências

- [Google Colab](https://colab.research.google.com/)
- [Exemplo Python (Colab)](../examples/python-exemplo.md)
- [Trilha Python](../trilhas/python.md)
- [Slide deck](../slides/semana-01.html)
- [Quiz](../quiz/semana-01.html)
