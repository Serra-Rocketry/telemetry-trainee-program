# Exemplo — KiCad: Esquemático Mínimo

## Objetivo

Criar um esquemático de ESP32-C3 + BMP280 (I2C) no KiCad.

## Componentes

| Componente | Símbolo KiCad | Valor |
|---|---|---|
| U1 | ESP32-C3 Super Mini | Microcontrolador |
| U2 | `Sensor_Temperature:BMP280` | Barômetro I2C |
| R1, R2 | `Device:R` | 10kΩ (pull-up I2C) |
| C1 | `Device:C_Small` | 100nF (desacoplamento) |
| J1 | `Connector:Conn_02x04` | Conector expansão |

## Conexões

```
ESP32-C3 (U1)          BMP280 (U2)
GPIO4 ──────┬───────── SDA (pino 5)
            └── R1 ─── 3.3V
GPIO5 ──────┬───────── SCL (pino 4)
            └── R2 ─── 3.3V
3.3V ───────────────── VCC (pino 6)
GND ────────────────── GND (pino 3)
```

## Check-list ERC

- [ ] VCC do BMP280 conectado ao 3.3V
- [ ] GND do BMP280 conectado ao GND
- [ ] Pull-ups (R1, R2) de 10k para 3.3V
- [ ] Capacitor C1 entre VCC e GND (próximo ao ESP32)
- [ ] PWR_FLAG colocado no 3.3V e GND
- [ ] Nenhum pino de output conectado a outro output
- [ ] Todos os pinos de alimentação conectados

## Como exportar

```bash
# PDF do esquemático
File → Plot → Esquema em PDF

# Gerber para fabricação
File → Fabrication Outputs → Gerber
# Selecionar: F.Cu, B.Cu, F.Mask, B.Mask, F.Silkscreen, Edge.Cuts
```
