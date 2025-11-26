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
- **`test_runner.py`**: Sistema de testes automatizado que compila o interpretador e valida casos de sucesso e falha.

---

<<<<<<< HEAD
# 4. Instalação de dependências e comandos úteis

## Flex / Bison (Debian/Ubuntu)

## Pré-requisitos

No Ubuntu/Debian:

=======
## 🛠️ Pré-requisitos

Para compilar e rodar o projeto, você precisará das seguintes ferramentas instaladas:

* **GCC** (Compilador C - versão 7.0 ou superior recomendada)
* **Flex** (Fast Lexical Analyzer - versão 2.6+)
* **Bison** (GNU Parser Generator - versão 3.0+)
* **Python 3** (Para rodar a suíte de testes - versão 3.6+)
* **Make** (GNU Make para automação de build)

**Instalação no Ubuntu/Debian/WSL:**
>>>>>>> dev

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

<<<<<<< HEAD
```makefile
# Makefile para Estrutura de Pastas Organizada
=======
O projeto utiliza um Makefile para automatizar todo o processo de build.

### Compilar o Projeto
>>>>>>> dev

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

#### Teste Válido - Contador Simples
```c
// tests/valid/contador.lang
int i;
for (i = 0; i < 5; i = i + 1) {
    printf("Contador: %d\n", i);
}
```

#### Teste Válido - Cálculo de Média
```c
// tests/valid/media.lang
float nota1 = 8.5;
float nota2 = 7.0;
float nota3 = 9.5;
float media = (nota1 + nota2 + nota3) / 3.0;
printf("Média: %f\n", media);
```

#### Teste Válido - Switch-Case
```c
// tests/valid/switch_test.lang
int dia = 3;
switch (dia) {
    case 1:
        printf("Segunda-feira\n");
        break;
    case 2:
        printf("Terça-feira\n");
        break;
    case 3:
        printf("Quarta-feira\n");
        break;
    default:
        printf("Outro dia\n");
        break;
}
```

#### Teste Inválido - Variável Não Declarada
```c
// tests/invalid/undefined_var.lang
printf("%d\n", x);  // Erro: x não foi declarada
```

#### Teste Inválido - Erro de Sintaxe
```c
// tests/invalid/syntax_error.lang
int x = 10  // Erro: faltando ponto e vírgula
printf("%d\n", x);
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
| Token | Descrição | Padrão/Exemplo |
|-------|-----------|----------------|
| `ID` | Identificador (variável, função) | `[a-zA-Z_][a-zA-Z0-9_]*` → `x`, `contador`, `soma_total` |
| `NUM` | Literal numérico inteiro | `[0-9]+` → `42`, `0`, `123` |
| `FLOAT_NUM` | Literal numérico de ponto flutuante | `[0-9]+\.[0-9]*` → `3.14`, `0.5`, `2.718` |
| `CHAR_LIT` | Literal de caractere | `'[^']'` → `'a'`, `'Z'`, `'9'` |
| `STRING_LIT` | Literal de string | `\"[^\"]*\"` → `"Hello"`, `"Mundo"` |

#### **Palavras-Chave Reservadas**
```
if          else        while       for         do          switch
case        break       default     printf
```

#### **Tipos de Dados**
```
int         float       char        string
```

#### **Operadores Aritméticos**
| Operador | Token | Descrição |
|----------|-------|-----------|
| `+` | `PLUS` | Adição |
| `-` | `MINUS` | Subtração |
| `*` | `TIMES` | Multiplicação |
| `/` | `DIVIDE` | Divisão |

#### **Operadores Relacionais**
| Operador | Token | Descrição |
|----------|-------|-----------|
| `==` | `EQ` | Igualdade |
| `!=` | `NE` | Diferente |
| `<` | `LT` | Menor que |
| `>` | `GT` | Maior que |
| `<=` | `LE` | Menor ou igual |
| `>=` | `GE` | Maior ou igual |

#### **Operadores de Atribuição**
| Operador | Token | Descrição |
|----------|-------|-----------|
| `=` | `IGUAL` | Atribuição simples |

#### **Delimitadores e Pontuação**
| Símbolo | Token | Descrição |
|---------|-------|-----------|
| `;` | `PONTO_VIRGULA` | Fim de instrução |
| `,` | `VIRGULA` | Separador |
| `(` | `LPAREN` | Parêntese esquerdo |
| `)` | `RPAREN` | Parêntese direito |
| `{` | `LBRACE` | Chave esquerda (início de bloco) |
| `}` | `RBRACE` | Chave direita (fim de bloco) |
| `[` | `LBRACKET` | Colchete esquerdo (arrays) |
| `]` | `RBRACKET` | Colchete direito (arrays) |
| `:` | `COLON` | Dois pontos (switch/case) |

#### **Comentários**
O lexer suporta dois tipos de comentários:
- **Comentário de linha:** `// comentário até o fim da linha`
- **Comentário de bloco:** `/* comentário em múltiplas linhas */`

### Tipos de Dados Suportados

A linguagem suporta os seguintes tipos de dados primitivos:

| Tipo | Descrição | Exemplo de Declaração | Exemplo de Literal |
|------|-----------|----------------------|-------------------|
| `int` | Números inteiros | `int x = 42;` | `123`, `0`, `999` |
| `float` | Números de ponto flutuante | `float pi = 3.14;` | `3.14`, `0.5`, `2.0` |
| `char` | Caractere único | `char letra = 'A';` | `'a'`, `'Z'`, `'9'` |
| `string` | Cadeia de caracteres | `string nome = "João";` | `"Hello"`, `"Mundo"` |

**Observações sobre tipos:**
- Inteiros são representados sem ponto decimal
- Floats devem conter pelo menos um dígito após o ponto (ex: `2.0` não apenas `2.`)
- Chars são delimitados por aspas simples (`'`)
- Strings são delimitadas por aspas duplas (`"`)

### Construções Sintáticas

#### **1. Declaração de Variáveis**
```c
int x;                  // Declaração sem inicialização
int y = 10;             // Declaração com inicialização
float pi = 3.14159;     // Float
char letra = 'A';       // Char
string nome = "Maria";  // String
```

#### **2. Atribuições**
```c
x = 5;                  // Atribuição simples
y = x + 10;             // Atribuição com expressão
letra = 'B';            // Atribuição de char
```

#### **3. Expressões Aritméticas**
```c
int resultado = (a + b) * c - d / 2;
float media = (nota1 + nota2) / 2.0;
int resto = x - y * z;
```

#### **4. Expressões Relacionais**
```c
if (x > 0) { /* ... */ }
if (a == b) { /* ... */ }
if (nota >= 7.0) { /* ... */ }
while (i < 10) { /* ... */ }
```

#### **5. Estrutura Condicional (if/else)**
```c
if (x > 0) {
    printf("Positivo");
} else if (x < 0) {
    printf("Negativo");
} else {
    printf("Zero");
}
```

#### **6. Laço While**
```c
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    i = i + 1;
}
```

#### **7. Laço Do-While**
```c
int i = 0;
do {
    printf("%d\n", i);
    i = i + 1;
} while (i < 10);
```

#### **8. Laço For**
```c
int i;
for (i = 0; i < 10; i = i + 1) {
    printf("%d\n", i);
}
```

#### **9. Estrutura Switch-Case**
```c
int opcao = 2;
switch (opcao) {
    case 1:
        printf("Opção 1");
        break;
    case 2:
        printf("Opção 2");
        break;
    case 3:
        printf("Opção 3");
        break;
    default:
        printf("Opção inválida");
        break;
}
```

#### **10. Saída de Dados (printf)**
```c
printf("Hello, World!\n");              // String simples
printf("Valor: %d\n", x);                // Inteiro
printf("Pi: %f\n", pi);                  // Float
printf("Letra: %c\n", letra);            // Char
printf("Nome: %s\n", nome);              // String
printf("Soma: %d\n", a + b);             // Expressão
```

#### **11. Arrays** *(se suportados)*
```c
int numeros[5];                          // Declaração de array
numeros[0] = 10;                         // Atribuição
int valor = numeros[0];                  // Acesso
```

### Gramática Simplificada

```bnf
programa    ::= declaracao*

declaracao  ::= declaracao_var | comando

declaracao_var ::= tipo ID ('[' NUM ']')? ('=' expressao)? ';'

tipo        ::= 'int' | 'float' | 'char' | 'string'

comando     ::= bloco
              | if_stmt
              | while_stmt
              | do_while_stmt
              | for_stmt
              | switch_stmt
              | printf_stmt
              | atribuicao ';'
              | 'break' ';'
              | expressao ';'

bloco       ::= '{' comando* '}'

if_stmt     ::= 'if' '(' expressao ')' comando ('else' comando)?

while_stmt  ::= 'while' '(' expressao ')' comando

do_while_stmt ::= 'do' comando 'while' '(' expressao ')' ';'

for_stmt    ::= 'for' '(' atribuicao ';' expressao ';' atribuicao ')' comando

switch_stmt ::= 'switch' '(' expressao ')' '{' case_list default_case? '}'

case_list   ::= case_stmt+

case_stmt   ::= 'case' NUM ':' comando*

default_case ::= 'default' ':' comando*

printf_stmt ::= 'printf' '(' STRING_LIT (',' expressao)* ')' ';'

atribuicao  ::= ID '=' expressao
              | ID '[' expressao ']' '=' expressao

expressao   ::= igualdade

igualdade   ::= comparacao (('==' | '!=') comparacao)*

comparacao  ::= termo (('<' | '>' | '<=' | '>=') termo)*

termo       ::= fator (('+' | '-') fator)*

fator       ::= unario (('*' | '/') unario)*

unario      ::= ('-') unario | primario

primario    ::= NUM 
              | FLOAT_NUM 
              | CHAR_LIT 
              | STRING_LIT
              | ID
              | ID '[' expressao ']'
              | '(' expressao ')'
```

### Regras de Precedência de Operadores

Da menor para a maior precedência:

1. **Comparação de igualdade:** `==`, `!=`
2. **Comparação relacional:** `<`, `>`, `<=`, `>=`
3. **Adição e subtração:** `+`, `-`
4. **Multiplicação e divisão:** `*`, `/`
5. **Unário:** `-` (negação)
6. **Primários:** Literais, identificadores, parênteses

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

* **Lexer:** Reporta caracteres inválidos e tokens mal-formados com número de linha
* **Parser:** Usa recuperação de erros do Bison para reportar múltiplos erros
* **Formato:** `Linha N: Erro Léxico: descrição`

**Exemplo:**
```
Linha 5: Erro Léxico: Caractere inesperado '@'
Linha 10: erro: esperado ';' após declaração
```

### Erros Semânticos

* Variáveis não declaradas antes do uso
* Redeclaração de variáveis no mesmo escopo
* Incompatibilidade de tipos em operações
* Uso incorreto de `break` fora de loops ou switch

**Exemplo:**
```
erro: variável 'x' não declarada
erro: não é possível somar 'string' com 'int'
```

### Erros de Runtime

* Divisão por zero
* Acesso a índice inválido de array
* Operações inválidas com tipos incompatíveis
* Estouro de pilha em recursões profundas

**Exemplo:**
```
Runtime error: divisão por zero
Runtime error: índice de array fora dos limites
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

<<<<<<< HEAD
* **Repositório:** GitHub (adicionar todos os membros e o professor).
* **Branches:** `main` (estável), `dev` (integração), `feature/<nome>`.
* **Pull Requests:** revisão por pelo menos 1 membro antes de merge em `dev`.
* **Commits:** mensagens claras (`feat: adicionar lexer`; `fix: parser while`).

=======
>>>>>>> dev
