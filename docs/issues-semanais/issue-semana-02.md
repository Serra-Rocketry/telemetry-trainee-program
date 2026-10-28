# Semana 2 - Python aplicado (pacotes)

## Objetivo

Limpar o log cru da base (`entregas/dados/semana-02/voo_thonyan.txt`) e gerar um CSV tratado, pronto para o Colab.

## Contexto

O computador da base recebe a telemetria crua e precisa limpa-la para plotar em tempo real. O parser e o primeiro estagio desse pipeline: log cru -> dados limpos (CSV) -> graficos.

## Tarefas

- [ ] Implementar parser (24 campos, ignorar linhas que nao sao pacote)
- [ ] Implementar validador (ranges de alt, temp, press, lat, lon, sat, rssi)
- [ ] Gerar `dados_tratados.csv` com cabecalho e apenas pacotes validos
- [ ] Abrir o CSV no Colab e gerar os graficos da Semana 1
- [ ] Contar pacotes perdidos a partir das linhas `[LOST]` (opcional)

## Criterios de aceite

- [ ] Parser rejeita os pacotes de 22 campos (`[LoRa] RX`) sem quebrar
- [ ] Parser ignora `[STATS]`, `[MESH]` e `[LOST]`
- [ ] `dados_tratados.csv` gerado com cabecalho
- [ ] CSV roda no Colab com `pd.read_csv("dados_tratados.csv")`
- [ ] README atualizado

## Referencias

- [Google Colab](https://colab.research.google.com/)
- [Trilha Python](../trilhas/python.md)
- [Exemplo Python](../examples/python-exemplo.md)
