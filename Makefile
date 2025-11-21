CC = gcc
CFLAGS = -g -Wall -Isrc -Iparser
LDFLAGS = -lfl
TARGET = interpretador
BUILD_DIR = build
C_SOURCES   = $(wildcard src/*.c)
LEXER_SRC   = lexer/lexer.l
PARSER_SRC  = parser/parser.y
PARSER_GEN_C = parser/parser.tab.c
PARSER_GEN_H = parser/parser.tab.h
LEXER_GEN_C  = lexer/lex.yy.c
C_OBJS      = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
PARSER_OBJ  = $(BUILD_DIR)/parser.tab.o
LEXER_OBJ   = $(BUILD_DIR)/lex.yy.o
OBJECTS     = $(C_OBJS) $(PARSER_OBJ) $(LEXER_OBJ)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@echo "===> LINKANDO PROJETO..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "===> PROJETO CONSTRUÍDO: $@"

$(BUILD_DIR)/%.o: src/%.c $(PARSER_GEN_H)
	@echo "===> COMPILANDO (C): $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_OBJ): $(PARSER_GEN_C)
	@echo "===> COMPILANDO (PARSER): $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(LEXER_OBJ): $(LEXER_GEN_C)
	@echo "===> COMPILANDO (LEXER): $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_GEN_C) $(PARSER_GEN_H): $(PARSER_SRC)
	@echo "===> GERANDO PARSER (BISON)..."
	bison -d -o $(PARSER_GEN_C) $(PARSER_SRC)

$(LEXER_GEN_C): $(LEXER_SRC) $(PARSER_GEN_H)
	@echo "===> GERANDO LEXER (FLEX)..."
	flex -o $(LEXER_GEN_C) $(LEXER_SRC)

$(OBJECTS): | $(BUILD_DIR)

$(BUILD_DIR):
	@echo "===> CRIANDO DIRETÓRIO DE BUILD..."
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	@echo "===> LIMPANDO PROJETO..."
	rm -rf $(BUILD_DIR)
	rm -f $(PARSER_GEN_C) $(PARSER_GEN_H) $(LEXER_GEN_C)