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