%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIMBOLOS 100
struct simbolo {
    char nome[50];
    int valor;
};

struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

/* flag de erro semântico na linha atual */
int hadError = 0;

int inserir_simbolo(char *nome, int valor) {
    if (proximoSimbolo >= MAX_SIMBOLOS) {
        fprintf(stderr, "Erro: Tabela de símbolos cheia!\n");
        exit(1);
    }
    strncpy(tabelaSimbolos[proximoSimbolo].nome, nome, sizeof(tabelaSimbolos[proximoSimbolo].nome)-1);
    tabelaSimbolos[proximoSimbolo].nome[sizeof(tabelaSimbolos[proximoSimbolo].nome)-1] = '\0';
    tabelaSimbolos[proximoSimbolo].valor = valor;
    return proximoSimbolo++;
}

int procurar_simbolo(char *nome) {
    for (int i = 0; i < proximoSimbolo; i++) {
        if (strcmp(tabelaSimbolos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/* evitar warnings de símbolos do lexer */
int yylex(void);
void yyerror(const char *s);

/* acessar a linha do lexer */
extern int yylineno;
/* obter texto do token atual (do lexer) */
extern char *yytext;
%}

/* Tipo dos valores semânticos */
%union {
    int valor;
    char *str;
}

%token <valor> NUM
%type <valor> expressao
%token NEWLINE
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN
%token <str> ID
%token IGUAL

/* mensagens de erro mais detalhadas */
%define parse.error detailed

/* precedências */
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc LPAREN RPAREN

%%
programa:
    /* vazio */
    | programa linha
    ;

linha:
    expressao NEWLINE {
        if (!hadError) {
            printf("Resultado: %d\n", $1);
        }
        hadError = 0; /* reset para próxima linha */
    }
    | atribuicao NEWLINE {
        hadError = 0;
    }
    | NEWLINE
    | error NEWLINE {
        fprintf(stderr, "Linha %d: Erro sintático — recuperado até fim da linha\n", yylineno);
        yyerrok; /* limpa o estado de erro do parser */
        hadError = 0;
    }
    ;

atribuicao:
    ID IGUAL expressao {
        /* $1 é strdup() no lexer; copiamos para a tabela e liberamos */
        int indice = procurar_simbolo($1);
        if (indice == -1) {
            inserir_simbolo($1, $3);
        } else {
            tabelaSimbolos[indice].valor = $3;
        }
        free($1); /* evita memory leak */
    }
    ;

expressao:
    NUM { $$ = $1; }

  | ID {
        int indice = procurar_simbolo($1);
        if (indice == -1) {
            fprintf(stderr, "Linha %d: Erro semântico: Variável '%s' não declarada\n", yylineno, $1);
            hadError = 1;
            $$ = 0; /* valor default para seguir a análise */
        } else {
            $$ = tabelaSimbolos[indice].valor;
        }
        free($1);
    }

  | expressao PLUS expressao  { $$ = $1 + $3; }
  | expressao MINUS expressao { $$ = $1 - $3; } /* CORREÇÃO: $3 em vez de 3 */
  | expressao TIMES expressao { $$ = $1 * $3; }
  | expressao DIVIDE expressao {
        if ($3 == 0) {
            fprintf(stderr, "Linha %d: Erro semântico: divisão por zero\n", yylineno);
            hadError = 1;
            $$ = 0; /* evita crash, continua a análise */
        } else {
            $$ = $1 / $3;
        }
    }
  | LPAREN expressao RPAREN { $$ = $2; }
  ;
%%

void yyerror(const char *s) {
    /* yytext vem do scanner; yylineno também */
    fprintf(stderr, "Linha %d: Erro sintático: %s perto de '%s'\n", yylineno, s, yytext ? yytext : "");
}

int main(void) {
    while (1) {
        printf("Digite uma expressao: ");
        fflush(stdout);
        yyparse();
    }
    return 0;
}