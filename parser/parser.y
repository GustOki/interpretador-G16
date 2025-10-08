%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

int yylex(void);
void yyerror(const char *s);

/* DECLARAÇÕES LEXER */
extern int yylineno; /* linha fornecida pelo lexer */
extern char *yytext; /* texto do token atual */
extern int interpret_error;

/* varivael quem lembra a linha em que o último erro sintático foi detectado */
int last_error_lineno = 0;

/* funcao que salva a linha no momento do erro */
void yyerror(const char *s) {
    last_error_lineno = yylineno;

    if (yytext && yytext[0] != '\0') 
    {
        fprintf(stderr, "Linha %d: Erro sintático: %s perto de '%s'\n",
                last_error_lineno, s, yytext);
    } 
    else 
    {
        fprintf(stderr, "Linha %d: Erro sintático: %s\n", last_error_lineno, s);
    }
}
%}

%define parse.error verbose

%union {
    int valor;
    char *str;
    struct AstNode* no;
}

%token <valor> NUM
%token <str> ID
%type <no> linha expressao atribuicao

%token NEWLINE PLUS MINUS TIMES DIVIDE LPAREN RPAREN IGUAL

%left PLUS MINUS
%left TIMES DIVIDE


/* SECAO DE REGRAS */

%%

programa:
    | programa linha
    ;

linha:
    expressao NEWLINE   {
                            interpret_error = 0; /* zera antes de interpretar */
                            int resultado = interpretar($1);
                            if (!interpret_error) {
                                printf("Resultado: %d\n", resultado);
                            }
                            liberar_ast($1);
                        }
    | atribuicao NEWLINE  {
                            interpret_error = 0;
                            int resultado = interpretar($1);
                            if (!interpret_error) {
                                printf("Resultado: %d\n", resultado);
                            }
                            liberar_ast($1);
                        }
    | NEWLINE             { /* em casos de nao acontecer nada na linha */ }

    | error NEWLINE       { /* mensagem de erro */
                            fprintf(stderr, "Linha %d: erro sintático — recuperado até fim da linha\n", last_error_lineno);
                            yyerrok;
                          }
    ;


atribuicao:
    ID IGUAL expressao {
        AstNode* left = create_id_node($1);
        left->lineno = yylineno; /* linha do ID token */
        $$ = create_assign_node(left, $3);
        $$->lineno = left->lineno; /* linha da atribuição = linha do id */
    }
;


expressao:
    NUM                          { $$ = create_num_node($1); $$->lineno = yylineno; }
    | ID                         { $$ = create_id_node($1); $$->lineno = yylineno; }
    | expressao PLUS expressao   { $$ = create_op_node('+', $1, $3); $$->lineno = $1->lineno; }
    | expressao MINUS expressao  { $$ = create_op_node('-', $1, $3); $$->lineno = $1->lineno; }
    | expressao TIMES expressao  { $$ = create_op_node('*', $1, $3); $$->lineno = $1->lineno; }
    | expressao DIVIDE expressao { $$ = create_op_node('/', $1, $3); $$->lineno = $1->lineno; }
    | LPAREN expressao RPAREN    { $$ = $2; }
;


/* IMPLEMENTACOES EM C*/
%%

// Definicoes globais da tabela de simbolos
struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

int inserir_simbolo(char *nome, int valor) {
    int indice = procurar_simbolo(nome);

    if (indice != -1) 
    {
        tabelaSimbolos[indice].valor = valor;
        return indice;
    }

    if (proximoSimbolo >= MAX_SIMBOLOS) 
    {
        fprintf(stderr, "Tabela de simbolos cheia!\n");
        exit(1);
    }

    strcpy(tabelaSimbolos[proximoSimbolo].nome, nome);
    tabelaSimbolos[proximoSimbolo].valor = valor;
    return proximoSimbolo++;
}

int procurar_simbolo(char *nome) 
    {
    for (int i = 0; i < proximoSimbolo; i++) 
    {
        if (strcmp(tabelaSimbolos[i].nome, nome) == 0) return i;
    }
    return -1;
}
