# Semana 8 - Capstone

## Objetivo

Integrar tudo que aprendeu em um pipeline end-to-end: Arduino gera dados → Python recebe, processa e analisa. Este é o momento de aplicar todo o conteúdo das 8 semanas em um projeto funcional.

## Preparação

- [ ] Semanas 0 a 7 concluídas
- [ ] ESP32-C3 + BMP280 montados na protoboard
- [ ] Ver o [Slide deck](../slides/semana-08.html)
- [ ] Fazer o [Quiz](../quiz/semana-08.html)
- [ ] Revisar as trilhas de [Python](../trilhas/python.md), [Arduino](../trilhas/arduino-ide.md) e [PlatformIO](../trilhas/platformio.md)

## Problema

Implemente um pipeline completo de telemetria com 4 fases, inspirado nos projetos reais.

### Fase 1 - Aquisição (Arduino)

O ESP32-C3 deve funcionar como um mini Flight Computer:

Hardware:
- ESP32-C3 Super Mini
- BMP280 (I2C, endereço 0x76)
- LED (GPIO 1) como heartbeat
- Buzzer (GPIO 0) para alertas
- Botão (GPIO 2) para trigger manual

Firmware:
```cpp
// main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define LED_PIN 1
#define BUZZER_PIN 0
#define BUTTON_PIN 2

Adafruit_BMP280 bmp;
unsigned long lastSample = 0;
const unsigned long INTERVAL = 50; // 20 Hz

float peakAltitude = 0;
int apogeeCount = 0;
bool apogeeDetected = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    Wire.begin(8, 9); // SDA=GPIO8, SCL=GPIO9
    
    if (!bmp.begin(0x76)) {
        Serial.println("ERRO:BMP280#");
        while (1);
    }
    
    Serial.println("BOOT OK#");
}

void loop() {
    unsigned long now = millis();
    
    // LED heartbeat (1 Hz)
    digitalWrite(LED_PIN, (now / 500) % 2);
    
    // Amostragem a 20 Hz
    if (now - lastSample >= INTERVAL) {
        lastSample = now;
        
        // Ler sensores
        float alt = bmp.readAltitude(1013.25);
        float temp = bmp.readTemperature();
        float pres = bmp.readPressure() / 100.0F;
        
        // Simular acelerômetro (substitua por ICM-20602 real)
        float ax = 0.01 + (random(-10, 10) / 1000.0);
        float ay = 0.02 + (random(-10, 10) / 1000.0);
        float az = 9.81 + (random(-50, 50) / 1000.0);
        
        // Calcular checksum (soma dos caracteres do payload)
        char payload[128];
        snprintf(payload, sizeof(payload), "%lu;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f",
            now, alt, ax, ay, az, temp, pres);
        
        uint8_t checksum = 0;
        for (int i = 0; payload[i] != 0; i++) {
            checksum ^= payload[i];
        }
        
        // Montar pacote completo
        char packet[144];
        snprintf(packet, sizeof(packet), "#%s;%02X#", payload, checksum);
        Serial.println(packet);
        
        // Detectar apogeu (simplificado)
        if (alt > peakAltitude) {
            peakAltitude = alt;
            apogeeCount = 0;
        } else {
            apogeeCount++;
            if (apogeeCount >= 10 && !apogeeDetected) {
                apogeeDetected = true;
                digitalWrite(BUZZER_PIN, HIGH);
                delay(200);
                digitalWrite(BUZZER_PIN, LOW);
                Serial.println("EVENT:APOGEE#");
            }
        }
        
        // Reset com botão
        if (digitalRead(BUTTON_PIN) == LOW) {
            peakAltitude = 0;
            apogeeCount = 0;
            apogeeDetected = false;
            Serial.println("EVENT:RESET#");
            delay(200); // debounce
        }
    }
}
```

Formato do pacote:
```
#timestamp;alt;ax;ay;az;temp;pres;checksum#
Exemplo: #12345;150.25;0.01;0.02;9.81;25.3;1013.25;A3#
```

### Fase 2 - Recepção (Python)

Script que recebe, valida e armazena dados:

```python
#!/usr/bin/env python3
"""receptor.py - Recebe pacotes do ESP32 via serial"""

import serial
import csv
import time
from datetime import datetime

def parse_packet(line):
    """Parse e valida pacote no formato #payload;checksum#"""
    if not line.startswith('#') or not line.endswith('#'):
        return None, "Formato inválido"
    
    content = line[1:-1]  # Remove # inicial e final
    parts = content.split(';')
    
    if len(parts) != 7:
        return None, "Número de campos inválido"
    
    try:
        timestamp = int(parts[0])
        altitude = float(parts[1])
        ax = float(parts[2])
        ay = float(parts[3])
        az = float(parts[4])
        temp = float(parts[5])
        pres = float(parts[6])
        checksum = parts[7] if len(parts) > 7 else None
    except (ValueError, IndexError) as e:
        return None, f"Erro de parsing: {e}"
    
    # Validar ranges físicos
    if altitude < -100 or altitude > 10000:
        return None, f"Altitude fora do range: {altitude}"
    
    g = (ax2 + ay2 + az2)  0.5
    if g < 8.0 or g > 12.0:
        return None, f"Aceleração fora do range: {g:.2f}g"
    
    if temp < -40 or temp > 85:
        return None, f"Temperatura fora do range: {temp}"
    
    if pres < 300 or pres > 1200:
        return None, f"Pressão fora do range: {pres}"
    
    # Verificar checksum
    if checksum:
        payload = ';'.join(parts[:7])
        calc_checksum = 0
        for c in payload:
            calc_checksum ^= ord(c)
        if f"{calc_checksum:02X}" != checksum:
            return None, f"Checksum inválido"
    
    return {
        'timestamp': timestamp,
        'altitude': altitude,
        'ax': ax, 'ay': ay, 'az': az,
        'temp': temp,
        'pres': pres
    }, None

def main():
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    
    valid_count = 0
    invalid_count = 0
    start_time = time.time()
    
    # Preparar arquivos
    csv_file = open('dados/telemetria.csv', 'w', newline='')
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(['timestamp', 'altitude', 'ax', 'ay', 'az', 'temp', 'pres'])
    
    error_log = open('dados/erros.log', 'w')
    
    print("Aguardando pacotes...")
    print("Pressione Ctrl+C para parar")
    
    try:
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if not line:
                continue
            
            data, error = parse_packet(line)
            
            if data:
                valid_count += 1
                csv_writer.writerow([
                    data['timestamp'], data['altitude'],
                    data['ax'], data['ay'], data['az'],
                    data['temp'], data['pres']
                ])
                
                # Estatísticas a cada 50 pacotes
                if valid_count % 50 == 0:
                    elapsed = time.time() - start_time
                    rate = valid_count / elapsed if elapsed > 0 else 0
                    print(f"[{valid_count}] Válidos: {valid_count} | "
                          f"Inválidos: {invalid_count} | "
                          f"Taxa: {rate:.1f} pkt/s")
            else:
                invalid_count += 1
                error_log.write(f"{datetime.now().isoformat()} | {line} | {error}\n")
    
    except KeyboardInterrupt:
        print("\nEncerrando...")
    finally:
        csv_file.close()
        error_log.close()
        ser.close()
        
        # Relatório final
        total = valid_count + invalid_count
        print(f"\nRelatório:")
        print(f"  Total: {total}")
        print(f"  Válidos: {valid_count} ({100*valid_count/total:.1f}%)")
        print(f"  Inválidos: {invalid_count} ({100*invalid_count/total:.1f}%)")

if __name__ == '__main__':
    main()
```

### Fase 3 - Análise (Python)

Script que gera relatório e gráficos:

```python
#!/usr/bin/env python3
"""analise.py - Analisa dados de telemetria coletados"""

import csv
import matplotlib.pyplot as plt
import statistics

def analyze_data(filename):
    timestamps, altitudes, temps, pressures = [], [], [], []
    
    with open(filename) as f:
        reader = csv.DictReader(f)
        for row in reader:
            timestamps.append(int(row['timestamp']) / 1000)  # ms → s
            altitudes.append(float(row['altitude']))
            temps.append(float(row['temp']))
            pressures.append(float(row['pres']))
    
    # Encontrar apogeu
    apogee_idx = altitudes.index(max(altitudes))
    apogee_time = timestamps[apogee_idx]
    apogee_alt = altitudes[apogee_idx]
    
    # Estatísticas
    stats = {
        'Altitude (m)': {
            'Média': statistics.mean(altitudes),
            'Máxima': max(altitudes),
            'Mínima': min(altitudes),
            'Std': statistics.stdev(altitudes) if len(altitudes) > 1 else 0
        },
        'Temperatura (°C)': {
            'Média': statistics.mean(temps),
            'Máxima': max(temps),
            'Mínima': min(temps)
        },
        'Pressão (hPa)': {
            'Média': statistics.mean(pressures),
            'Máxima': max(pressures),
            'Mínima': min(pressures)
        }
    }
    
    # Gráfico
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
    
    ax1.plot(timestamps, altitudes, 'b-', linewidth=2, label='Altitude')
    ax1.axvline(x=apogee_time, color='r', linestyle='--', 
                label=f'Apogeu: {apogee_alt:.1f}m')
    ax1.set_xlabel('Tempo (s)')
    ax1.set_ylabel('Altitude (m)')
    ax1.set_title('Perfil de Voo')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    ax2.plot(timestamps, temps, 'r-', label='Temperatura')
    ax2.plot(timestamps, pressures, 'g-', label='Pressão')
    ax2.set_xlabel('Tempo (s)')
    ax2.set_ylabel('Temperatura (°C) / Pressão (hPa)')
    ax2.set_title('Ambiente')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('resultados/perfil_voo.png', dpi=150)
    
    return stats, apogee_alt, apogee_time

def main():
    print("Analisando dados de telemetria...")
    
    stats, apogee_alt, apogee_time = analyze_data('dados/telemetria.csv')
    
    print(f"\n{'='*50}")
    print(f"APOGEU: {apogee_alt:.1f}m em t={apogee_time:.1f}s")
    print(f"{'='*50}\n")
    
    for campo, valores in stats.items():
        print(f"{campo}:")
        for nome, valor in valores.items():
            print(f"  {nome}: {valor:.2f}")
        print()
    
    print("Gráfico salvo em: resultados/perfil_voo.png")

if __name__ == '__main__':
    main()
```

### Fase 4 - Documentação

README.md completo com:

```markdown
# Capstone - Pipeline de Telemetria

## Visão Geral

Pipeline end-to-end inspirado nos projetos Flight Computer e Helike da Serra Rocketry.

## Arquitetura

```
ESP32-C3 (20 Hz) → Serial USB → Python Receptor → CSV + Logs
                                          ↓
                                   Python Análise → Gráficos + Estatísticas
```

## Como Executar

### 1. Firmware (Arduino/PlatformIO)

```bash
# Arduino IDE
# Abrir firmware/src/main.cpp
# Selecionar: ESP32-C3 Super Mini
# Upload

# PlatformIO
pio run -t upload
```

### 2. Receptor Python

```bash
pip install pyserial
python receptor.py
```

### 3. Análise

```bash
python analise.py
```

## Resultados

![Perfil de Voo](resultados/perfil_voo.png)

| Métrica | Valor |
|---------|-------|
| Total de pacotes | 250 |
| Válidos | 248 (99.2%) |
| Inválidos | 2 (0.8%) |
| Apogeu | 152.3m |
| Taxa | 20 pkt/s |

## Lições Aprendidas

- O que foi mais difícil? [Resposta do aluno]
- O que faria diferente? [Resposta do aluno]
- Conexão com projetos reais: [Reflexão]

## Referências

- Flight Computer: https://github.com/ViniciusCMB/flight-computer
- Helike: https://github.com/ViniciusCMB/satellite
```

## Estrutura de entrega

```
capstone/
├── firmware/
│   ├── src/
│   │   └── main.cpp
│   └── platformio.ini
├── script/
│   ├── receptor.py
│   ├── analise.py
│   └── requirements.txt
├── dados/
│   ├── telemetria.csv
│   └── erros.log
├── resultados/
│   └── perfil_voo.png
└── README.md
```

## Critérios de avaliação

| Critério | Peso | Descrição |
|---|---|---|
| Pipeline funcional | 40% | Dado entra → dado sai → gráfico |
| Qualidade do código | 25% | Organização, legibilidade, comments |
| Documentação | 20% | README, gráfico, lições |
| Organização | 15% | Estrutura de pastas, gitignore |

Bonus (até +10%):
- Checksum funcional (+3%)
- Detecção de apogeu (+3%)
- Testes automatizados (+2%)
- Interface gráfica (+2%)

## Perguntas para reflexão

- O que você faria diferente se fosse para um voo real?
- Como tornar o sistema mais robusto a falhas?
- O que falta para esse pipeline virar o firmware de verdade do Flight Computer?
- Como o FreeRTOS ajudaria在这个projeto?
- Como integrar com LoRa em vez de serial?

## Dicas

- Comece simples (só altitude) e vá adicionando campos
- Valide cada etapa antes de avançar
- Documente problemas encontrados e como resolveu
- Use os exemplos das trilhas como base

## Referências

- [Trilha Python](../trilhas/python.md)
- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Trilha PlatformIO](../trilhas/platformio.md)
- [Trilha Arquitetura de Firmware](../trilhas/firmware-architecture.md)
- [Slide deck](../slides/semana-08.html)
- [Quiz](../quiz/semana-08.html)
