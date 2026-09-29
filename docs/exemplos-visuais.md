# Guia de Exemplos Visuais

Referência visual do que você deve esperar de cada etapa do treinamento.

## Python - Saída Esperada

### Estatísticas de CSV

```
=== Análise de Telemetria ===
Total de amostras: 1250
Duração: 62.5 segundos

Altitude:
  Máxima: 452.30 m
  Mínima: 0.00 m
  Média: 187.45 m

Temperatura:
  Máxima: 28.50 °C
  Mínima: 22.10 °C
  Média: 25.30 °C
```

### Gráfico de Perfil de Voo

```
Perfil de Voo - Flight Computer
     ↑
 500 │        ╱╲
     │       ╱  ╲
 400 │      ╱    ╲
     │     ╱      ╲
 300 │    ╱        ╲
     │   ╱          ╲
 200 │  ╱            ╲
     │ ╱              ╲
 100 │╱                ╲
     └──────────────────→
     0   10  20  30  40  50  60s
```

### Output Serial (Arduino → Python)

```
#1250;45.23;25.3;1013.25#
#1300;45.89;25.4;1013.20#
#1350;46.12;25.3;1013.18#
```

---

## Arduino - Serial Monitor

### Formato de Pacote

```
#timestamp;altitude;temp;pressao#
#1250;45.23;25.30;1013.25#
#1300;45.89;25.40;1013.20#
```

### Scan I2C

```
Escaneando I2C...
Dispositivo em 0x76
Dispositivo em 0x68
2 dispositivos encontrados
```

### Erros Comuns

```
BMP280 não encontrado!
```
→ Verifique conexões VCC, GND, SDA, SCL

```
ERRO:BMP280#
```
→ Sensor não respondeu durante leitura

---

## PlatformIO - Build Output

### Compilação Bem-Sucedida

```
Building in release mode
Compiling .pio/build/esp32c3/src/main.cpp.o
Compiling .pio/build/esp32c3/lib/calc/SensorManager.cpp.o
Linking .pio/build/esp32c3/firmware.elf
Checking size .pio/build/esp32c3/firmware.elf
RAM:   [===       ]  32.5% (used 26624 bytes from 81920)
Flash: [====      ]  38.2% (used 401408 bytes from 1048576)
```

### Testes Nativos

```
test/test_vz/test_vz.cpp:15:test_ema_filter: PASS
test/test_vz/test_vz.cpp:23:test_vertical_velocity: PASS
test/test_apogee/test_apogee.cpp:12:test_detect_apogee: PASS

-----------------------
3 Tests 0 Failures 0 Ignored
```

---

## KiCad - Verificações

### ERC (Electrical Rules Check)

```
ERC Messages:
  Errors: 0
  Warnings: 2
    - Pin connected to multiple nets (PWR_FLAG)
    - Input pin not driven (GPIO2)
```

### DRC (Design Rules Check)

```
DRC Messages:
  Errors: 0
  Warnings: 1
    - Silkscreen overlapping (R1, R2)
```

---

## GitHub - PR Template

### Pull Request Title

```
feat: adiciona parser de pacote de telemetria
```

### PR Description

```markdown
## Descrição
Implementa parser para pacotes no formato `#timestamp;altitude;temp;pressao#`

## Mudanças
- Adiciona função `parse_packet()` em `src/parser.py`
- Adiciona validação de campos numéricos
- Testes unitários em `tests/test_parser.py`

## Checklist
- [x] Código compila/roda sem erros
- [x] Testes passam
- [x] README atualizado
- [ ] Screenshots do output (opcional)
```

---

## Conexões de Hardware

### ESP32-C3 + BMP280

```
┌─────────────┐      ┌─────────────┐
│  ESP32-C3   │      │   BMP280    │
├─────────────┤      ├─────────────┤
│  3.3V  ─────┼──────┼──── VCC     │
│  GND   ─────┼──────┼──── GND     │
│  GPIO4 ─────┼──────┼──── SDA     │
│  GPIO5 ─────┼──────┼──── SCL     │
└─────────────┘      └─────────────┘

Pull-ups: 10kΩ entre SDA→3.3V e SCL→3.3V
```

### ESP32-C3 + LoRa (RFM95W)

```
┌─────────────┐      ┌─────────────┐
│  ESP32-C3   │      │  RFM95W     │
├─────────────┤      ├─────────────┤
│  GPIO7  ────┼──────┼──── NSS/CS  │
│  GPIO1  ────┼──────┼──── RESET   │
│  GPIO2  ────┼──────┼──── DIO0    │
│  GPIO6  ────┼──────┼──── MOSI    │
│  GPIO5  ────┼──────┼──── MISO    │
│  GPIO4  ────┼──────┼──── SCK     │
│  3.3V   ────┼──────┼──── VCC     │
│  GND    ────┼──────┼──── GND     │
└─────────────┘      └─────────────┘
```

---

## Interpretando Gráficos

### Perfil de Voo Típico

```
Altitude (m)
    ↑
    │         ·  ← Apogeu
    │        / \
    │       /   \
    │      /     \  ← Descida
    │     /       \
    │    /         \___________  ← Pouso
    │   / Ascensão
    └──────────────────────────→ Tempo (s)
```

### Dados com Ruído vs Filtrados

```
Valor
  ↑   ~~ dados brutos
  │  ~~~~ 
  │ ──────── dados filtrados (EMA)
  │  ~~~~
  └──────────────────────────→ Tempo
```

---

## Checklists Visuais

### Status do PR

```
✅ Código compila
✅ Testes passam
✅ README atualizado
✅ Branch nome-sobrenome
✅ Commit tipo semântico
⬜ Screenshots (opcional)
```

### Progressão do Aluno

```
Semana 0: [✅] Setup
Semana 1: [✅] Python básico
Semana 2: [✅] Python aplicado
Semana 3: [✅] Arduino
Semana 4: [✅] PlatformIO
Semana 5: [⬜] Integração
Semana 6: [⬜] KiCad
Semana 7: [⬜] Projetos reais
Semana 8: [⬜] Capstone
```
