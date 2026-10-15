import csv
import matplotlib.pyplot as plt
import math # Necessário para o Extra (raiz quadrada do desvio padrão)

#Fazer a conversão dos dados
def ler_dados(caminho):
    dados_convertidos = []
    with open(caminho, mode='r') as arquivo:
        leitor = csv.DictReader(arquivo)
        for linha in leitor:
            dados_convertidos.append({
                "timestamp": int(linha["timestamp"]),
                "altitude": float(linha["altitude"]),
                "temp": float(linha["temp"]),
                "pressao": float(linha["pressao"]),
                "voltage": float(linha["voltage"])
            })
    return dados_convertidos


def calcular_estatisticas(dados, campo):
    valores = [linha[campo] for linha in dados]
    n = len(valores)
    
    media = sum(valores) / n
    maximo = max(valores)
    minimo = min(valores)
    
    
    variancia = sum((x - media) ** 2 for x in valores) / n
    desvio_padrao = math.sqrt(variancia)

    return {
        "media": media,
        "maximo": maximo,
        "minimo": minimo,
        "desvio_padrao": desvio_padrao,
        "amostras": n
    }

#para salvar em txt
def salvar_resumo(stats, nome_arquivo="resumo.txt"):
    with open(nome_arquivo, "w") as f:
        f.write("=== RESUMO DE TELEMETRIA ===\n\n")
        for campo, valores in stats.items():
            f.write(f"{campo.upper()}:\n")
            f.write(f"  Media:         {valores['media']:.2f}\n")
            f.write(f"  Maximo:        {valores['maximo']:.2f}\n")
            f.write(f"  Minimo:        {valores['minimo']:.2f}\n")
            f.write(f"  Desvio Padrao: {valores['desvio_padrao']:.2f}\n") # Incluindo o Extra
            f.write(f"  Amostras:      {valores['amostras']}\n\n")
    print(f"-> Ficheiro {nome_arquivo} criado com sucesso!")

#criar o grafico
def gerar_graficos(dados):
    tempo = [linha["timestamp"] for linha in dados]
    altitudes = [linha["altitude"] for linha in dados]
    temperaturas = [linha["temp"] for linha in dados]


    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

 
    ax1.plot(tempo, altitudes, color="blue", linewidth=2, marker='o')
    ax1.set_title("Evolução da Altitude")
    ax1.set_ylabel("Altitude (m)")
    ax1.grid(True, alpha=0.3)

    ax2.plot(tempo, temperaturas, color="red", linewidth=2, marker='x')
    ax2.set_title("Evolução da Temperatura")
    ax2.set_xlabel("Tempo (ms)")
    ax2.set_ylabel("Temperatura (°C)")
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig("grafico_voo.png", dpi=150)
    print("-> Ficheiro grafico_voo.png gerado com sucesso!")


def main():
    print("A processar telemetria...")
    

    dados = ler_dados("telemetria.csv")
    
    
    estatisticas_gerais = {}
    for grandeza in ["altitude", "temp", "pressao", "voltage"]:
        estatisticas_gerais[grandeza] = calcular_estatisticas(dados, grandeza)
        salvar_resumo(estatisticas_gerais)
    

    gerar_graficos(dados)

if __name__ == "__main__":
    main()