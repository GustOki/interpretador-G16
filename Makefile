# Makefile (Versão Robusta)

# Compilador C
CC = gcc

# Opções de compilação: 
# -Wall (todos warnings)
# -Isrc (inclui a pasta src para achar ast.h)
# -Iparser (inclui a pasta parser para achar parser.tab.h)
# -g (adiciona símbolos de debug, útil para depurar)
CFLAGS = -Wall -Isrc -Iparser -g

# Ferramentas
LEX = flex
YACC = bison

# Nome do executável
TARGET = interpretador

# Arquivos fonte de C (gerados e escritos)
SOURCES = \
	src/main.c \
	src/ast.c \
	src/interpretador.c \
	parser/parser.tab.c \
	lexer/lexer.yy.c

# Arquivos objeto correspondentes
OBJECTS = $(SOURCES:.c=.o)

# Regra principal: o que fazer quando digitar "make"
all: $(TARGET)

# Regra de linkagem: como criar o executável final a partir dos objetos
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regra de compilação genérica: como transformar qualquer .c em .o
# O $< representa o pré-requisito (o arquivo .c)
# O $@ representa o alvo (o arquivo .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- REGRAS DE GERAÇÃO (A PARTE MAIS IMPORTANTE) ---

# Regra de dependência para o Parser:
# Alvo: os dois arquivos que o Bison deve criar
# Pré-requisitos: o que o Bison precisa para trabalhar
parser/parser.tab.c parser/parser.tab.h: parser/parser.y src/ast.h
	# Comando para o Bison:
	# -d: cria o arquivo .h
	# --header=...: especifica EXATAMENTE onde criar o .h
	# -o ...: especifica EXATAMENTE onde criar o .c
	$(YACC) -d --header=parser/parser.tab.h -o parser/parser.tab.c parser/parser.y

# Regra de dependência para o Lexer:
# Alvo: o arquivo que o Flex vai criar
# Pré-requisitos: o que o Flex precisa para trabalhar
lexer/lexer.yy.c: lexer/lexer.l parser/parser.tab.h
	# Comando para o Flex
	$(LEX) -o lexer/lexer.yy.c lexer/lexer.l

# Regra para limpar os arquivos gerados (muito útil para forçar uma reconstrução limpa)
clean:
	rm -f $(TARGET) $(OBJECTS) parser/parser.tab.c parser/parser.tab.h lexer/lexer.yy.c