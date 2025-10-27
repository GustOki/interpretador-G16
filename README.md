# Documentação Inicial — Interpretador (Grupo 16)

* **Linguagem do projeto:** C
* **Grupo:** Grupo 16
* **Professor:** `sergioaafreitas` / `sergiofreitas@unb.br`

---

# Por que usamos C?

* **Protótipo rápido:** A Linguagrem C permite desenvolver e iterar rapidamente estruturas de dados (AST, tabela de símbolos) e componentes do interpretador.
* **Leitura e manutenção:** sintaxe clara e menor boilerplate facilitam o trabalho em equipe.
* **Bibliotecas e ferramentas:** opção de usar bibliotecas de parsing (ex.: PLY, lark) caso se deseje, além de utilitários para testes e logging.
* **Integração com Flex/Bison:** Flex/Bison podem gerar a etapa léxica/sintática tradicional; Python é ideal para implementar a camada de execução (AST/interpreter) sem precisar compilar todo o backend em C.
* **Educação:** facilita a compreensão de conceitos como AST, análise semântica e avaliação.


## Visão Geral

Este repositório contém a implementação de um **interpretador** para uma linguagem simples definida pelo grupo. A análise léxica e sintática é feita com **Flex** e **Bison**; a AST, análise semântica e o interpretador estão implementados em **C**.

Pipeline alvo:
`arquivo fonte → lexer (Flex) → parser (Bison) → AST → checagem semântica → interpretação/execução`.

---

## Estrutura do repositório (atual)

```
interpretador-grupo16/
├── build/                    # artefatos de build (opcional)
├── lexer/
│   ├── lex.yy.c              # gerado (Flex)
│   └── lexer.l
├── parser/
│   ├── parser.tab.c          # gerado (Bison)
│   ├── parser.tab.h          # gerado (Bison)
│   └── parser.y
├── src/
│   ├── ast.c
│   ├── ast.h
│   ├── interpretador.c
│   ├── main.c
│   └── simbolo.h
├── Makefile
└── README.md
```

---

<<<<<<< HEAD
# 4. Instalação de dependências e comandos úteis

## Flex / Bison (Debian/Ubuntu)
=======
## Pré-requisitos

No Ubuntu/Debian:
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f

```bash
sudo apt-get update
sudo apt-get install build-essential flex bison
```

Opcional (testes/unit):

```bash
sudo apt-get install cmocka
```

Recomendações de desenvolvimento:

* `gcc` ou `clang`
* `clang-format` (padronização de estilo)
* usar sanitizers em builds de debug: `-fsanitize=address,undefined`

---

## Build — Como compilar (Makefile)

Abaixo está o **Makefile** atual usado no projeto (cole-o no `Makefile` se ainda não estiver):

```makefile
# Makefile para Estrutura de Pastas Organizada

# --- Variáveis de Compilação ---
CC = gcc
# Flags do compilador:
# -g para informações de debug (para usar com gdb)
# -Wall para mostrar todos os avisos
# -Isrc para procurar headers na pasta src/
# -Iparser para procurar o parser.tab.h na pasta parser/
CFLAGS = -g -Wall -Isrc -Iparser
# Flags do Linker: -lfl para a biblioteca do Flex
LDFLAGS = -lfl

# --- Variáveis de Projeto ---
TARGET = interpretador
BUILD_DIR = build

# --- Definição dos Arquivos ---
# Fontes .c escritos à mão
C_SOURCES   = $(wildcard src/*.c)
# Arquivos de parser e lexer
LEXER_SRC   = lexer/lexer.l
PARSER_SRC  = parser/parser.y

# Arquivos que serão gerados pelo Flex e Bison
PARSER_GEN_C = parser/parser.tab.c
PARSER_GEN_H = parser/parser.tab.h
LEXER_GEN_C  = lexer/lex.yy.c

# Lista de todos os arquivos objeto que serão criados na pasta build/
C_OBJS      = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
PARSER_OBJ  = $(BUILD_DIR)/parser.tab.o
LEXER_OBJ   = $(BUILD_DIR)/lex.yy.o
OBJECTS     = $(C_OBJS) $(PARSER_OBJ) $(LEXER_OBJ)

# --- Regras do Make ---

# Regra padrão: executada ao digitar "make"
# Depende do executável final.
all: $(BUILD_DIR)/$(TARGET)

# Regra para linkar e criar o executável final
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@echo "===> LINKANDO PROJETO..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "===> PROJETO CONSTRUÍDO: $@"

# Regra para compilar os arquivos .c da pasta src/ para .o na pasta build/
# Depende do header do Bison para garantir a ordem correta de compilação.
$(BUILD_DIR)/%.o: src/%.c $(PARSER_GEN_H)
	@echo "===> COMPILANDO (C): $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o parser.tab.c gerado
$(PARSER_OBJ): $(PARSER_GEN_C)
	@echo "===> COMPILANDO (PARSER): $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o lex.yy.c gerado
$(LEXER_OBJ): $(LEXER_GEN_C)
	@echo "===> COMPILANDO (LEXER): $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para gerar os arquivos do Parser (.tab.c e .tab.h) a partir do .y
$(PARSER_GEN_C) $(PARSER_GEN_H): $(PARSER_SRC)
	@echo "===> GERANDO PARSER (BISON)..."
	bison -d -o $(PARSER_GEN_C) $(PARSER_SRC)

# Regra para gerar o arquivo do Lexer (.yy.c) a partir do .l
# Depende do header do Bison, garantindo que o Bison rode primeiro.
$(LEXER_GEN_C): $(LEXER_SRC) $(PARSER_GEN_H)
	@echo "===> GERANDO LEXER (FLEX)..."
	flex -o $(LEXER_GEN_C) $(LEXER_SRC)

# Regra para garantir que a pasta build/ exista antes de criar arquivos .o nela
$(OBJECTS): | $(BUILD_DIR)

$(BUILD_DIR):
	@echo "===> CRIANDO DIRETÓRIO DE BUILD..."
	mkdir -p $(BUILD_DIR)

# Regra para limpar todos os arquivos gerados
.PHONY: clean
clean:
	@echo "===> LIMPANDO PROJETO..."
	rm -rf $(BUILD_DIR)
	rm -f $(PARSER_GEN_C) $(PARSER_GEN_H) $(LEXER_GEN_C)
```

### Comandos Make mais usados

```bash
# Compilar tudo (regra padrão 'all')
make

# Ou explicitamente
make all

# Limpar artefatos gerados
make clean
```

**O executável final** ficará em `build/interpretador` (conforme `BUILD_DIR` e `TARGET`).

---

## Uso — Como rodar

Supondo que `make` já foi executado com sucesso:

```bash
./build/interpretador caminhos/para/exemplo.lang
```

Observações:

* Verifique `src/main.c` para confirmar o formato de invocação (argumentos esperados).
* Se o projeto não receber um arquivo por argumento, talvez rode em modo interativo — veja `main.c`.

---

## Tokens, tipos e construções iniciais

**Tokens (exemplos):**
`IDENT`, `NUMBER`, `FLOAT`, `STRING`, palavras-chave (`if`, `else`, `while`, `func`, `return`, `var`, `print`), operadores (`+ - * / = == != < > <= >= && ||`), delimitadores (`; , ( ) { }`).

**Tipos iniciais (sugeridos):** `int`, `float` (opcional), `bool`, `string`.  
**Construções básicas:** atribuições, expressões aritméticas e lógicas, `if/else`, `while`, `print`. Funções podem ser adicionadas nas sprints posteriores.

---

## Testes e boas práticas

* **Unit tests:** escrever testes para AST, semântica e runtime (sugestão: `cmocka`/`CUnit`).
* **Integration tests:** criar scripts que rodem o interpretador sobre exemplos e comparem saída.
* **Mensagens de erro:** reportar `linha:coluna` e fornecer contexto.
* **Estilo:** usar `clang-format`; mantenham regras comuns no time.
* **Sanitizers:** use `-fsanitize=address,undefined` em builds de desenvolvimento para detectar vazamentos/uso inválido de memória.

---

## Erros e reporting

* **Lexer/Parser:** reportem a posição do erro e trecho de código; considerem usar a recuperação de erros do Bison para testes de robustez.
* **Semântica:** verificação de variáveis não declaradas, coerência mínima de tipos, número de parâmetros em funções (se houver).
* **Runtime:** mensagens claras para erros de execução (divisão por zero, índice inválido, etc.).

---

## Sprints / Cronograma (resumo)

As sprints previstas são:

* **Sprint 1** — formar equipes, configurar ambiente, definir linguagem e gramática inicial.
* **Sprint 2** — concluir lexer e primeiras regras do parser; preparar P1.
* **Sprint 3** — criar AST, iniciar análise semântica e melhorar mensagens de erro.
* **Sprint 4** — implementar interpretador que percorre a AST; preparar P2.
* **Sprint 5** — otimizações, recursos extras, testes integrados e entrega final.
* **Sprint 6** — entrevistas finais com o professor, correções e documentação final.

---

# Fluxo de trabalho com Git

* **Repositório:** GitHub (adicionar todos os membros e o professor).
* **Branches:** `main` (estável), `dev` (integração), `feature/<nome>`.
* **Pull Requests:** revisão por pelo menos 1 membro antes de merge em `dev`.
* **Commits:** mensagens claras (`feat: adicionar lexer`; `fix: parser while`).
