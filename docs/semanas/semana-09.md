# Semana 9 - Chips personalizados no Wokwi

## Objetivo

Adicionar os **componentes personalizados** (custom chips) do computador de bordo ao [Wokwi](https://wokwi.com/), rodar um firmware que lê os três sensores e gerar a telemetria, e fechar o ciclo do treinamento analisando o log no Colab.

## Contexto

O firmware de voo precisa ser validado antes de ir para o hardware físico. No Wokwi, os sensores são **chips personalizados** escritos na Chips API (C compilado para WebAssembly): eles respondem em I2C/UART exatamente como os componentes reais, então o mesmo firmware de telemetria pode ser exercitado sem montar a placa.

Esta semana fecha o ciclo do treinamento com o hardware do Flight Computer (ESP32-S3 + BMP585 + LSM6DS3 + NEO-8M):

```
sensores simulados (custom chips)
        -> firmware (ESP32-S3)
        -> log serial cru
        -> parser -> CSV tratado
        -> graficos (Colab)
```

> Esta semana é **oficial** e vem depois do capstone. Ela usa as mesmas habilidades das Semanas 1, 2 e 3, agora com os sensores reais do computador de bordo simulados.

## Preparação

- [ ] Semanas 0 a 8 concluídas (esta semana reaproveita as Semanas 1, 2 e 3)
- [ ] Conta no [Wokwi](https://wokwi.com/) (login com GitHub)
- [ ] Ler o [README dos chips personalizados](../../entregas/dados/wokwi-chips/README.md)
- [ ] Ler a [Trilha Sensores e Protocolos](../trilhas/sensors.md) (seções de I2C e UART)
- [ ] Ver o [Slide deck](../slides/semana-09.html)
- [ ] Fazer o [Quiz](../quiz/semana-09.html)

## Kit de simulação

Os chips ficam em [`entregas/dados/wokwi-chips/`](../../entregas/dados/wokwi-chips/) e são **somente leitura**:

| Chip | Pasta | Interface | Endereço / Baud |
|------|-------|-----------|-----------------|
| BMP585 (barômetro) | `bmp585/` | I2C | `0x7E` |
| LSM6DS3 (IMU) | `lsm6ds3/` | I2C | `0x6B` |
| NEO-8M (GPS) | `neo-8m/` | UART | `9600` baud |

Ligação (a mesma do `diagram-exemplo.json` e do `config.h` do computador de bordo):

| Sensor | Sinal | ESP32-S3 |
|--------|-------|----------|
| BMP585 | SDA / SCL | GPIO 8 / GPIO 9 |
| LSM6DS3 | SDA / SCL | GPIO 8 / GPIO 9 |
| NEO-8M | TX / RX | GPIO 17 / GPIO 18 |
| Todos | VCC / GND | 3.3V / GND |

> Os dois sensores I2C compartilham o mesmo barramento (GPIO 8/9). O GPS é UART a 9600 baud, então usa `Serial1` (a `Serial` continua livre para o log no monitor).

## Problema

### Tarefa 1 - Adicionar os chips personalizados

Monte o projeto **do zero** (não vale importar o exemplo pronto) para praticar o fluxo que você vai repetir em qualquer sensor novo:

1. Abra [wokwi.com](https://wokwi.com/) e crie um projeto com a placa **ESP32-S3** (`board-esp32-s3-devkitc-1`).
2. Clique no **"+"** da barra roxa e adicione um **Custom Chip** com:
   - nome `bmp585`, linguagem **C**;
   - depois `lsm6ds3`, também **C**;
   - depois `neo-8m`, também **C**.
3. Para cada chip, **substitua** o conteúdo dos dois arquivos gerados pelo Wokwi (`<nome>.chip.json` e `<nome>.chip.c`) pelos arquivos da pasta correspondente em `entregas/dados/wokwi-chips/`.
4. Ligue os pinos conforme a tabela acima. No `diagram.json`, os chips aparecem como `chip-bmp585`, `chip-lsm6ds3` e `chip-neo-8m`.
5. Confira se as mensagens de boot aparecem no **Chips Console**:
   ```
   BMP585 pronto (I2C 0x7E)
   LSM6DS3 pronto (I2C 0x6B)
   NEO-8M pronto: lat=... lon=... alt=...
   ```

Referência: o [`diagram-exemplo.json`](../../entregas/dados/wokwi-chips/diagram-exemplo.json) mostra o resultado esperado das ligações. Use-o para **conferir** o seu diagrama, não para substituir a tarefa.

### Tarefa 2 - Firmware de telemetria

Substitua o `sketch.ino` pelo firmware abaixo. Ele lê os três chips, converte a pressão em altitude e envia um pacote a **10 Hz** pela serial:

```cpp
#include <Wire.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// --- Pinos (mesmos do config.h do computador de bordo) ---
#define I2C_SDA 8
#define I2C_SCL 9
#define GPS_RX  17
#define GPS_TX  18

// --- Enderecos I2C dos chips personalizados ---
#define BMP585_ADDR  0x7E
#define LSM6DS3_ADDR 0x6B

// --- Parametros da telemetria ---
#define INTERVALO_MS 100        // 10 Hz
#define P_REF_HPA    1013.25f   // pressao de referencia (nivel do mar)

unsigned long ultimoEnvio = 0;
unsigned long contadorPacote = 0;

float lat = 0.0f, lon = 0.0f, gpsAlt = 0.0f;
int satelites = 0;
char linhaNmea[120];
size_t tamNmea = 0;

int32_t lerInt24LE(const uint8_t *buf) {
  int32_t valor = buf[0] | (buf[1] << 8) | (buf[2] << 16);
  if (valor & 0x800000) valor -= 0x1000000;   // complemento de dois
  return valor;
}

int16_t lerInt16LE(const uint8_t *buf, int offset) {
  return (int16_t)(buf[offset] | (buf[offset + 1] << 8));
}

bool lerBMP585(float &pressaoHpa, float &temperaturaC) {
  uint8_t buf[3];

  Wire.beginTransmission(BMP585_ADDR);
  Wire.write(0x20);                            // PRESS_XLSB
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)BMP585_ADDR, (uint8_t)3) != 3) return false;
  for (int i = 0; i < 3; i++) buf[i] = Wire.read();
  pressaoHpa = (lerInt24LE(buf) / 64.0f) / 100.0f;   // LSB = 1/64 Pa

  Wire.beginTransmission(BMP585_ADDR);
  Wire.write(0x1D);                            // TEMP_XLSB
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)BMP585_ADDR, (uint8_t)3) != 3) return false;
  for (int i = 0; i < 3; i++) buf[i] = Wire.read();
  temperaturaC = lerInt24LE(buf) / 65536.0f;        // LSB = 1/2^16 C

  return true;
}

bool lerLSM6DS3(float &ax, float &ay, float &az,
                float &gx, float &gy, float &gz) {
  // O chip inicia com CTRL1_XL = 0x60 (+-2 g) e CTRL2_G = 0x60 (250 dps)
  const float SENS_G = 0.000061f;      // g/LSB
  const float SENS_DPS = 0.00875f;     // dps/LSB
  uint8_t buf[6];

  Wire.beginTransmission(LSM6DS3_ADDR);
  Wire.write(0x28);                            // OUTX_L_XL
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)LSM6DS3_ADDR, (uint8_t)6) != 6) return false;
  for (int i = 0; i < 6; i++) buf[i] = Wire.read();
  ax = lerInt16LE(buf, 0) * SENS_G;
  ay = lerInt16LE(buf, 2) * SENS_G;
  az = lerInt16LE(buf, 4) * SENS_G;

  Wire.beginTransmission(LSM6DS3_ADDR);
  Wire.write(0x22);                            // OUTX_L_G
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)LSM6DS3_ADDR, (uint8_t)6) != 6) return false;
  for (int i = 0; i < 6; i++) buf[i] = Wire.read();
  gx = lerInt16LE(buf, 0) * SENS_DPS;
  gy = lerInt16LE(buf, 2) * SENS_DPS;
  gz = lerInt16LE(buf, 4) * SENS_DPS;

  return true;
}

float nmeaParaGraus(const char *valor, char hemisferio) {
  if (valor[0] == '\0') return 0.0f;
  float bruto = atof(valor);                   // ddmm.mmmm
  int graus = (int)(bruto / 100.0f);
  float minutos = bruto - graus * 100.0f;
  float decimal = graus + minutos / 60.0f;
  if (hemisferio == 'S' || hemisferio == 'W') decimal = -decimal;
  return decimal;
}

void interpretarGGA(char *linha) {
  if (strncmp(linha, "$GPGGA,", 7) != 0) return;

  char *campos[15];
  int n = 0;
  char *token = strtok(linha, ",");
  while (token != NULL && n < 15) {
    campos[n++] = token;
    token = strtok(NULL, ",");
  }
  if (n < 10 || campos[6][0] == '0') return;   // sem fix

  lat = nmeaParaGraus(campos[2], campos[3][0]);
  lon = nmeaParaGraus(campos[4], campos[5][0]);
  satelites = atoi(campos[7]);
  gpsAlt = atof(campos[9]);
}

void lerGPS() {
  while (Serial1.available() > 0) {
    char c = (char)Serial1.read();
    if (c == '\n') {
      linhaNmea[tamNmea] = '\0';
      interpretarGGA(linhaNmea);
      tamNmea = 0;
    } else if (c != '\r' && tamNmea < sizeof(linhaNmea) - 1) {
      linhaNmea[tamNmea++] = c;
    }
  }
}

float pressaoParaAltitude(float pressaoHpa) {
  return 44330.0f * (1.0f - powf(pressaoHpa / P_REF_HPA, 0.1903f));
}

bool i2cPresente(uint8_t endereco) {
  Wire.beginTransmission(endereco);
  return Wire.endTransmission() == 0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial1.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  delay(200);

  Serial.print("[INFO] BMP585=");
  Serial.print(i2cPresente(BMP585_ADDR) ? "OK" : "AUSENTE");
  Serial.print(" LSM6DS3=");
  Serial.println(i2cPresente(LSM6DS3_ADDR) ? "OK" : "AUSENTE");
  Serial.println("[INFO] pacote: ID,millis,pckt,alt,temp,press,gx,gy,gz,ax,ay,az,gps_alt,lat,lon,sat");
}

void loop() {
  lerGPS();                                  // GPS chega a 1 Hz

  unsigned long agora = millis();
  if (agora - ultimoEnvio < INTERVALO_MS) return;
  ultimoEnvio = agora;

  float pressao, temperatura;
  if (!lerBMP585(pressao, temperatura)) {
    Serial.println("[ERRO] BMP585 nao respondeu");
    return;
  }

  float ax, ay, az, gx, gy, gz;
  if (!lerLSM6DS3(ax, ay, az, gx, gy, gz)) {
    Serial.println("[ERRO] LSM6DS3 nao respondeu");
    return;
  }

  float altitude = pressaoParaAltitude(pressao);

  char pacote[192];
  snprintf(pacote, sizeof(pacote),
           "#51,%lu,%lu,%.2f,%.2f,%.2f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%.6f,%.6f,%d",
           millis(), contadorPacote, altitude, temperatura, pressao,
           gx, gy, gz, ax, ay, az, gpsAlt, lat, lon, satelites);
  Serial.println(pacote);
  contadorPacote++;
}
```

Regras do firmware:

- Taxa de envio de **10 Hz** (`INTERVALO_MS = 100`), sem `delay()` no `loop()`
- Lat/lon/GPS se atualizam a **1 Hz**; o firmware mantém o último valor válido entre atualizações
- A altitude é calculada **no firmware** a partir da pressão (`P_REF_HPA = 1013.25`)

### Tarefa 3 - Simular e salvar o log cru

1. Rode a simulação e abra o **Serial Monitor** em **115200**.
2. Deixe rodando por **5 minutos** (mínimo de **3.000 pacotes** a 10 Hz), mexendo nos controles dos chips:
   - **BMP585**: altere a pressão para simular a subida (ex.: `1013` → `950` → `800` hPa) e a temperatura;
   - **LSM6DS3**: altere a aceleração e o giroscópio (deixe `accelZ ≈ 1 g` em repouso);
   - **NEO-8M**: altere latitude/longitude/altitude para a "base".
3. Copie a saída do Serial Monitor e salve como `log_wokwi.txt` na pasta da semana.
4. Anote no README a pressão mínima atingida e o intervalo de tempo simulado.

> Dica: mantenha a linha `[INFO]` de boot no log. O parser vai ignorá-la, como já fazia com `[STATS]`/`[MESH]` na Semana 2.

### Tarefa 4 - Parser e CSV tratado

Adapte o parser da **Semana 2** para o formato desta semana. As mudanças são só três:

```python
COLUNAS = ["ID", "millis", "pckt", "alt", "temp", "press",
           "gx", "gy", "gz", "ax", "ay", "az",
           "gps_alt", "lat", "lon", "sat"]

def parse_packet(linha):
    linha = linha.strip()
    if not linha.startswith("#51,"):
        return None
    campos = linha.split(",")
    if len(campos) != len(COLUNAS):        # 16 campos
        return None
    try:
        dados = dict(zip(COLUNAS, campos))
        dados["ID"] = int(dados["ID"].lstrip("#"))
        for nome in COLUNAS[1:]:
            dados[nome] = float(dados[nome])
        dados["sat"] = int(dados["sat"])
        return dados
    except ValueError:
        return None
```

Validação (mesmos ranges físicos da Semana 2, com os campos novos):

| Campo | Range |
|---|---|
| `alt` | -100 a 10000 m |
| `temp` | -40 a 85 °C |
| `press` | 300 a 1100 hPa |
| `gps_alt` | -100 a 10000 m |
| `lat` | -90 a 90 |
| `lon` | -180 a 180 |
| `sat` | 0 a 30 |

Linhas que **não** são pacote (`[INFO]`, `[ERRO]`, vazias) devem ser ignoradas sem quebrar o script. O resultado vai para `dados_tratados.csv` **com cabeçalho**, como na Semana 2.

Saída esperada no terminal (exemplo):

```
Processando...

Resumo:
  Linhas lidas:    3002
  Pacotes:         3000
  Validos:         3000
  Parse err:          0
  Valid rej:          0
  Taxa OK:          100%
  Ignoradas:          2
CSV tratado salvo em dados_tratados.csv
```

### Tarefa 5 - Análise no Colab

Abra o `dados_tratados.csv` no Colab (como na Semana 1) e gere:

- altitude do barômetro (`alt`) **e** altitude do GPS (`gps_alt`) no tempo;
- aceleração nos três eixos (`ax`, `ay`, `az`);
- pressão (`press`) e temperatura (`temp`);
- (opcional) trajetória `lon` × `lat` colorida pelo tempo.

```python
import pandas as pd

df = pd.read_csv("dados_tratados.csv")
df["tempo_s"] = (df["millis"] - df["millis"].min()) / 1000

df.plot(x="tempo_s", y=["alt", "gps_alt"], title="Altitude (barometro x GPS)")
df.plot(x="tempo_s", y=["ax", "ay", "az"], title="Aceleracao")
df.plot(x="tempo_s", y=["press", "temp"], title="Pressao e temperatura")
```

## Formato do pacote

Pacote de **16 campos**, separados por vírgula, começando com `#51` e **sem** terminador no fim:

```
#51,millis,pckt,alt,temp,press,gx,gy,gz,ax,ay,az,gps_alt,lat,lon,sat
```

| # | Campo | Unidade | Origem |
|---|-------|---------|--------|
| 0 | `ID` | - | fixo `51` (identificador do computador de bordo) |
| 1 | `millis` | ms | `millis()` |
| 2 | `pckt` | - | contador de pacotes |
| 3 | `alt` | m | BMP585 (pressão convertida em altitude) |
| 4 | `temp` | °C | BMP585 |
| 5 | `press` | hPa | BMP585 |
| 6-8 | `gx`, `gy`, `gz` | dps | LSM6DS3 |
| 9-11 | `ax`, `ay`, `az` | g | LSM6DS3 |
| 12 | `gps_alt` | m | NEO-8M (GGA) |
| 13 | `lat` | graus decimais | NEO-8M (GGA) |
| 14 | `lon` | graus decimais | NEO-8M (GGA) |
| 15 | `sat` | - | NEO-8M (GGA) |

> É um **subconjunto** do pacote de 24 campos do computador de bordo (mesmos nomes e mesma ordem relativa), reduzido ao que os chips simulam. Assim o parser da Semana 2 é reaproveitado mudando só a lista de colunas.

## Dicas

- O BMP585 simulado **não** funciona com a biblioteca Adafruit: leia os registradores direto com `Wire` (é o que o firmware acima faz)
- `Wire.begin(sda, scl)` no ESP32-S3 define o barramento I2C em GPIO 8/9
- `Serial1.begin(9600, SERIAL_8N1, RX, TX)` usa UART1 sem ocupar a `Serial` do monitor
- O GPS do Wokwi manda `GPGGA` + `GPRMC` a cada 1 s; só o `GPGGA` interessa aqui
- `strtok()` destrói a string original: por isso o parser do GPS trabalha numa cópia (`linhaNmea`)
- A conversão barométrica é aproximada; o objetivo é a lógica, não a precisão de ±1 m
- Se o pacote sair com `nan`, algum campo veio inválido: confira o `requestFrom()` e o número de bytes lidos

## Extra (opcional)

- Adicione checksum XOR no fim do pacote (como no capstone) e valide no parser
- Calcule a norma da aceleração (`sqrt(ax² + ay² + az²)`) e plote
- Crie um contador de pacotes perdidos: compare `pckt` com o número de pacotes válidos
- Use o [wokwi-cli](https://github.com/wokwi/wokwi-cli) para rodar a simulação e capturar o log sem abrir o navegador
- Compile os chips localmente com `wokwi-cli chip compile bmp585.chip.c`

## Critérios de aceite

- [ ] Projeto no Wokwi com ESP32-S3 + os três chips personalizados, montado do zero
- [ ] Chips Console mostra `BMP585 pronto`, `LSM6DS3 pronto` e `NEO-8M pronto`
- [ ] Firmware lê os três sensores e envia pacotes a 10 Hz
- [ ] Formato de 16 campos: `#51,millis,pckt,alt,temp,press,gx,gy,gz,ax,ay,az,gps_alt,lat,lon,sat`
- [ ] `log_wokwi.txt` com pelo menos 5 minutos de simulação
- [ ] `parser.py` gera `dados_tratados.csv` com cabeçalho e só pacotes válidos
- [ ] Gráficos no Colab a partir do CSV tratado
- [ ] README da semana com links do Wokwi e do Colab
- [ ] Commit com tipo semântico

## Entrega

Dentro de `entregas/<seu-nome-sobrenome>/semana-09/`:

```
semana-09/
├── README.md              # links do Wokwi/Colab + como rodar o parser
├── sketch.ino             # copia do firmware usado no Wokwi
├── log_wokwi.txt          # log serial cru (5+ min)
├── parser.py              # parser + validador + geracao do CSV
├── dados_tratados.csv     # saida do parser
└── evidencias/
    └── graficos.png       # (ou prints dos graficos)
```

## Perguntas para reflexão

- Por que simular os sensores antes de montar o hardware físico?
- O que muda entre o log real (`voo_thonyan.txt`) e o log gerado pelos chips simulados?
- Por que o GPS atualiza a 1 Hz e o pacote sai a 10 Hz? O que o firmware faz nos 9 pacotes intermediários?
- Como você validaria que o firmware não perde amostras?
- Por que a altitude do barômetro e a do GPS não são iguais?

## Referências

- [Chips personalizados do Wokwi](../../entregas/dados/wokwi-chips/README.md)
- [Semana 1](semana-01.md) - análise no Colab
- [Semana 2](semana-02.md) - parser e CSV tratado
- [Semana 3](semana-03.md) - Arduino IDE / Wokwi
- [Trilha Sensores e Protocolos](../trilhas/sensors.md)
- [Documentação dos Custom Chips do Wokwi](https://docs.wokwi.com/chips-api/getting-started)
- [Slide deck](../slides/semana-09.html)
- [Quiz](../quiz/semana-09.html)
