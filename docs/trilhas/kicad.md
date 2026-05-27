# Trilha KiCad

KiCad é a ferramenta de design de PCB usada nos projetos da Serra Rocketry. O Helike tem uma placa completa (CDB — Central Data Board) projetada no KiCad, e o Flight Computer terá sua própria revisão de hardware.

## O que você vai aprender

- Navegar no KiCad (Eeschema, Pcbnew)
- Criar um esquemático com MCU + sensores
- Associar footprints e verificar ERC
- Layout de PCB simples
- Exportar arquivos para fabricação

## Pré-requisitos

- KiCad 8+ instalado
- Conceitos básicos de eletrônica (GND, VCC, pull-up, I2C, SPI)

---

## 1. Visão geral do KiCad

Um projeto KiCad tem 3 partes principais:

| Ferramenta | Função | Atalho |
|---|---|---|
| **Eeschema** | Editor de esquemático | desenha as conexões lógicas |
| **Pcbnew** | Editor de PCB | posiciona componentes e roteia trilhas |
| **Symbol Editor** | Cria/edita símbolos | componentes no esquemático |
| **Footprint Editor** | Cria/edita footprints | pads físicos na PCB |

## 2. Criando um projeto

1. File → New → Project → escolher nome e pasta
2. O KiCad cria: `.kicad_pro` (projeto), `.kicad_sch` (esquemático), `.kicad_pcb` (PCB)

**No Helike** — O projeto CDB inclui `CDB.kicad_pro`, `CDB.kicad_sch` e `CDB.kicad_pcb` na pasta `hardware/CDB/`.

---

## 3. Esquemático — Eeschema

### 3.1 Montando um circuito MCU + sensor

Vamos criar o esquemático de um circuito mínimo com ESP32-C3 + BMP280 (barômetro I2C), similar ao Flight Computer.

**Componentes necessários:**

| Componente | Símbolo KiCad | Notas |
|---|---|---|
| ESP32-C3 Super Mini | Criar ou baixar | Microcontrolador |
| BMP280 | `Sensor_Temperature: BMP280` | Barômetro I2C |
| Resistor 10kΩ | `Device:R` | Pull-up I2C |
| Capacitor 100nF | `Device:C_Small` | Desacoplamento |
| Conector 2x4 pinos | `Connector:Conn_02x04` | Interface externa |
| Regulador LM2596 | Criar módulo | Fonte (pode ser módulo externo) |

**Conexões principais:**

```
ESP32-C3                     BMP280
GPIO4 (SDA) ────┬──────────── SDA
                └─── 10kΩ ─── 3.3V
GPIO5 (SCL) ────┬──────────── SCL
                └─── 10kΩ ─── 3.3V
3.3V ──────────────────────── VCC
GND ───────────────────────── GND
```

### 3.2 Passos no Eeschema

1. **Place → Symbol**: adicione ESP32-C3, BMP280, resistores, capacitores, conectores.
2. **Wire (W)**: conecte os pinos.
3. **Global labels**: crie labels para VCC, GND, SDA, SCL.
4. **Power ports**: coloque símbolos de alimentação (PWR_FLAG).
5. **Annotate**: Tools → Annotate Schematic (auto-numera referências: R1, C1, U1...).
6. **ERC**: Inspect → Electrical Rules Check.

### 3.3 ERC — Electrical Rules Check

O ERC verifica erros de conexão:
- Pinos de saída conectados entre si
- Alimentação não conectada
- Pinos flutuantes sem aviso

**Exemplo real — No Helike:** O CDB passou por várias iterações de ERC até eliminar todos os erros críticos antes de enviar para fabricação.

**Checklist de ERC:**
- [ ] Todos os VCCs conectados a uma fonte de alimentação
- [ ] Todos os GNDs conectados ao terra
- [ ] Nenhum pino de output ligado a outro output
- [ ] Pinos de input com resistor de pull-up quando necessário (I2C!)
- [ ] Capacitores de desacoplamento próximos a cada IC

---

## 4. PCB — Pcbnew

### 4.1 Importando do esquemático

Tools → Update PCB from Schematic (F8). Os componentes aparecem empilhados; você posiciona e roteia.

### 4.2 Layout básico

1. **Posicione os componentes**: MCU no centro, sensores nas bordas, conectores nas extremidades
2. **Camadas**: Front Copper (topo), Bottom Copper (fundo)
3. **Roteamento**: Route Tracks (X) para conectar os pads
4. **Espessura de trilha**:
   - Sinal: 0.25 mm (mínimo recomendado)
   - Alimentação: 0.5 mm ou mais
5. **Via**: muda de camada (topo → fundo)

### 4.3 Dicas de layout para projetos de foguetes

- **Vibração**: use capacitores de desacoplamento (100nF) próximos a cada IC
- **Conectores**: oriente para facilitar acesso na bancada
- **GND plane**: preencha área livre com cobre no GND (Zones → Add Filled Zone)
- **I2C**: mantenha trilhas SDA/SCL curtas e paralelas
- **Separação analógico/digital**: mantenha sensor longe de fontes de ruído (regulador)

**Flight Computer** — A placa CDB (~125mm x 50mm) é projetada para o formato SR21000. Tem ESP32-C3 + BMP280 + MPU6050 + NEO-6M + RFM95W + LM2596 em uma placa de 2 camadas.

---

## 5. Exportação para fabricação

### 5.1 Gerando Gerber (JLCPCB / PCBWay)

File → Fabrication Outputs → Gerber:
- Camadas: F.Cu, B.Cu, F.Mask, B.Mask, F.Silkscreen, Edge.Cuts
- Formato: RS-274X
- Drill: File → Fabrication Outputs → Drill (.drl)

### 5.2 Exportar PDF do esquemático

File → Plot → esquema em PDF (para documentação e revisão).

---

## 6. Boas práticas

1. **Sempre rode ERC** antes de passar para o PCB.
2. **Nomeie nets**: labels descritivos (I2C_SCL, BMP_VCC, GPS_TX) facilitam debug.
3. **Documente versões**: mantenha backups do projeto (`CDB-backups/` como no Helike).
4. **BOM**: gere a lista de materiais (Tools → BOM) para conferir componentes.
5. **Revise o .step**: gere o modelo 3D para conferir interferências mecânicas.

---

## Exercícios práticos

### Nível 1 — Navegação

1. Abra o projeto CDB do Helike em `hardware/CDB/`.
2. Explore o esquemático: identifique MCU, sensores, conectores e alimentação.
3. Mude para o PCB: veja o posicionamento dos componentes e roteamento.

### Nível 2 — Esquemático simples

1. Crie um projeto novo.
2. Adicione ESP32-C3, BMP280, 2 resistores 10k, 1 capacitor 100nF.
3. Conecte I2C (SDA/SCL com pull-ups) e alimentação (3.3V/GND).
4. Rode ERC e corrija todos os erros.
5. Exporte PDF.

### Nível 3 — PCB simples

1. Importe o esquemático para o Pcbnew.
2. Posicione os componentes manualmente.
3. Roteie VCC e GND com trilha de 0.5 mm.
4. Adicione um plano de GND (Zones → Filled Zone).
5. Exporte Gerber.

### Nível 4 — Contexto real

1. Compare seu esquemático com o CDB do Helike.
2. Identifique diferenças: o Helike tem mais sensores (ICM-20602, BME280, GPS NEO-8M) e um RFM95W LoRa.
3. Adicione um conector de expansão I2C ao seu projeto.
4. Gere BOM e confira se todos os componentes têm footprint.

---

## Conexão com os projetos reais

| Conceito | Projeto | Onde é usado |
|---|---|---|
| Esquemático MCU + sensores | Ambos | CDB do Helike, FC board |
| ERC | Ambos | Verificação de erros de conexão |
| Pull-up I2C | Ambos | Barramento I2C compartilhado |
| GND plane | Ambos | Redução de ruído em voo |
| Capacitores de desacoplamento | Ambos | Estabilidade em ambientes de vibração |
| Gerber / Fab | Helike | CDB fabricado para o PocketQube |

## Entregas relacionadas

- [Semana 6](../semanas/semana-06.md): Esquemático + ERC + exportação

## Critérios de aceite

- ERC sem erros críticos
- PDF do esquemático exportado
- Componentes com footprints atribuídos
- README com descrição do circuito
