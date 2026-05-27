# Troubleshooting

Problemas comuns e soluções organizados por semana.

## Geral

### "Comando não encontrado" (git, python, etc.)
**Causa:** Ferramenta não está no PATH ou não foi instalada.  
**Solução:** Verifique a instalação. No Windows, reinicie o terminal após instalar.

### "Permission denied" ao acessar porta serial
**Causa:** Usuário não tem permissão no Linux.  
**Solução:**
```bash
sudo usermod -a -G dialout $USER
```
Faça logout e login novamente.

---

## Semana 0 — Git

### "fatal: not a git repository"
**Causa:** Comando git executado fora do repositório.  
**Solução:** `cd` para dentro da pasta do projeto.

### "error: failed to push some refs"
**Causa:** Branch local está atrás do remoto.  
**Solução:** `git pull origin main` e resolva conflitos se houver.

### Conflito no PR
**Causa:** Dois trainees editaram a mesma linha.  
**Solução:** No GitHub, clique em "Resolve conflicts" ou faça localmente:
```bash
git pull origin main
# resolva os conflitos
git add .
git commit -m "fix: resolve conflito com main"
git push
```

---

## Semana 1 e 2 — Python

### "FileNotFoundError"
**Causa:** Caminho do CSV errado.  
**Solução:** Use caminho relativo ou `sys.argv` para receber como argumento. Verifique com `ls` ou `dir`.

### "ValueError: could not convert string to float"
**Causa:** Linha do CSV com caractere não numérico.  
**Solução:** Use `try/except` em volta do `float()`. Ignore ou registre a linha.

### CSV com 0 linhas
**Causa:** Arquivo vazio ou `DictReader` não encontrou cabeçalho.  
**Solução:** Verifique se o CSV tem cabeçalho. Trate lista vazia antes de calcular estatísticas.

---

## Semana 3 — Arduino IDE

### Placa não aparece na porta COM/ACM
**Causa:** Driver não instalado ou cabo USB defeituoso.  
**Solução:**
1. Teste outro cabo USB
2. Instale driver CP2102 ou CH340 (dependendo do módulo)
3. No Linux: `ls /dev/tty*` para listar portas

### "A fatal error occurred: Failed to connect to ESP32-C3"
**Causa:** ESP32 não está em modo de upload.  
**Solução:** Segure o botão BOOT, clique em Upload, solte BOOT quando começar a gravar.

### "Serial Monitor não mostra nada"
**Causa:** Baud rate errado.  
**Solução:** Confirme que o Serial Monitor está em 115200 baud.

### BMP280 não aparece no scan I2C
**Causa:** Conexão errada ou endereço diferente.  
**Solução:**
1. Verifique VCC (3.3V), GND, SDA (GPIO4), SCL (GPIO5)
2. Confirme os pull-ups de 10kΩ
3. Teste o endereço `0x77` se `0x76` não funcionar

### Taxa de amostragem varia
**Causa:** Uso de `delay()` no loop.  
**Solução:** Substitua por `millis()` para controle de tempo não-bloqueante.

---

## Semana 4 — PlatformIO

### "Please build the project first"
**Causa:** Tentou fazer upload sem build.  
**Solução:** Execute `pio run` antes de `pio run -t upload`.

### "Library not found"
**Causa:** Biblioteca não declarada em `lib_deps`.  
**Solução:** Adicione no `platformio.ini` e rode `pio update`.

### Build falha com "undefined reference"
**Causa:** Falta `#include <Arduino.h>` no `main.cpp`.  
**Solução:** Adicione no topo de `src/main.cpp`.

---

## Semana 5 — Integração

### Python não encontra a porta serial
**Causa:** Porta errada ou permissão.  
**Solução:**
```bash
ls /dev/tty*  # Linux
```
Use a porta correta no script. Siga o troubleshooting de permissão no início deste guia.

### "serial.serialutil.SerialException"
**Causa:** ESP desconectou ou porta ocupada.  
**Solução:** Use `try/except` em volta da leitura. Feche outros programas que usam a serial (IDE, monitor).

### Perda de pacotes
**Causa:** Python não lê rápido o suficiente.  
**Solução:** Aumente o timeout do serial ou diminua a taxa do ESP (10 Hz em vez de 20 Hz).

---

## Semana 6 — KiCad

### ERC acusa "Power pin not connected"
**Causa:** Faltou PWR_FLAG na fonte.  
**Solução:** Place → Power Port → PWR_FLAG no 3.3V e GND.

### "Input pin not driven"
**Causa:** Pino de entrada do componente não está conectado.  
**Solução:** Conecte ao net correto ou adicione resistor de pull-up/down.

### Componente não aparece na lista
**Causa:** Biblioteca não carregada.  
**Solução:** Preferences → Manage Symbol Libraries → carregue a biblioteca necessária.

---

## Semana 7 e 8

### Não consegue acessar repositório do projeto
**Causa:** Sem permissão ou repositório privado.  
**Solução:** Solicite acesso ao instrutor. Verifique se está logado no GitHub.

### Capstone não compila
**Causa:** Erro de sintaxe ou biblioteca faltando.  
**Solução:** Compile cada parte separadamente. Primeiro o firmware, depois o script Python, depois a integração.
