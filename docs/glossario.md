# Glossário Técnico

Termos usados no treinamento e nos projetos da equipe.

## A

**Apogeu** — Ponto mais alto da trajetória de voo. No Flight Computer, é detectado para disparar o paraquedas.

**Arduino IDE** — Ambiente de desenvolvimento integrado para programar microcontroladores. Usado na Semana 3.

## B

**BMP280 / BME280** — Sensores de pressão e temperatura (o BME280 também mede umidade). Comunicam via I2C.

**Baud rate** — Taxa de transmissão serial em bits por segundo. 115200 é o padrão usado no treinamento.

## C

**Checksum** — Valor calculado a partir dos dados de um pacote para verificar integridade. Se o checksum não bater, o pacote está corrompido.

**CSV** — Comma-Separated Values. Formato de arquivo onde cada linha é um registro e os campos são separados por vírgulas.

## D

**Debounce** — Técnica para eliminar ruído elétrico na leitura de botões (os contatos mecânicos oscilam por alguns ms).

**Delay** — Função que pausa o programa por um tempo. Deve ser evitada em sistemas que precisam de tempo real.

## E

**EMA** — Exponential Moving Average. Filtro que suaviza sinais ruidosos dando mais peso às amostras recentes.

**ERC** — Electrical Rules Check. Verificação automática do esquemático no KiCad para encontrar erros de conexão.

**ESP32-C3** — Microcontrolador RISC-V de 32 bits com WiFi e Bluetooth. Usado no Flight Computer e Helike.

## F

**Footprint** — Representação física de um componente na PCB (pads, furos, silkscreen).

**FreeRTOS** — Sistema operacional de tempo real para microcontroladores. Usado na v2.0 do Flight Computer.

**FSM** — Finite State Machine. Modelo de comportamento com estados e transições. Ex: IDLE → ASCENT → DESCENT → LANDED.

## G

**Gerber** — Formato de arquivo padrão para fabricação de PCBs. Contém as camadas de cobre, máscara, serigrafia, etc.

**GPIO** — General Purpose Input/Output. Pino do microcontrolador que pode ser configurado como entrada ou saída digital.

## I

**I2C** — Inter-Integrated Circuit. Barramento serial de dois fios (SDA, SCL) para comunicação com sensores e periféricos.

**IMU** — Inertial Measurement Unit. Sensor que combina acelerômetro e giroscópio (ex: MPU6050, ICM-20602, LSM6DS3).

**Issue** — Tarefa, bug ou melhoria registrada no GitHub. Usada para planejar o trabalho de cada semana.

## J

**Jekyll** — Gerador de sites estáticos que converte Markdown em HTML. Usado pelo GitHub Pages.

## K

**KiCad** — Software livre para design de esquemáticos e PCBs.

## L

**Label** — No KiCad, nome dado a um fio (net) para conectar pontos distantes no esquemático sem usar um fio físico.

**LittleFS** — Sistema de arquivos para microcontroladores que usa a memória flash interna.

**LoRa** — Long Range. Protocolo de rádio de baixa potência para telemetria. Usado a 868-915 MHz.

## M

**MCU** — Microcontroller Unit. Unidade microcontroladora (ex: ESP32-C3).

**millis()** — Função do Arduino que retorna o tempo em ms desde o boot. Usada para controle de tempo sem bloquear.

## N

**NMEA** — Protocolo de comunicação usado por receptores GPS. Sentenças como $GPRMC, $GPGGA.

## P

**Parser** — Função que converte uma string bruta em uma estrutura de dados organizada.

**PCB** — Printed Circuit Board. Placa de circuito impresso.

**PlatformIO** — Ecossistema de desenvolvimento profissional para sistemas embarcados. Alternativa ao Arduino IDE.

**PocketQube** — Padrão de satélite miniaturizado (1P = 5×5×5 cm). O Helike é um PocketQube 1P.

**PR** — Pull Request. Mecanismo do GitHub para propor alterações e solicitar revisão.

**Pull-up** — Resistor que mantém um pino em nível HIGH quando não está sendo acionado. Essencial no I2C.

**PWR_FLAG** — Componente especial no KiCad que indica uma fonte de alimentação (necessário para passar no ERC).

## S

**Serial** — Comunicação ponto-a-ponto entre dispositivos (UART). Usada para debug e coleta de dados.

**Servo** — Motor com controle de posição angular. Usado no Flight Computer para acionar o paraquedas.

**SPI** — Serial Peripheral Interface. Barramento síncrono full-duplex. Usado pelo LoRa (RFM95W).

**SRAB** — Sistema de Recuperação Autorrotativo Bioinspirado. Asa rotativa (samara) para descida controlada do Helike.

**Símbolo** — Representação lógica de um componente no esquemático do KiCad.

## T

**Telemetria** — Coleta e transmissão remota de dados de sensores.

## U

**UART** — Universal Asynchronous Receiver-Transmitter. Protocolo serial para comunicação entre dispositivos.

**Unity** — Framework de testes unitários em C/C++ usado com PlatformIO.

## V

**Vz** — Velocidade vertical (derivada da altitude). Usada para detectar apogeu.
