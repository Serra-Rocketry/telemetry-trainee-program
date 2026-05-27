# Guia do Instrutor

Roteiro para conduzir o treinamento semana a semana.

## Formato do encontro semanal

| Etapa | Duração |
|---|---|
| Mini-aula expositiva (com slides) | 20-30 min |
| Dúvidas e discussão | 10 min |
| Revisão dos PRs da semana anterior | 30-40 min |
| Apresentação da próxima semana | 5 min |

## Semana 0 — Onboarding & GitHub

**Mini-aula:**
- O que é git? Clone, add, commit, push, pull
- Branches: por que usar, como nomear
- PRs: template, revisão, merge
- Convenção de commits semânticos

**Perguntas para discussão:**
- Qual a diferença de clone, fork e branch?
- O que fazer quando dá conflito?
- Por que commits semânticos?

**Na revisão de PRs:**
- Confira se a branch segue o padrão
- Confira o formato do commit
- Dê feedback sobre a qualidade do README

## Semana 1 — Python básico

**Mini-aula:**
- Leitura de arquivos com open/with
- csv.DictReader vs Reader
- List comprehensions para extrair colunas
- try/except para arquivos e valores

**Perguntas para discussão:**
- O que `with open() as f` garante?
- Por que `DictReader` é melhor que `split(",")`?

**Na revisão:**
- Tratou arquivo inexistente?
- Tratou CSV vazio?
- Usou `sys.argv` para o caminho?

## Semana 2 — Python aplicado

**Mini-aula:**
- Parsing de strings: split, strip, validação
- Separação de responsabilidades: parser vs validador
- Tipos de erro: formato vs conteúdo

**Perguntas para discussão:**
- Por que parser e validador são funções separadas?
- O que é um pacote corrompido?

**Na revisão:**
- Parser rejeita formatos inválidos sem crash?
- Validador cobre todos os campos?
- Estatísticas no final?

## Semana 3 — Arduino IDE

**Mini-aula:**
- GPIO: pinMode, digitalWrite, analogWrite
- I2C: endereços, SDA/SCL, pull-ups
- Serial: baud rate, print vs write
- millis() sem delay

**Perguntas para discussão:**
- Por que `delay()` atrapalha a taxa de amostragem?
- O que acontece se o I2C não tiver pull-up?

**Na revisão:**
- Taxa de 20 Hz consistente?
- LED pisca independente?
- Formato do pacote correto?

## Semana 4 — PlatformIO

**Mini-aula:**
- Estrutura de projeto PlatformIO
- platformio.ini: ambientes, lib_deps, build_flags
- Testes nativos com Unity
- Header-only libraries

**Perguntas para discussão:**
- Por que manter lib/ sem dependência Arduino?
- Quando usar build_flags vs #define?

**Na revisão:**
- Build sem erros?
- Testes nativos passam?
- Organização correta das pastas?

## Semana 5 — Integração

**Mini-aula:**
- Comunicação serial bidirecional
- Buffer serial e overflow
- Robustez: tratar desconexão, linha corrompida

**Perguntas para discussão:**
- O que acontece se o Python ler mais devagar que o Arduino envia?
- Como garantir que o CSV não corrompa?

**Na revisão:**
- Pipeline roda 5+ min sem perder pacotes?
- Parser reaproveitado da Semana 2?
- CSV limpo + log de erros separado?

## Semana 6 — KiCad

**Mini-aula:**
- Eeschema: símbolos, fios, labels, ERC
- Pcbnew: posicionamento, roteamento, GND plane
- BOM e exportação

**Perguntas para discussão:**
- O que o ERC verifica?
- Por que usar PWR_FLAG?

**Na revisão:**
- ERC sem erros críticos?
- Pull-ups I2C presentes?
- PDF exportado?

## Semana 7 — Projetos reais

**Mini-aula:**
- Arquitetura do Flight Computer
- Arquitetura do Helike
- Onde cada semana se aplica

**Perguntas para discussão:**
- Quais componentes são comuns entre FC e Helike?
- O que muda na estratégia de logging entre foguete e satélite?

**Na revisão:**
- Resumos mostram entendimento da arquitetura?
- Mapa conceitual conecta semanas aos projetos?

## Semana 8 — Capstone

**Mini-aula:**
- Revisão geral dos conceitos
- Expectativas para projetos reais
- Próximos passos

**Avaliação:**
| Critério | Peso |
|---|---|
| Pipeline funcional | 40% |
| Qualidade do código | 25% |
| Documentação | 20% |
| Organização | 15% |

## Checklist rápido do revisor

- [ ] PR segue o template
- [ ] Commits com tipos semânticos
- [ ] Entrega atende o objetivo
- [ ] README explica como reproduzir
- [ ] Código organizado e legível
