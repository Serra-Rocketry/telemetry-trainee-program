# FAQ do Treinamento

## Geral

### O que fazer se eu travar?

1. Consulte o [Troubleshooting](troubleshooting.md) para problemas comuns
2. Abra uma issue com sua dúvida
3. Participe da revisão semanal para orientação

### Posso entregar algo incompleto?

Sim, desde que explique claramente o que falta no PR. O instrutor poderá orientar sobre os próximos passos.

### Preciso usar exatamente as ferramentas?

Sim, a ideia é padronizar o fluxo do time. Caso tenha limitações no computador, avise o instrutor antecipadamente.

### Quem revisa meu PR?

O instrutor da semana ou um membro mais experiente da equipe.

### Qual o prazo para entrega das semanas?

O PR deve ser aberto até o fim da semana. Se precisar de mais tempo, avise o instrutor com antecedência.

### Como funciona a revisão semanal?

Toda semana tem uma sessão de revisão onde o instrutor e outros trainees revisam os PRs, tiram dúvidas e discutem soluções.

---

## Git e GitHub

### Como crio uma branch?

```bash
git checkout -b nome-sobrenome
```

### Esqueci de criar a branch antes de committar

Se já committou na `main`, crie a branch e faça o push:
```bash
git checkout -b nome-sobrenome
git push -u origin nome-sobrenome
```

### Meu PR está com conflito

1. Atualize sua branch com a main:
```bash
git pull origin main
```
2. Resolva os conflitos no editor
3. Faça commit e push

### Como vejo o histórico de commits?

```bash
git log --oneline
```

---

## Python

### "FileNotFoundError" ao rodar o script

Causa: Caminho do CSV errado.

Soluções:
- Use caminho relativo: `python script.py dados/teste.csv`
- Verifique se o arquivo existe: `ls dados/`
- Use `sys.argv` para receber o caminho como argumento

### "ValueError: could not convert string to float"

Causa: Linha do CSV com caractere não numérico.

Solução: Use `try/except`:
```python
try:
    altitude = float(row["altitude_m"])
except ValueError:
    continue  # pula linha inválida
```

### CSV retorna 0 linhas

Causa: Arquivo vazio ou `DictReader` não encontrou cabeçalho.

Solução: Verifique se o CSV tem cabeçalho na primeira linha.

### Como instalo bibliotecas Python?

```bash
pip install numpy matplotlib scipy
```

---

## Arduino e Hardware

### Placa não aparece na porta COM/ACM

Causas possíveis:
1. Cabo USB sem dados (apenas alimentação)
2. Driver não instalado (CP2102 ou CH340)
3. Porta errada no Arduino IDE

Solução:
```bash
# Linux - listar portas disponíveis
ls /dev/tty*

# Adicionar permissão (Linux)
sudo usermod -a -G dialout $USER
# Faça logout e login novamente
```

### "Failed to connect to ESP32"

Causa: ESP32 não está em modo de upload.

Solução:
1. Segure o botão BOOT
2. Clique em Upload
3. Solte BOOT quando começar a gravar

### Serial Monitor não mostra nada

Causa: Baud rate errado.

Solução: Confirme que o Serial Monitor está em 115200 baud.

### BMP280 não aparece no scan I2C

Verifique:
1. Conexões: VCC→3.3V, GND→GND, SDA→GPIO4, SCL→GPIO5
2. Pull-ups de 10kΩ no SDA e SCL
3. Teste o endereço `0x77` se `0x76` não funcionar

### Taxa de amostragem varia muito

Causa: Uso de `delay()` no loop.

Solução: Use `millis()`:
```cpp
if (millis() - ultimo >= INTERVALO) {
    ultimo = millis();
    // ler sensor e enviar
}
```

---

## PlatformIO

### "Please build the project first"

Execute `pio run` antes de `pio run -t upload`.

### "Library not found"

Adicione a biblioteca em `platformio.ini`:
```ini
lib_deps = 
    adafruit/Adafruit BMP280 Library
```

### Build falha com "undefined reference"

Adicione `#include <Arduino.h>` no topo de `src/main.cpp`.

---

## KiCad

### ERC acusa "Power pin not connected"

Adicione PWR_FLAG:
1. Place → Power Port → PWR_FLAG
2. Conecte no 3.3V e GND

### "Input pin not driven"

Conecte o pino de entrada ao net correto ou adicione pull-up/down.

### Componente não aparece na lista

Preferences → Manage Symbol Libraries → carregue a biblioteca necessária.

---

## Integração Arduino ↔ Python

### Python não encontra a porta serial

```bash
# Linux - listar portas
ls /dev/tty*

# Windows - verificar no Gerenciador de Dispositivos
```

Use a porta correta no script e feche outros programas que usam a serial.

### "serial.serialutil.SerialException"

Causa: ESP desconectou ou porta ocupada.

Solução: Use `try/except` e feche monitores/IDEs concorrentes.

### Perda de pacotes

Causa: Python não lê rápido o suficiente.

Solução: 
- Aumente o timeout do serial
- Diminua a taxa do ESP (10 Hz em vez de 20 Hz)

---

## Dúvidas Frequentes sobre Entregas

### Preciso entregar o README?

Sim. Todo PR deve ter um README com instruções de execução.

### O que é "tipo semântico" nos commits?

Prefixo que descreve a natureza da mudança:
- `feat:` - nova funcionalidade
- `fix:` - correção de bug
- `docs:` - documentação
- `test:` - testes
- `refactor:` - refatoração

### Posso usar outro nome de branch?

Não. Use o formato `nome-sobrenome` para identificação.

### Como sei se meu PR foi aceito?

O instrutor dará feedback na review. Se aprovado, o PR será merged.
