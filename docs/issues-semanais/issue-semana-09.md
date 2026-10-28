# Semana 9 - Chips personalizados no Wokwi

## Objetivo

Montar o computador de bordo simulado no Wokwi, adicionando os chips personalizados (BMP585, LSM6DS3 e NEO-8M) e gerando telemetria para analisar no Colab.

## Tarefas

- [ ] Criar projeto ESP32-S3 no Wokwi e adicionar os três custom chips do zero
- [ ] Firmware lendo BMP585 (I2C 0x7E), LSM6DS3 (I2C 0x6B) e NEO-8M (UART 9600)
- [ ] Enviar pacotes a 10 Hz no formato `#51,millis,pckt,alt,temp,press,gx,gy,gz,ax,ay,az,gps_alt,lat,lon,sat`
- [ ] Salvar `log_wokwi.txt` com 5+ minutos de simulação
- [ ] Adaptar o parser da Semana 2 e gerar `dados_tratados.csv`
- [ ] Gerar os gráficos no Colab a partir do CSV tratado

## Criterios de aceite

- [ ] Projeto no Wokwi montado do zero com os três chips (link compartilhado)
- [ ] Chips Console mostra os três sensores prontos
- [ ] Pacotes a 10 Hz no formato de 16 campos
- [ ] Log com 5+ minutos de simulação
- [ ] `dados_tratados.csv` com cabeçalho e apenas pacotes válidos
- [ ] README com links do Wokwi e do Colab

## Referencias

- [Roteiro da Semana 9](../semanas/semana-09.md)
- [Chips personalizados do Wokwi](../../entregas/dados/wokwi-chips/README.md)
- [Trilha Sensores e Protocolos](../trilhas/sensors.md)
- [Wokwi](https://wokwi.com/)
