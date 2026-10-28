# Semana 3 - Arduino IDE

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

### Tarefa 1 - Blink

Faça um LED (GPIO 1) piscar a 1 Hz. Use `pinMode()` e `digitalWrite()`.

Confira: o LED acende e apaga numa frequência visível.

### Tarefa 2 - Scan I2C

Conecte o BMP280 ao ESP32-C3 e faça um scan I2C para descobrir o endereço.

Conexões:
```
BMP280 → ESP32-C3
VCC    → 3.3V
GND    → GND
SDA    → GPIO4
SCL    → GPIO5
```

Use `Wire.begin(4, 5)` e `Wire.endTransmission()` para varrer endereços de 1 a 127.

Saída esperada: `Dispositivo em 0x76` (ou `0x77`)

### Tarefa 3 - Leitura do sensor

Com a biblioteca Adafruit BMP280, leia altitude, temperatura e pressão. Envie os valores pela serial a cada 1 segundo.

### Tarefa 4 - Pacotes formatados (20 Hz)

Adapte o código para enviar pacotes a 20 Hz no formato:

```
#timestamp_ms;altitude_m;temperatura_C;pressao_hPa#
```

Regras:
- Use `millis()` para controle de tempo - `delay()` não é aceito
- A taxa deve ser consistente (20 Hz ± 1 Hz)
- O LED deve piscar independente (1 Hz) - não pode travar junto com o sensor

## Fazendo no Wokwi (sem hardware)

Se você ainda não tem o ESP32-C3 e o BMP280 físicos, dá para fazer **todas as tarefas desta semana no [Wokwi](https://wokwi.com/)** - um simulador de Arduino/ESP32 que roda no navegador. O Wokwi já tem o BMP280 pronto (endereço **0x76**).

### Passo 1 - Criar o projeto

1. Acesse **[https://wokwi.com/](https://wokwi.com/)** e clique em **Start New Project** (faça login com GitHub/Google para salvar o projeto).
2. Escolha a placa **ESP32-C3** (procure por *"ESP32-C3"*).

### Passo 2 - Adicionar os componentes

Clique no botão **"+"** (barra roxa) e adicione:

- **LED** (`wokwi-led`)
- **Resistor** (`wokwi-resistor`) - use 220 Ω
- **BMP280** (`wokwi-bmp280`)

### Passo 3 - Ligar os fios

Clique em um pino e depois no outro para criar cada fio (a tabela do README deve refletir isto).

Tarefa 1 - blink:
```
LED (ânodo)  -> Resistor -> ESP32-C3 GPIO 1
LED (cátodo) -> GND
```

Tarefas 2 a 4 - sensor:
```
BMP280 -> ESP32-C3
VCC    -> 3V3
GND    -> GND
SDA    -> GPIO 4
SCL    -> GPIO 5
```

### Passo 4 - Instalar a biblioteca

Crie um arquivo **`libraries.txt`** (na lista de arquivos do projeto) com o conteúdo:

```
Adafruit BMP280 Library
Adafruit Unified Sensor
```

O Wokwi instala essas bibliotecas automaticamente ao iniciar a simulação.

### Passo 5 - Colar o código

Substitua o conteúdo de `sketch.ino` pelo código abaixo (resolve as Tarefas 1 a 4):

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define LED_PIN 1
#define INTERVALO_MS 50        // 20 Hz

Adafruit_BMP280 bmp;

unsigned long ultimo = 0;
unsigned long ultimoBlink = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(4, 5);            // SDA=4, SCL=5
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 nao encontrado!");
    while (1) delay(10);
  }
}

void loop() {
  unsigned long agora = millis();

  // LED pisca a 1 Hz, independente do sensor
  if (agora - ultimoBlink >= 500) {
    ultimoBlink = agora;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }

  // Pacote a 20 Hz
  if (agora - ultimo >= INTERVALO_MS) {
    ultimo = agora;

    float altitude = bmp.readAltitude(1013.25);   // m
    float temp = bmp.readTemperature();           // C
    float pressao = bmp.readPressure() / 100.0;   // Pa -> hPa

    char pacote[80];
    snprintf(pacote, sizeof(pacote), "#%lu;%.2f;%.2f;%.2f#",
             agora, altitude, temp, pressao);
    Serial.println(pacote);
  }
}
```

### Passo 6 - Simular

1. Clique no **▶ Start Simulation**.
2. Abra o **Serial Monitor** (parte de baixo) e confira o baud em **115200**.
3. Você deve ver um pacote a cada ~50 ms, no formato `#timestamp;alt;temp;pressao#`, e o LED piscando a 1 Hz.

### Passo 7 - Entregar

- Clique em **Save** e depois em **Share** para copiar o link do projeto (ex.: `https://wokwi.com/projects/...`).
- Cole o link no `README.md` da sua pasta da semana, junto com a tabela de ligação dos pinos.

## Dicas

- `Wire.begin(sda, scl)` no `setup()` define os pinos I2C no ESP32-C3
- A biblioteca BMP280 precisa de `#include <Adafruit_BMP280.h>` e `#include <Wire.h>`
- Para leitura de altitude: `bmp.readAltitude(1013.25)` - o parâmetro é a pressão ao nível do mar
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
- Calcule a norma da aceleração (`sqrt(ax²+ay²+az²)`) - mesmo que os dados sejam simulados

## Critérios de aceite

- [ ] Código compila e faz upload sem erros
- [ ] Serial envia pacotes a 20 Hz constante
- [ ] LED piscando a 1 Hz independente
- [ ] Formato do pacote: `#timestamp;alt;temp;pres#`
- [ ] README com esquema de ligação dos pinos
- [ ] (Sem hardware) projeto simulado no Wokwi com o link compartilhado
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Por que `delay()` atrapalha a taxa de amostragem?
- O que acontece com o barramento I2C se o SDA ou SCL não tiver pull-up?
- Qual a diferença entre 9600 e 115200 baud? Quando usar cada um?

## Referências

- [Wokwi - simulador online](https://wokwi.com/)
- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Slide deck](../slides/semana-03.html)
- [Quiz](../quiz/semana-03.html)
