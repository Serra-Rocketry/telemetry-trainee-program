# Trilha Python

Python é a linguagem padrão do setor para análise de dados, simulação e ferramentas auxiliares. Tanto o Flight Computer quanto o satélite Helike usam Python extensivamente.

## O que você vai aprender

- Ler e processar dados de telemetria (CSV, JSON, serial)
- Implementar parsers e validadores de pacotes
- Analisar séries temporais com numpy/scipy
- Gerar gráficos e relatórios
- Simular sistemas físicos

## Pré-requisitos

- Python 3.10+ instalado
- VS Code (ou editor equivalente)
- Conhecimento básico de terminal

---

## 1. Fundamentos para telemetria

### 1.1 Leitura de arquivos CSV

Dados de telemetria geralmente são registrados em CSV. É o formato usado tanto nos testes de bancada do Helike quanto nos logs de voo do Flight Computer.

```python
import csv

with open("telemetria.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        print(row["altitude_m"], row["temperatura_C"])
```

**Exemplo real** — O Flight Computer loga sensores em CSV com colunas como `timestamp_ms, altitude_m, ax, ay, az, pressao_hPa`.

### 1.2 Parsing de pacotes serial

Dados vindos da serial do Arduino/ESP32 costumam vir em formato textual delimitado:

```
# Formato típico: #2024.1;125.3;9.81;0.01;0.02#
```

```python
def parse_packet(line: str) -> dict | None:
    if not line.startswith("#") or not line.endswith("#"):
        return None
    parts = line.strip("#").split(";")
    if len(parts) != 5:
        return None
    return {
        "timestamp_ms": float(parts[0]),
        "altitude_m": float(parts[1]),
        "ax": float(parts[2]),
        "ay": float(parts[3]),
        "az": float(parts[4]),
    }
```

**No Helike** — Os testes de integração (v3) enviam pacotes de 15 campos pela serial para validação.

### 1.3 Validação de dados

Sensores podem falhar. Dados inválidos precisam ser rejeitados antes do processamento.

```python
def validate_sensor_data(data: dict) -> bool:
    altitude = data.get("altitude_m")
    if altitude is None or altitude < -100 or altitude > 10000:
        return False
    ax, ay, az = data.get("ax", 0), data.get("ay", 0), data.get("az", 0)
    g = (ax**2 + ay**2 + az**2) ** 0.5
    if g < 8.0 or g > 11.0:
        return False
    return True
```

Verifique também por NaN, valores negativos impossíveis e range físico de cada sensor.

---

## 2. Análise de séries temporais

### 2.1 Estatísticas básicas

```python
import csv

altitudes = []
with open("telemetria.csv") as f:
    for row in csv.DictReader(f):
        altitudes.append(float(row["altitude_m"]))

print(f"Media: {sum(altitudes)/len(altitudes):.2f} m")
print(f"Max:   {max(altitudes):.2f} m")
print(f"Min:   {min(altitudes):.2f} m")
```

### 2.2 Detecção de apogeu

O Flight Computer detecta apogeu (ponto mais alto do voo) para disparar o paraquedas. É uma lógica simples que você pode implementar:

```python
def detect_apogee(altitudes: list, window: int = 3) -> int:
    for i in range(window, len(altitudes) - window):
        before = altitudes[i - window : i]
        after = altitudes[i : i + window]
        if max(before) < altitudes[i] > max(after):
            return i
    return -1
```

**No Flight Computer** — A detecção real usa velocidade vertical (Vz) com filtro EMA, mas a lógica conceitual é a mesma.

### 2.3 Filtro EMA (Exponential Moving Average)

Usado no Helike para suavizar leituras de altitude e calcular Vz:

```python
def ema_filter(values: list, alpha: float = 0.3) -> list:
    result = [values[0]]
    for v in values[1:]:
        result.append(alpha * v + (1 - alpha) * result[-1])
    return result
```

---

## 3. Visualização com matplotlib

```python
import matplotlib.pyplot as plt
import csv

times, alts = [], []
with open("telemetria.csv") as f:
    for row in csv.DictReader(f):
        times.append(float(row["timestamp_ms"]) / 1000)
        alts.append(float(row["altitude_m"]))

plt.plot(times, alts)
plt.xlabel("Tempo (s)")
plt.ylabel("Altitude (m)")
plt.title("Perfil de Voo")
plt.grid(True)
plt.savefig("perfil_voo.png")
```

**No Helike** — A simulação da asa SRAB gera gráficos de altitude, velocidade e ângulo de cone automaticamente.

---

## 4. Numpy e Scipy (para processamento avançado)

```python
import numpy as np
from scipy import signal

# Carregar dados
data = np.genfromtxt("telemetria.csv", delimiter=",", skip_header=1)
altitudes = data[:, 1]

# Filtro passa-baixa
b, a = signal.butter(4, 0.1)
altitudes_filtradas = signal.filtfilt(b, a, altitudes)

# Estatisticas
print(f"Media:  {np.mean(altitudes):.2f}")
print(f"Std:    {np.std(altitudes):.2f}")
print(f"Max:    {np.max(altitudes):.2f}")
```

**No Helike** — A simulação da asa SRAB usa scipy para integrar EDOs (solve_ivp) e otimizar parâmetros.

---

## 5. Exercícios práticos

### Nível 1 — Leitura e estatísticas

1. Dado um CSV de telemetria (timestamp, altitude, ax, ay, az), calcule:
   - Altitude máxima, mínima e média
   - Número total de amostras
   - Duração total do registro em segundos

2. Salve o resumo em um arquivo `.txt`.

### Nível 2 — Parser e validação

1. Crie um parser que receba linhas no formato `#t;alt;ax;ay;az#` e retorne um dicionário.
2. Adicione validação: rejeite valores fora do range físico.
3. Conte e exiba estatísticas de pacotes válidos vs inválidos.

### Nível 3 — Análise de série temporal

1. Detecte o apogeu em uma série de altitude.
2. Aplique um filtro EMA e compare o resultado com os dados brutos (gráfico sobreposto).
3. Calcule a velocidade vertical (Vz) por diferença finita.

### Nível 4 — Conexão com projetos reais

1. Leia um arquivo CSV gerado pelo Flight Computer (Helike ou FC) e reproduza o perfil de voo.
2. Identifique as fases do voo (ascensão, queda, pouso) com base na altitude e Vz.

---

## Conexão com os projetos reais

| Conceito | Projeto | Onde é usado |
|---|---|---|
| Leitura de CSV | Flight Computer | Logs de voo pós-missão |
| Parsing serial | Helike / FC | Validação de dados em testes de bancada |
| Filtro EMA | Helike | Cálculo de Vz no firmware |
| Detecção de apogeu | Flight Computer | Disparo do paraquedas |
| numpy/scipy | Helike | Simulação aerodinâmica da asa SRAB |
| matplotlib | Ambos | Relatórios e análises pós-voo |

## Entregas relacionadas

- [Semana 1](../semanas/semana-01.md): Leitura de CSV + estatísticas
- [Semana 2](../semanas/semana-02.md): Parser + validação
- [Semana 5](../semanas/semana-05.md): Integração Arduino ↔ Python

## Critérios de aceite

- Código executa sem erros
- Output legível e documentado
- README com instruções de execução
- Código no repositório pessoal (branch `nome-sobrenome`)
