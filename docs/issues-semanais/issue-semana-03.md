# Semana 3 - Arduino IDE

## Objetivo

Ler um BMP280 e enviar pacotes formatados pela serial (no hardware ou no Wokwi).

## Tarefas

- [ ] Blink no GPIO 1 (1 Hz)
- [ ] Scan I2C e leitura do BMP280
- [ ] Envio de pacote a 20 Hz no formato `#timestamp_ms;alt;temp;pressao#`
- [ ] (Sem hardware) montar e simular tudo no [Wokwi](https://wokwi.com/)

## Criterios de aceite

- [ ] Upload/simulacao sem erros
- [ ] Serial monitor exibe pacotes a 20 Hz
- [ ] LED piscando a 1 Hz independente
- [ ] README com esquema de ligacao e (se Wokwi) o link do projeto

## Referencias

- [Wokwi - simulador online](https://wokwi.com/)
- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Exemplo Arduino IDE](../examples/arduino-ide-exemplo.md)
