# Exemplo — Python: Leitura e Análise de Telemetria

## Objetivo

Ler um CSV de telemetria, calcular estatísticas e gerar um gráfico.

## Código completo

```python
#!/usr/bin/env python3
"""Leitor de telemetria — calcula estatísticas e gera gráfico."""

import csv
import sys
from pathlib import Path


def ler_csv(caminho: str) -> list[dict]:
    """Lê CSV e retorna lista de dicionários."""
    with open(caminho) as f:
        return list(csv.DictReader(f))


def calcular_estatisticas(dados: list[dict], campo: str) -> dict:
    """Calcula média, max e min de um campo numérico."""
    valores = [float(linha[campo]) for linha in dados]
    return {
        "campo": campo,
        "media": sum(valores) / len(valores),
        "max": max(valores),
        "min": min(valores),
        "amostras": len(valores),
    }


def salvar_resumo(estats: list[dict], caminho: str = "resumo.txt"):
    """Salva estatísticas em arquivo."""
    with open(caminho, "w") as f:
        for e in estats:
            f.write(
                f"{e['campo']}: media={e['media']:.2f}, "
                f"max={e['max']:.2f}, min={e['min']:.2f}, "
                f"amostras={e['amostras']}\n"
            )
    print(f"Resumo salvo em {caminho}")


def main():
    if len(sys.argv) < 2:
        print("Uso: python exemplo.py <arquivo.csv>")
        sys.exit(1)

    caminho = sys.argv[1]
    if not Path(caminho).exists():
        print(f"Erro: arquivo {caminho} não encontrado")
        sys.exit(1)

    dados = ler_csv(caminho)
    print(f"Linhas lidas: {len(dados)}")

    campos = ["altitude_m", "temperatura_C", "pressao_hPa"]
    estats = [calcular_estatisticas(dados, c) for c in campos]

    for e in estats:
        print(f"{e['campo']}: média={e['media']:.2f}, "
              f"max={e['max']:.2f}, min={e['min']:.2f}")

    salvar_resumo(estats)

    # Gráfico (opcional)
    try:
        import matplotlib.pyplot as plt

        times = [float(l["timestamp_ms"]) / 1000 for l in dados]
        alts = [float(l["altitude_m"]) for l in dados]

        plt.plot(times, alts)
        plt.xlabel("Tempo (s)")
        plt.ylabel("Altitude (m)")
        plt.title("Perfil de Voo")
        plt.grid(True)
        plt.savefig("perfil_voo.png")
        print("Gráfico salvo em perfil_voo.png")
    except ImportError:
        print(" matplotlib não instalado — gráfico ignorado")


if __name__ == "__main__":
    main()
```

## Como testar

```bash
# 1. Salve o código acima como exemplo.py
# 2. Crie um CSV de teste:

cat > dados.csv << 'EOF'
timestamp_ms,altitude_m,temperatura_C,pressao_hPa,ax,ay,az
0,0.0,25.0,1013.25,0.01,0.02,9.81
50,0.5,24.9,1012.80,0.02,0.01,9.80
100,1.2,24.8,1012.30,0.03,0.01,9.79
150,2.0,24.7,1011.75,0.02,0.02,9.81
200,3.1,24.6,1011.20,0.01,0.01,9.80
EOF

# 3. Execute:
python exemplo.py dados.csv

# 4. Saída esperada:
# Linhas lidas: 5
# altitude_m: média=1.36, max=3.10, min=0.00
# temperatura_C: média=24.80, max=25.00, min=24.60
# pressao_hPa: média=1012.26, max=1013.25, min=1011.20
# Resumo salvo em resumo.txt
```
