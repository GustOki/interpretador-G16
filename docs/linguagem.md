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