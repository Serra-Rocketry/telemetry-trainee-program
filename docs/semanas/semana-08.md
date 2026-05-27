# Semana 8 — Capstone

## Objetivo

Integrar tudo que aprendeu em um pipeline end-to-end: Arduino gera dados → Python recebe, processa e analisa.

## Preparação

- [ ] Semanas 0 a 7 concluídas
- [ ] ESP32-C3 + BMP280 montados na protoboard
- [ ] Ver o [Slide deck](../slides/semana-08.html)
- [ ] Fazer o [Quiz](../quiz/semana-08.html)

## Problema

Implemente um pipeline completo de telemetria com 4 fases.

### Fase 1 — Aquisição (Arduino)

O ESP32-C3 deve:
- Ler BMP280 a **20 Hz** via I2C
- Piscar LED como heartbeat (1 Hz)
- Calcular **checksum** do pacote e incluir no payload
- Detectar **apogeu**: se altitude cair por N amostras consecutivas, acender um LED de aviso
- Formato: `#timestamp;alt;ax;ay;az;temp;pres;chk#`

### Fase 2 — Recepção (Python)

Script que:
- Conecta na serial do ESP
- Parseia cada pacote em tempo real
- Valida campos (range físico + checksum)
- Salva válidos em CSV, inválidos em `erros.log`
- Estatísticas a cada 50 pacotes

### Fase 3 — Análise (Python)

Após coletar **200+ pacotes**, gere:
- Resumo estatístico (média, max, min por campo)
- Gráfico de altitude × tempo (salvar PNG)
- Apogeu marcado no gráfico

### Fase 4 — Documentação

README.md completo com:
- Como executar (Arduino + Python)
- Gráfico de resultado
- Tabela de estatísticas
- Lições aprendidas: o que foi mais difícil? O que faria diferente?

## Estrutura de entrega

```
capstone/
├── firmware/           # Código do Arduino/PlatformIO
│   ├── src/main.cpp
│   └── platformio.ini
├── script/             # Código Python
│   ├── receptor.py
│   ├── analise.py
│   └── requirements.txt
├── dados/
│   └── log_telemetria.csv
├── resultados/
│   └── perfil_voo.png
└── README.md
```

## Critérios de avaliação

| Critério | Peso |
|---|---|
| Pipeline funcional (dado entra, dado sai) | 40% |
| Qualidade do código (organização, legibilidade) | 25% |
| Documentação (README, gráfico, lições) | 20% |
| Organização do repositório | 15% |

- Commits com tipos semânticos (`feat:`, `build:`, `docs:`, `fix:`, `test:`, ...)

## Perguntas para reflexão

- O que você faria diferente se fosse para um voo real?
- Como tornar o sistema mais robusto a falhas?
- O que falta para esse pipeline virar o firmware de verdade do Flight Computer?

## Referências

- [Trilha Python](../trilhas/python.md)
- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Trilha PlatformIO](../trilhas/platformio.md)
- [Slide deck](../slides/semana-08.html)
- [Quiz](../quiz/semana-08.html)
