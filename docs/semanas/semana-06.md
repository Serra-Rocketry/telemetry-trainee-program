# Semana 6 — KiCad básico

## Objetivo

Criar um esquemático de ESP32-C3 + BMP280 no KiCad e rodar ERC.

## Preparação

- [ ] KiCad 8+ instalado
- [ ] Ler a [Trilha KiCad](../trilhas/kicad.md) (seções 1 a 5)
- [ ] Ver o [Slide deck](../slides/semana-06.html)
- [ ] Fazer o [Quiz](../quiz/semana-06.html)

## Problema

### Tarefa 1 — Criar projeto

Crie um novo projeto KiCad: `semana-6-kicad`

### Tarefa 2 — Esquemático

No Eeschema, monte o circuito:

**Componentes:**
- ESP32-C3 (use um conector ou símbolo genérico de MCU)
- BMP280 (`Sensor_Temperature:BMP280`)
- 2 resistores de 10k (pull-up I2C)
- 1 capacitor de 100nF (desacoplamento)
- Conector de expansão (opcional)

**Conexões:**
- BMP280 VCC → 3.3V
- BMP280 GND → GND
- BMP280 SDA → I2C_SDA (label global)
- BMP280 SCL → I2C_SCL (label global)
- Pull-up de 10k de I2C_SDA para 3.3V
- Pull-up de 10k de I2C_SCL para 3.3V
- ESP GPIO4 → I2C_SDA
- ESP GPIO5 → I2C_SCL
- Capacitor 100nF entre 3.3V e GND (próximo ao ESP)
- PWR_FLAG no 3.3V e no GND

### Tarefa 3 — ERC

Rode o ERC (Inspect → Electrical Rules Check).

**Corrija todos os erros críticos.** Os mais comuns:

| Erro | Causa | Solução |
|---|---|---|
| "Power pin not connected" | Faltou PWR_FLAG | Adicione PWR_FLAG |
| "Input pin not driven" | Pino de entrada solto | Conecte ao net correto |
| "Pin connected to other pin" | Dois outputs ligados | Revise a conexão |

### Tarefa 4 — Exportar PDF

Exporte o esquemático como PDF (File → Plot → PDF).

## Dicas

- `PWR_FLAG` é um componente especial: Place → Power Port → selecione PWR_FLAG
- Use labels globais (Place → Global Label) para SDA e SCL
- A tecla W é o Wire (fio) para conexões
- A tecla C é para copiar componentes

## Extra (opcional)

- Passe para o Pcbnew e posicione os componentes
- Roteie trilhas (VCC e GND com 0.5mm, SDA/SCL com 0.25mm)
- Adicione um plano de GND (Zones → Add Filled Zone)
- Exporte Gerber

## Critérios de aceite

- [ ] Esquemático completo com todos os componentes
- [ ] ERC sem erros críticos
- [ ] PDF exportado
- [ ] README descrevendo o circuito
- [ ] Commit com tipo semântico

## Perguntas para reflexão

- Por que o I2C precisa de resistores de pull-up?
- O que o capacitor de desacoplamento faz?
- Qual a diferença entre símbolo e footprint no KiCad?

## Referências

- [Trilha KiCad](../trilhas/kicad.md)
- [Exemplo KiCad](../examples/kicad-exemplo.md)
- [Slide deck](../slides/semana-06.html)
- [Quiz](../quiz/semana-06.html)
