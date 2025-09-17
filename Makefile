# Makefile (Com TABs Corretos)

# Compilador C
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Isrc -Iparser -g

# Ferramentas
LEX = flex
YACC = bison

# Nome do executável
TARGET = interpretador

# Arquivos fonte
SOURCES = \
	src/main.c \
	src/ast.c \
	src/interpretador.c \
	parser/parser.tab.c \
	lexer/lexer.yy.c

# Arquivos objeto
OBJECTS = $(SOURCES:.c=.o)

# Regra principal
all: $(TARGET)

# Regra de linkagem
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regra de compilação genérica
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para o Parser
parser/parser.tab.c parser/parser.tab.h: parser/parser.y src/ast.h src/simbolo.h
	$(YACC) -d --header=parser/parser.tab.h -o parser/parser.tab.c parser/parser.y

# Regra para o Lexer
lexer/lexer.yy.c: lexer/lexer.l parser/parser.tab.h
	$(LEX) -o lexer/lexer.yy.c lexer/lexer.l

# Regra para limpar
clean:
	rm -f $(TARGET) $(OBJECTS) parser/parser.tab.c parser/parser.tab.h lexer/lexer.yy.c