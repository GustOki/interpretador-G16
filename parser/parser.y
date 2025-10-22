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

/* varivael quem lembra a linha em que o último erro sintático foi detectado */
int last_error_lineno = 0;
int interpret_error = 0;


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

%token IF ELSE LBRACE RBRACE LPAREN RPAREN
%token EQ NE LT GT LE GE
%token PONTO_VIRGULA 
%token NEWLINE PLUS MINUS TIMES DIVIDE IGUAL

%type <no> linha expressao atribuicao comando_if comando lista_comandos

%right IF ELSE
%left GT LT GE LE EQ NE
%left PLUS MINUS


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
    | comando_if NEWLINE {
                            interpret_error = 0;
                            interpretar($1);
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
comando:
    atribuicao PONTO_VIRGULA  { $$ = $1; }
    | comando_if              { $$ = $1; }
    ;

comando_if:
    IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE {
        $$ = create_if_node($3, $6, NULL);
        $$->lineno = yylineno;
    }
    | IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE ELSE LBRACE lista_comandos RBRACE {
        $$ = create_if_node($3, $6, $10);
        $$->lineno = yylineno;
    }
    ;

lista_comandos:
    comando                      { $$ = $1; }
    | lista_comandos comando     { $$ = $2; }
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
