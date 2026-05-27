# Semana 5 — Integração Arduino ↔ Python

## Objetivo

Fazer o Arduino enviar dados pela serial e o Python receber, parsear e salvar em tempo real.

## Preparação

- [ ] Semanas 3 e 4 concluídas (Arduino enviando pacotes)
- [ ] Semanas 1 e 2 concluídas (parser Python funcional)
- [ ] ESP32-C3 conectado via USB
- [ ] Rever [serial no Arduino](../trilhas/arduino-ide.md#3-serial--comunicao-com-o-pc)
- [ ] Rever [parsing no Python](../trilhas/python.md#12-parsing-de-pacotes-serial)
- [ ] Ver o [Slide deck](../slides/semana-05.html)
- [ ] Fazer o [Quiz](../quiz/semana-05.html)

## Problema

### Lado Arduino

O ESP32-C3 deve enviar pacotes a **20 Hz** no formato:

```
#timestamp_ms;altitude_m;temperatura_C;pressao_hPa;ax;ay;az#
```

- Use o código das Semanas 3/4
- Mantenha o LED piscando a 1 Hz
- O ESP deve ficar ligado e enviando por tempo indeterminado

### Lado Python

Crie um script que:

1. **Conecta** na porta serial onde o ESP32 está
2. **Lê** linhas continuamente
3. **Parseia** cada linha com o parser da Semana 2
4. **Valida** com o validador da Semana 2
5. **Salva** pacotes válidos em CSV
6. **Registra** pacotes inválidos em `erros.log`
7. **Exibe** estatísticas a cada 50 pacotes:

```
[14:30:15] 150 pacotes | 142 OK | 5 parser err | 3 valid rej | 94.7% OK
```

**Requisitos de robustez:**
- Trate desconexão da serial (`serial.SerialException`)
- Trate linha corrompida sem quebrar o loop
- Use `serial.timeout` para não travar se o ESP parar de enviar
- O script deve rodar 5+ minutos sem perder pacotes

## Dicas

- Descubra a porta: `ls /dev/tty*` no Linux, ou `COM3` no Windows
- `serial.Serial(porta, 115200, timeout=1)` — timeout de 1s evita travamento
- `ser.readline()` retorna bytes — use `.decode("utf-8", errors="ignore")` e `.strip()`
- Reaproveite as funções `parse_packet()` e `validate_packet()` da Semana 2
- Para o CSV: abra uma vez no início e escreva cada pacote válido com `csv.writer`

## Extra (opcional)

- Gráfico em tempo real com matplotlib (`plt.ion()`)
- Envie um comando pelo serial para o ESP (ex: "R" reseta, "P" pausa)
- Calcule e exiba a taxa de amostragem real (pacotes/segundo)

## Critérios de aceite

- [ ] Arduino envia a 20 Hz constante
- [ ] Python lê, parseia e valida sem perder dados
- [ ] CSV de saída com dados limpos
- [ ] Log de erros separado
- [ ] Roda 5+ min sem travamento
- [ ] Commit com tipo semântico
- [ ] README com instruções de execução

## Perguntas para reflexão

- O que acontece se o Python ler mais devagar que o Arduino envia?
- Por que separar parser e validador em funções diferentes?
- Como garantir que o CSV não corrompa se o script for interrompido no meio?

## Referências

- [Trilha Python](../trilhas/python.md)
- [Trilha Arduino IDE](../trilhas/arduino-ide.md)
- [Slide deck](../slides/semana-05.html)
- [Quiz](../quiz/semana-05.html)
