# Chips customizados do Wokwi - Onboard Computer

Arquivos de **custom chips** do [Wokwi](https://wokwi.com/) que simulam os sensores do computador de bordo (ESP32-S3) descrito em `hardware.md`. São usados na [Semana 9](../../../docs/semanas/semana-09.md) do treinamento, em que a turma simula o firmware de telemetria sem precisar do hardware físico.

Os chips são escritos na **Chips API** do Wokwi (C compilado para WebAssembly). Cada chip tem dois arquivos:

- `<nome>.chip.json` - nome, pinos e controles (sliders) da simulação
- `<nome>.chip.c` - a lógica do chip

## Componentes

| Chip | Pasta | Interface | Endereço / Baud |
|------|-------|-----------|-----------------|
| BMP585 (barômetro) | `bmp585/` | I2C | `0x7E` (ajustável) |
| LSM6DS3 (IMU) | `lsm6ds3/` | I2C | `0x6B` (ajustável) |
| NEO-8M (GPS) | `neo-8m/` | UART | `9600` baud |

## Estrutura

```
wokwi-chips/
├── diagram-exemplo.json      # projeto de exemplo (ESP32-S3 + os 3 chips)
├── README.md
├── bmp585/
│   ├── bmp585.chip.json
│   └── bmp585.chip.c
├── lsm6ds3/
│   ├── lsm6ds3.chip.json
│   └── lsm6ds3.chip.c
└── neo-8m/
    ├── neo-8m.chip.json
    └── neo-8m.chip.c
```

## Como usar no Wokwi

1. Abra [wokwi.com](https://wokwi.com/) e crie um projeto novo (placa **ESP32-S3**).
2. Para cada componente, clique no **"+"** → **Custom Chip** → informe o nome (`bmp585`, `lsm6ds3`, `neo-8m`) e a linguagem **C**.
3. O Wokwi cria `bmp585.chip.json` e `bmp585.chip.c` no projeto. **Substitua o conteúdo** desses dois arquivos pelos desta pasta (mesmo nome de arquivo).
4. (Mais rápido) Cole o conteúdo de `diagram-exemplo.json` no `diagram.json` do projeto - ele já adiciona a placa ESP32-S3 e os três chips ligados.
5. Rode a simulação. As mensagens `printf()` dos chips aparecem no **Chips Console**.

> Também dá para validar/compilar localmente com o [wokwi-cli](https://github.com/wokwi/wokwi-cli): `wokwi-cli chip compile bmp585.chip.c`.

## Ligação de pinos

Baseado no `config.h` do firmware (ESP32-S3):

| Sensor | Sinal | ESP32-S3 |
|--------|-------|----------|
| BMP585 | SDA | GPIO 8 |
| BMP585 | SCL | GPIO 9 |
| LSM6DS3 | SDA | GPIO 8 |
| LSM6DS3 | SCL | GPIO 9 |
| NEO-8M | TX | GPIO 17 (RX_GPS) |
| NEO-8M | RX | GPIO 18 (TX_GPS) |
| Todos | VCC / GND | 3.3V / GND |

> Os dois sensores I2C compartilham o mesmo barramento (GPIO 8/9). O GPS é UART a 9600 baud.

## O que cada chip simula

### BMP585

- A **pressão** é definida diretamente por um controle (em hPa) - a conversão pressão → altitude fica para o firmware/aluno fazer depois
- Registradores: `CHIP_ID` (0x51), `STATUS`, `INT_STATUS`, temp (0x1D-0x1F) e pressão (0x20-0x22), config (0x30/0x31/0x36/0x37/0x7E)
- Escala: temperatura `LSB = 1/2^16 °C`; pressão `LSB = 1/2^6 Pa`
- Controles: `pressure` (hPa), `temperature` (°C); atributo `address` (padrão `0x7E`)

### LSM6DS3

- Acelerômetro (g) e giroscópio (dps) vindos de controles; sensibilidade calculada pelos registradores `CTRL1_XL`/`CTRL2_G` (FS)
- Registradores: `WHO_AM_I` (0x6A), `STATUS_REG` (0x07 = XLDA|GDA|TDA), `OUT_TEMP_L/H` (0x20-0x21), giroscópio (0x22-0x27), acelerômetro (0x28-0x2D)
- Controles: `accelX/Y/Z` (g), `gyroX/Y/Z` (dps); atributo `address` (padrão `0x6B`)

### NEO-8M (GPS)

- Envia **NMEA 0183** (`GPGGA` + `GPRMC`) a cada 1 s, com checksum
- Se `latitude`/`longitude` não forem informadas, sorteia uma **localização aleatória** no boot e a move levemente a cada segundo
- Controles: `latitude`, `longitude`, `altitude` (m)

## Limitações

- Os registradores implementados são um **subconjunto** do datasheet. O objetivo é permitir que o firmware leia os valores essenciais (identificação, status e dados) e valide a lógica de telemetria. Bibliotecas que fazem sequências de calibração/compensação complexas podem precisar de ajustes.
- O `address` do BMP585 está em `0x7E` por padrão para casar com a medição de bancada do `hardware.md`. O datasheet do BMP585 costuma usar `0x47`/`0x46`; para mudar, ajuste o atributo `address` no `diagram.json`.
- GPS: a data no `GPRMC` é fixa (`171026`) e a hora é uma contagem a partir do boot - suficiente para exercitar o parser.

## Referências

- [Wokwi - Custom Chips](https://docs.wokwi.com/chips-api/getting-started)
- [Wokwi - Chip Definition (JSON)](https://docs.wokwi.com/chips-api/chip-json)
- [Wokwi - I2C API](https://docs.wokwi.com/chips-api/i2c) · [UART API](https://docs.wokwi.com/chips-api/uart) · [Attributes](https://docs.wokwi.com/chips-api/attributes)
- [Semana 9 - Chips personalizados no Wokwi](../../../docs/semanas/semana-09.md)
- `hardware.md` - documentação do computador de bordo
