# Documentação — Interpretador (Grupo 16)

* **Linguagem do projeto:** C
* **Grupo:** Grupo 16
* **Professor:** `sergioaafreitas` / `sergiofreitas@unb.br`

---

## 📖 Por que usamos C?

* **Prototipagem eficiente:** A linguagem C permite desenvolver e iterar rapidamente estruturas de dados (AST, tabela de símbolos) e componentes do interpretador.
* **Leitura e manutenção:** Sintaxe clara e menor boilerplate facilitam o trabalho em equipe.
* **Bibliotecas e ferramentas:** Integração nativa com Flex e Bison, além de ferramentas padrão de sistema (Make, GDB, Valgrind).
* **Integração com Flex/Bison:** Flex e Bison geram código C otimizado para as etapas léxica e sintática.
* **Educação:** Facilita a compreensão de conceitos de baixo nível como gerenciamento de memória, ponteiros e a estrutura interna de um processo de compilação.

---

## 📋 Visão Geral

Este repositório contém a implementação de um **interpretador** para uma linguagem simples definida pelo grupo. A análise léxica e sintática é feita com **Flex** e **Bison**; a AST, análise semântica e o interpretador estão implementados em **C**.

**Pipeline de Execução:**
```
Código Fonte (.lang) → Lexer (Tokens) → Parser (Gramática) → AST → Verificação Semântica → Interpretação/Execução
```

---

## 📂 Estrutura do Repositório

```text
interpretador-grupo16/
├── build/                    # Artefatos de compilação e executável final (gerado automaticamente)
│   └── interpretador         # Executável principal (gerado pelo make)
├── lexer/
│   ├── lex.yy.c              # Código C gerado automaticamente pelo Flex
│   └── lexer.l               # Especificação léxica (definição de tokens e regras)
├── parser/
│   ├── parser.tab.c          # Código C gerado automaticamente pelo Bison
│   ├── parser.tab.h          # Header com definições de tokens e tipos (gerado pelo Bison)
│   └── parser.y              # Especificação gramatical (regras de produção)
├── src/
│   ├── ast.c                 # Implementação da Árvore Sintática Abstrata
│   ├── ast.h                 # Estruturas de dados e interface da AST
│   ├── interpretador.c       # Motor de execução: avaliação de expressões e execução de comandos
│   ├── main.c                # Ponto de entrada: leitura de arquivo, inicialização e coordenação
│   ├── simbolo.c             # Implementação da Tabela de Símbolos (escopos e variáveis)
│   ├── simbolo.h             # Interface da Tabela de Símbolos
|   └── test_runner.py        # Script Python para orquestração e execução automatizada dos testes
├── tests/                    # Suíte completa de testes automatizados
│   ├── valid/                # Casos de teste válidos (devem executar com sucesso)
│   │   ├── *.lang            # Código fonte dos testes
│   │   └── *.out             # Saída esperada (gabarito) para cada teste
│   └── invalid/              # Casos de teste com erros (devem falhar propositalmente)
│       └── *.lang            # Código fonte com erros sintáticos ou semânticos
├── Makefile                  # Automação completa de build, limpeza e testes
└── README.md                 # Documentação do projeto
```

### Descrição dos Componentes Principais

- **`lexer/lexer.l`**: Define os tokens da linguagem (identificadores, números, palavras-chave, operadores) usando expressões regulares do Flex.
- **`parser/parser.y`**: Define a gramática livre de contexto usando a notação do Bison. Constrói a AST durante o parsing.
- **`src/ast.*`**: Estruturas de dados que representam o programa como uma árvore. Cada nó representa uma construção da linguagem (expressão, comando, declaração).
- **`src/simbolo.*`**: Gerencia variáveis e seus escopos. Implementa pilha de escopos para suportar funções e blocos aninhados.
- **`src/interpretador.c`**: Implementa o "walker" da AST que executa o programa: avalia expressões, executa comandos e gerencia o fluxo de controle.
- **`src/test_runner.py`**: Sistema de testes automatizado que compila o interpretador e valida casos de sucesso e falha.

---

## 🛠️ Pré-requisitos

Para compilar e rodar o projeto, você precisará das seguintes ferramentas instaladas:

* **GCC** (Compilador C - versão 7.0 ou superior recomendada)
* **Flex** (Fast Lexical Analyzer - versão 2.6+)
* **Bison** (GNU Parser Generator - versão 3.0+)
* **Python 3** (Para rodar a suíte de testes - versão 3.6+)
* **Make** (GNU Make para automação de build)

**Instalação no Ubuntu/Debian/WSL:**

```bash
sudo apt-get update
sudo apt-get install build-essential flex bison python3
```

**Instalação no macOS:**

```bash
brew install flex bison make python3
```

**Verificação da instalação:**

```bash
gcc --version
flex --version
bison --version
python3 --version
make --version
```

---

## 🚀 Compilação e Execução

O projeto utiliza um Makefile para automatizar todo o processo de build.

### Compilar o Projeto

Gera o executável `interpretador` na pasta `build/`:

```bash
make
```

O processo de compilação executa automaticamente:
1. Geração do lexer (Flex): `lexer/lex.yy.c`
2. Geração do parser (Bison): `parser/parser.tab.c` e `parser/parser.tab.h`
3. Compilação de todos os arquivos `.c` em `src/`
4. Linkagem final para produzir `build/interpretador`

### Limpar Arquivos Gerados

Remove a pasta `build/` e todos os arquivos gerados automaticamente:

```bash
make clean
```

### Executar um Script Manualmente

Para interpretar um arquivo específico da nossa linguagem:

```bash
./build/interpretador caminho/para/arquivo.lang
```

**Exemplo:**
```bash
./build/interpretador tests/valid/fibonacci.lang
```

### Recompilar Tudo do Zero

Para garantir uma compilação limpa:

```bash
make clean && make
```

---

## 🧪 Sistema de Testes Automatizados

O projeto possui um sistema robusto de **testes de regressão** que garante a qualidade do código e previne bugs em novas funcionalidades.

### Como Rodar os Testes

Execute o comando abaixo para compilar o projeto e rodar todos os testes automaticamente:

```bash
make test
```

O sistema de testes:
- ✅ Compila o interpretador automaticamente
- ✅ Executa todos os casos de teste válidos e inválidos
- ✅ Compara saídas com gabaritos esperados
- ✅ Reporta falhas com detalhes claros
- ✅ Fornece estatísticas de aprovação/falha

### Estrutura e Tipos de Testes

#### 1. **Testes Válidos (`tests/valid/`)**

Contêm programas que **devem executar com sucesso**. O sistema verifica:

- ✅ **Código de saída 0** (execução sem erros)
- ✅ **Saída padrão** corresponde exatamente ao gabarito (`.out`)

**Exemplo de teste válido:**
```
tests/valid/
├── hello_world.lang        # Código fonte do teste
├── hello_world.out         # Saída esperada (gabarito)
├── arithmetic.lang
├── arithmetic.out
├── loops.lang
└── loops.out
```

**Critérios de aprovação:**
- Interpretador retorna exit code 0
- Saída impressa em `stdout` é idêntica ao arquivo `.out` correspondente
- Nenhum erro de segmentação ou vazamento de memória

#### 2. **Testes Inválidos (`tests/invalid/`)**

Contêm programas com **erros propositais** (sintáticos ou semânticos). O sistema verifica:

- ✅ **Código de saída ≠ 0** (interpretador detectou o erro)
- ✅ **Mensagens de erro** são emitidas apropriadamente

**Exemplo de teste inválido:**
```
tests/invalid/
├── syntax_error.lang       # Erro de sintaxe
├── undefined_var.lang      # Variável não declarada
├── type_mismatch.lang      # Erro de tipo
└── division_by_zero.lang   # Erro de runtime
```

**Critérios de aprovação:**
- Interpretador retorna exit code diferente de 0
- Mensagem de erro clara é emitida
- Programa não trava ou causa comportamento indefinido

### Como Adicionar Novos Testes

#### Método 1: Geração Automática de Gabarito (Recomendado)

Para testes válidos, você pode gerar o gabarito automaticamente:

1. **Crie o arquivo `.lang`:**
   ```bash
   nano tests/valid/novo_teste.lang
   ```

2. **Gere o gabarito automaticamente:**
   ```bash
   for f in tests/valid/*.lang; do 
       ./build/interpretador "$f" > "${f%.lang}.out"
   done
   ```

3. **Verifique o gabarito gerado:**
   ```bash
   cat tests/valid/novo_teste.out
   ```

4. **Valide o teste:**
   ```bash
   make test
   ```

#### Método 2: Criação Manual do Gabarito

1. **Crie o arquivo de teste:**
   ```bash
   nano tests/valid/novo_teste.lang
   ```

2. **Execute manualmente e verifique:**
   ```bash
   ./build/interpretador tests/valid/novo_teste.lang
   ```

3. **Crie o gabarito com a saída esperada:**
   ```bash
   echo "saída esperada aqui" > tests/valid/novo_teste.out
   ```

#### Adicionando Testes de Erro

Para testes inválidos (não precisam de gabarito `.out`):

```bash
nano tests/invalid/novo_erro.lang
# Escreva código com erro proposital
make test  # Verifique que o interpretador detecta o erro
```

### Exemplos de Casos de Teste

#### Teste Válido - Fibonacci
```c
// tests/valid/fibonacci.lang
var n = 10;
var a = 0;
var b = 1;
var i = 0;

while (i < n) {
    print(a);
    var temp = a + b;
    a = b;
    b = temp;
    i = i + 1;
}
```

#### Teste Inválido - Variável Não Declarada
```c
// tests/invalid/undefined_var.lang
print(x);  // Erro: x não foi declarada
```

### Saída do Sistema de Testes

```
=================================
Executando Testes Automatizados
=================================

[VALID] hello_world.lang .................. ✓ PASS
[VALID] arithmetic.lang ................... ✓ PASS
[VALID] loops.lang ........................ ✓ PASS
[INVALID] syntax_error.lang ............... ✓ PASS (erro detectado)
[INVALID] undefined_var.lang .............. ✓ PASS (erro detectado)

=================================
Resumo: 5/5 testes aprovados (100%)
=================================
```

---

## 🔤 Tokens, Tipos e Construções da Linguagem

### Tokens Léxicos

O lexer reconhece os seguintes tipos de tokens:

#### **Identificadores e Literais**
| Token | Descrição | Exemplo |
|-------|-----------|---------|
| `IDENT` | Identificador (variável, função) | `x`, `contador`, `calcularSoma` |
| `NUMBER` | Literal numérico inteiro | `42`, `0`, `-15` |
| `FLOAT` | Literal numérico de ponto flutuante | `3.14`, `0.5`, `-2.718` |
| `STRING` | Literal de string | `"Hello"`, `"Mundo"` |
| `TRUE` / `FALSE` | Literais booleanos | `true`, `false` |

#### **Palavras-Chave Reservadas**
```
if      else      while     for       func      return
var     const     print     input     break     continue
```

#### **Operadores Aritméticos**
```
+       -         *         /         %         **
```

#### **Operadores Relacionais**
```
==      !=        <         >         <=        >=
```

#### **Operadores Lógicos**
```
&&      ||        !
```

#### **Operadores de Atribuição**
```
=       +=        -=        *=        /=        %=
```

#### **Delimitadores e Pontuação**
```
;       ,         (         )         {         }         [         ]
```

### Tipos de Dados Suportados

A linguagem suporta os seguintes tipos de dados:

| Tipo | Descrição | Exemplo |
|------|-----------|---------|
| `int` | Números inteiros de 32 bits | `42`, `-15`, `0` |
| `float` | Números de ponto flutuante | `3.14`, `-0.5` |
| `bool` | Booleanos (verdadeiro/falso) | `true`, `false` |
| `string` | Cadeias de caracteres | `"Hello"`, `"Mundo"` |

**Conversões implícitas:**
- `int` → `float` (automática em operações mistas)
- `bool` → `int` (`true` = 1, `false` = 0)

### Construções Sintáticas

#### **1. Declaração de Variáveis**
```c
var x;              // Declaração sem inicialização
var y = 10;         // Declaração com inicialização
var nome = "João";  // String
var pi = 3.14159;   // Float
```

#### **2. Atribuições**
```c
x = 5;              // Atribuição simples
y += 3;             // Equivalente a: y = y + 3
z *= 2;             // Equivalente a: z = z * 2
```

#### **3. Expressões Aritméticas**
```c
var resultado = (a + b) * c - d / 2;
var potencia = base ** expoente;
var resto = x % 3;
```

#### **4. Expressões Lógicas**
```c
var teste = (x > 5) && (y < 10);
var valido = !erro || status;
```

#### **5. Estrutura Condicional (if/else)**
```c
if (x > 0) {
    print("Positivo");
} else if (x < 0) {
    print("Negativo");
} else {
    print("Zero");
}
```

#### **6. Laço While**
```c
var i = 0;
while (i < 10) {
    print(i);
    i = i + 1;
}
```

#### **7. Laço For**
```c
for (var i = 0; i < 10; i = i + 1) {
    print(i);
}
```

#### **8. Funções** *(se implementadas)*
```c
func fibonacci(n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

var resultado = fibonacci(10);
```

#### **9. Entrada e Saída**
```c
print("Digite um número:");
var numero = input();
print("Você digitou: ");
print(numero);
```

### Gramática Simplificada

```bnf
programa    ::= declaracao*

declaracao  ::= declaracao_var | comando

declaracao_var ::= 'var' IDENT ('=' expressao)? ';'

comando     ::= bloco
              | if_stmt
              | while_stmt
              | for_stmt
              | return_stmt
              | print_stmt
              | atribuicao ';'
              | expressao ';'

bloco       ::= '{' comando* '}'

if_stmt     ::= 'if' '(' expressao ')' comando ('else' comando)?

while_stmt  ::= 'while' '(' expressao ')' comando

for_stmt    ::= 'for' '(' declaracao_var expressao ';' atribuicao ')' comando

return_stmt ::= 'return' expressao? ';'

print_stmt  ::= 'print' '(' expressao ')' ';'

atribuicao  ::= IDENT '=' expressao
              | IDENT '+=' expressao
              | IDENT '-=' expressao
              | IDENT '*=' expressao
              | IDENT '/=' expressao

expressao   ::= logica_ou

logica_ou   ::= logica_e ('||' logica_e)*

logica_e    ::= igualdade ('&&' igualdade)*

igualdade   ::= comparacao (('==' | '!=') comparacao)*

comparacao  ::= termo (('<' | '>' | '<=' | '>=') termo)*

termo       ::= fator (('+' | '-') fator)*

fator       ::= unario (('*' | '/' | '%') unario)*

unario      ::= ('!' | '-') unario | potencia

potencia    ::= primario ('**' primario)*

primario    ::= NUMBER | FLOAT | STRING | TRUE | FALSE
              | IDENT
              | '(' expressao ')'
              | chamada_funcao

chamada_funcao ::= IDENT '(' (expressao (',' expressao)*)? ')'
```

---

## 📊 Testes e Boas Práticas

### Estratégias de Teste

* **Unit tests:** Testes isolados para funções específicas (AST, tabela de símbolos, avaliação de expressões)
* **Integration tests:** Testes end-to-end que executam programas completos
* **Regression tests:** Garantem que correções não introduzem novos bugs
* **Error handling tests:** Validam mensagens de erro e recuperação

### Qualidade de Código

* **Mensagens de erro:** Sempre reportar `linha:coluna` e contexto do erro
* **Formatação:** Usar `clang-format` com estilo consistente
* **Documentação:** Comentar estruturas complexas e decisões de design
* **Sanitizers:** Usar `-fsanitize=address,undefined` em desenvolvimento para detectar:
  - Vazamentos de memória
  - Uso após liberação (use-after-free)
  - Buffer overflows
  - Comportamento indefinido

### Comandos de Debug

```bash
# Compilar com símbolos de debug
make DEBUG=1

# Executar com Valgrind para detectar vazamentos
valgrind --leak-check=full ./build/interpretador tests/valid/test.lang

# Usar GDB para debugging interativo
gdb ./build/interpretador
(gdb) run tests/valid/test.lang
```

---

## ⚠️ Tratamento de Erros

### Erros Léxicos e Sintáticos

* **Lexer:** Reporta caracteres inválidos e tokens mal-formados
* **Parser:** Usa recuperação de erros do Bison para reportar múltiplos erros
* **Formato:** `arquivo.lang:linha:coluna: erro: descrição`

**Exemplo:**
```
exemplo.lang:5:8: erro: caractere inválido '@'
exemplo.lang:10:15: erro: esperado ';' após declaração
```

### Erros Semânticos

* Variáveis não declaradas antes do uso
* Redeclaração de variáveis no mesmo escopo
* Incompatibilidade de tipos em operações
* Número incorreto de argumentos em funções
* Retorno fora de função

**Exemplo:**
```
exemplo.lang:15:5: erro: variável 'x' não declarada
exemplo.lang:20:9: erro: não é possível somar 'string' com 'int'
```

### Erros de Runtime

* Divisão por zero
* Acesso a índice inválido de array
* Stack overflow (recursão profunda)
* Operações inválidas (ex: raiz quadrada de negativo)

**Exemplo:**
```
Runtime error em exemplo.lang:25: divisão por zero
```

---

## 📅 Sprints e Cronograma

| Sprint | Objetivos | Entregáveis |
|--------|-----------|-------------|
| **Sprint 1** | Formar equipes, configurar ambiente | Repositório, README, gramática inicial |
| **Sprint 2** | Implementar lexer e parser básico | Lexer completo, primeiras regras do parser, P1 |
| **Sprint 3** | Construir AST e análise semântica | AST funcional, verificação básica de tipos |
| **Sprint 4** | Implementar interpretador | Motor de execução, P2 |
| **Sprint 5** | Otimizações e recursos extras | Interpretador completo, testes integrados |
| **Sprint 6** | Entrega final e documentação | Entrevistas, correções, documentação final |

---

## 🔄 Fluxo de Trabalho com Git

### Estrutura de Branches

* **`main`**: Branch estável, apenas código testado e aprovado
* **`dev`**: Branch de integração para desenvolvimento ativo
* **`feature/<nome>`**: Branches para novas funcionalidades individuais
* **`fix/<nome>`**: Branches para correção de bugs

### Processo de Desenvolvimento

1. **Criar feature branch:**
   ```bash
   git checkout dev
   git pull origin dev
   git checkout -b feature/nova-funcionalidade
   ```

2. **Desenvolver e commitar:**
   ```bash
   git add .
   git commit -m "feat: implementar nova funcionalidade"
   ```

3. **Abrir Pull Request:**
   - Requer revisão de pelo menos 1 membro
   - Todos os testes devem passar
   - Código deve seguir padrões de estilo

4. **Merge em `dev`:**
   ```bash
   git checkout dev
   git merge feature/nova-funcionalidade
   ```

### Convenção de Commits

Seguir o padrão [Conventional Commits](https://www.conventionalcommits.org/):

* `feat:` Nova funcionalidade
* `fix:` Correção de bug
* `docs:` Alterações em documentação
* `test:` Adição ou modificação de testes
* `refactor:` Refatoração de código
* `style:` Formatação, espaços em branco
* `chore:` Tarefas de manutenção

**Exemplos:**
```bash
git commit -m "feat: adicionar suporte a arrays"
git commit -m "fix: corrigir vazamento de memória no parser"
git commit -m "test: adicionar casos de teste para loops"
git commit -m "docs: atualizar README com exemplos de uso"
```

---

## Suporte e Contribuições

* **Issues:** Reportar bugs ou sugerir melhorias através do GitHub Issues
* **Pull Requests:** Contribuições são bem-vindas! Siga o fluxo de trabalho Git
* **Contato:** Entre em contato com o professor ou membros do grupo

---

## Licença

Este projeto é desenvolvido para fins educacionais como parte da disciplina de Compiladores da UnB.
