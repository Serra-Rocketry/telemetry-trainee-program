# Semana 3 — Arduino IDE

## Objetivo

Programar um ESP32-C3 para ler sensor BMP280 e enviar dados formatados pela serial.

## Preparação

- [ ] Arduino IDE 2.x instalado com suporte a ESP32
- [ ] ESP32-C3 Super Mini + cabo USB
- [ ] BMP280 + protoboard + jumpers
- [ ] Ler a [Trilha Arduino IDE](../trilhas/arduino-ide.md) (seções 1, 3, 4)
- [ ] Ver o [Slide deck](../slides/semana-03.html)
- [ ] Fazer o [Quiz](../quiz/semana-03.html)

## Problema

### Tarefa 1 — Blink

Faça um LED (GPIO 1) piscar a 1 Hz. Use `pinMode()` e `digitalWrite()`.

**Confira**: o LED acende e apaga numa frequência visível.

### Tarefa 2 — Scan I2C

Conecte o BMP280 ao ESP32-C3 e faça um scan I2C para descobrir o endereço.

**Conexões:**
```
BMP280 → ESP32-C3
VCC    → 3.3V
GND    → GND
SDA    → GPIO4
SCL    → GPIO5
```

Use `Wire.begin(4, 5)` e `Wire.endTransmission()` para varrer endereços de 1 a 127.

**Saída esperada:** `Dispositivo em 0x76` (ou `0x77`)

### Tarefa 3 — Leitura do sensor

Com a biblioteca Adafruit BMP280, leia altitude, temperatura e pressão. Envie os valores pela serial a cada 1 segundo.

### Tarefa 4 — Pacotes formatados (20 Hz)

Adapte o código para enviar pacotes a **20 Hz** no formato:

```
#timestamp_ms;altitude_m;temperatura_C;pressao_hPa#
```

**Regras:**
- Use `millis()` para controle de tempo — `delay()` não é aceito
- A taxa deve ser consistente (20 Hz ± 1 Hz)
- O LED deve piscar independente (1 Hz) — não pode travar junto com o sensor

## Dicas

- `Wire.begin(sda, scl)` no `setup()` define os pinos I2C no ESP32-C3
- A biblioteca BMP280 precisa de `#include <Adafruit_BMP280.h>` e `#include <Wire.h>`
- Para leitura de altitude: `bmp.readAltitude(1013.25)` — o parâmetro é a pressão ao nível do mar
- `sprintf()` ou `snprintf()` ajuda a formatar o pacote
- Estrutura típica sem delay:
  ```cpp
  if (millis() - ultimo >= INTERVALO) {
      ultimo = millis();
      // leitura e envio
  }
  ```

## Extra (opcional)

- Adicione validação: só envie pacote se o sensor respondeu corretamente
- Adicione um botão no GPIO 2 com debounce que alterna o LED
- Calcule a norma da aceleração (`sqrt(ax²+ay²+az²)`) — mesmo que os dados sejam simulados

## Critérios de aceite

- [ ] Código compila e faz upload sem erros
- [ ] Serial envia pacotes a 20 Hz constante
- [ ] LED piscando a 1 Hz independente
- [ ] Formato do pacote: `#timestamp;alt;temp;pres#`
- [ ] README com esquema de ligação dos pinos
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Por que `delay()` atrapalha a taxa de amostragem?
- O que acontece com o barramento I2C se o SDA ou SCL não tiver pull-up?
- Qual a diferença entre 9600 e 115200 baud? Quando usar cada um?

## Referências

- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Slide deck](../slides/semana-03.html)
- [Quiz](../quiz/semana-03.html)
