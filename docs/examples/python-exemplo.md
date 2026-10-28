# Exemplo - Colab: Análise de Telemetria (Semana 1)

## Objetivo

Mostrar, célula por célula, como analisar o `cleaned.csv` no Google Colab com **pandas** e gerar os gráficos da Semana 1.

## Como usar no Colab

1. Abra [colab.research.google.com](https://colab.research.google.com/) e crie um **Novo notebook**.
2. Renomeie para `semana-01`.
3. Copie cada célula abaixo na ordem.

## Célula 1 - Importar e subir o arquivo

```python
import pandas as pd
import matplotlib.pyplot as plt

from google.colab import files

COLUNAS = [
    "ID", "millis", "pckt", "alt", "temp", "humi", "press",
    "gx", "gy", "gz", "ax", "ay", "az", "vz", "maxalt", "state",
    "hora", "data", "gps_alt", "lat", "lon", "sat", "pqd", "rssi",
]

# Abre o seletor de arquivos - escolha o cleaned.csv
enviados = files.upload()
```

## Célula 2 - Ler e tratar com pandas

```python
# Lê o CSV sem cabeçalho e aplica os nomes das colunas
df = pd.read_csv("cleaned.csv", header=None, names=COLUNAS)

# Remove o '#' do ID (ex.: '#51' -> 51)
df["ID"] = df["ID"].astype(str).str.lstrip("#").astype(int)

# Garante que os campos numéricos estejam como número
for coluna in COLUNAS[1:]:
    df[coluna] = pd.to_numeric(df[coluna], errors="coerce")

# Cria uma coluna de tempo em segundos
df["tempo_s"] = (df["millis"] - df["millis"].min()) / 1000

df.head()
```

## Célula 3 - Explorar os dados

```python
print("Formato:", df.shape)
df.dtypes
```

```python
df.describe()
```

## Célula 4 - Estatísticas de altitude

```python
alt = df["alt"]
print(f"Amostras:        {alt.count()}")
print(f"Altitude media:  {alt.mean():.2f} m")
print(f"Altitude max:    {alt.max():.2f} m")
print(f"Altitude min:    {alt.min():.2f} m")
print(f"Duracao:         {df['tempo_s'].max():.2f} s")
```

Saída esperada:

```
Amostras:        38
Altitude media:  257.57 m
Altitude max:    359.42 m
Altitude min:    39.53 m
Duracao:         15.98 s
```

## Célula 5 - Gráfico de aceleração × tempo

```python
plt.figure(figsize=(10, 5))
plt.plot(df["tempo_s"], df["ax"], label="ax")
plt.plot(df["tempo_s"], df["ay"], label="ay")
plt.plot(df["tempo_s"], df["az"], label="az")
plt.title("Aceleração x Tempo")
plt.xlabel("Tempo (s)")
plt.ylabel("Aceleração")
plt.grid(True, alpha=0.3)
plt.legend()
plt.show()
```

## Célula 6 - Gráfico de velocidade e altitude

```python
fig, ax1 = plt.subplots(figsize=(10, 5))

ax1.plot(df["tempo_s"], df["alt"], color="#ff6b35", label="Altitude (m)")
ax1.set_xlabel("Tempo (s)")
ax1.set_ylabel("Altitude (m)", color="#ff6b35")

ax2 = ax1.twinx()
ax2.plot(df["tempo_s"], df["vz"], color="#2ea043", label="Velocidade (vz)")
ax2.set_ylabel("Velocidade (vz)", color="#2ea043")

ax1.grid(True, alpha=0.3)
plt.title("Altitude e Velocidade x Tempo")
plt.show()
```

## Compartilhar

No Colab: **Compartilhar** → **Qualquer pessoa com o link** → **Copiar link**.
Cole o link em `entregas/<seu-nome-sobrenome>/semana-01/README.md`.

## Notas

- O Colab já vem com `pandas` e `matplotlib` - não precisa instalar.
- Se preferir subir o arquivo pela barra lateral: ícone de pasta → **Upload** → selecione `cleaned.csv` (funciona igual).
