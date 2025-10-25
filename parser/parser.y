%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"


void yyerror(const char *s);
void interpretar_printf(struct AstNode* expr);

extern int yylineno;
extern char *yytext;

int yylex(void);
int last_error_lineno = 0;
int interpret_error = 0;

void yyerror(const char *s) {
    last_error_lineno = yylineno;
    if (yytext && yytext[0] != '\0') {
        fprintf(stderr, "Linha %d: Erro sintático: %s perto de '%s'\n",
                last_error_lineno, s, yytext);
    } else {
        fprintf(stderr, "Linha %d: Erro sintático: %s\n", last_error_lineno, s);
    }
}
%}

%define parse.error verbose

%union {
    int valor;
    char *str;
    struct AstNode* no;
    int tipo;
}

%token <valor> NUM
%token <str> ID
%token IF ELSE LBRACE RBRACE LPAREN RPAREN
%token EQ NE LT GT LE GE
%token PONTO_VIRGULA
%token NEWLINE PLUS MINUS TIMES DIVIDE IGUAL
%token INT FLOAT CHAR STRING PRINTF

%right IF ELSE
%left GT LT GE LE EQ NE
%left PLUS MINUS

%type <no> linha expressao atribuicao comando_if lista_comandos declaracao
%type <tipo> tipo

%%

programa:
    | programa linha
    ;

linha:
      expressao PONTO_VIRGULA {
          interpret_error = 0;
          int resultado = interpretar($1);
          if (!interpret_error) {
              printf("Resultado: %d\n", resultado);
          }
          liberar_ast($1);
          $$ = NULL;
      }
    | atribuicao PONTO_VIRGULA {
          interpret_error = 0;
          int resultado = interpretar($1);
          if (!interpret_error) {
              printf("Resultado: %d\n", resultado);
          }
          liberar_ast($1);
          $$ = NULL;
      }
    | declaracao {
          interpretar($1);
          liberar_ast($1);
          $$ = NULL;
      }
    | comando_if {
          interpretar($1);
          liberar_ast($1);
          $$ = NULL;
      }
    | PRINTF LPAREN expressao RPAREN PONTO_VIRGULA {
          interpretar_printf($3);
          liberar_ast($3);
          $$ = NULL;
      }
    | NEWLINE { $$ = NULL; }
    | error NEWLINE {
          fprintf(stderr, "Linha %d: erro sintático — recuperado até fim da linha\n", last_error_lineno);
          yyerrok;
          $$ = NULL;
      }
    ;

atribuicao:
      ID IGUAL expressao {
          AstNode* left = create_id_node($1);
          left->lineno = yylineno;
          $$ = create_assign_node(left, $3);
          $$->lineno = left->lineno;
      }
    ;

declaracao:
      tipo ID PONTO_VIRGULA {
          $$ = create_var_decl_node($1, $2, NULL);
      }
    | tipo ID IGUAL expressao PONTO_VIRGULA {
          $$ = create_var_decl_node($1, $2, $4);
      }
    ;

tipo:
      INT     { $$ = TIPO_INT; }
    | FLOAT   { $$ = TIPO_FLOAT; }
    | CHAR    { $$ = TIPO_CHAR; }
    | STRING  { $$ = TIPO_STRING; }
    ;

expressao:
      NUM { $$ = create_num_node($1); $$->lineno = yylineno; }
    | ID  { $$ = create_id_node($1); $$->lineno = yylineno; }
    | expressao PLUS expressao   { $$ = create_op_node('+', $1, $3); $$->lineno = $1->lineno; }
    | expressao MINUS expressao  { $$ = create_op_node('-', $1, $3); $$->lineno = $1->lineno; }
    | expressao TIMES expressao  { $$ = create_op_node('*', $1, $3); $$->lineno = $1->lineno; }
    | expressao DIVIDE expressao { $$ = create_op_node('/', $1, $3); $$->lineno = $1->lineno; }
    | LPAREN expressao RPAREN    { $$ = $2; }
    | expressao LT expressao     { $$ = create_op_node('<', $1, $3); $$->lineno = $1->lineno; }
    | expressao GT expressao     { $$ = create_op_node('>', $1, $3); $$->lineno = $1->lineno; }
    | expressao LE expressao     { $$ = create_op_node('L', $1, $3); $$->lineno = $1->lineno; }
    | expressao GE expressao     { $$ = create_op_node('G', $1, $3); $$->lineno = $1->lineno; }
    | expressao EQ expressao     { $$ = create_op_node('E', $1, $3); $$->lineno = $1->lineno; }
    | expressao NE expressao     { $$ = create_op_node('N', $1, $3); $$->lineno = $1->lineno; }
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
      linha { $$ = $1; }
    | lista_comandos linha { $$ = append_command_list($1, $2); }
    ;

%%

#define MAX_SIMBOLOS 1000

struct simbolo {
    char nome[64];
    int valor;
};

struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

int procurar_simbolo(char *nome);

int inserir_simbolo(char *nome, int valor) {
    int indice = procurar_simbolo(nome);
    if (indice != -1) {
        tabelaSimbolos[indice].valor = valor;
        return indice;
    }
    if (proximoSimbolo >= MAX_SIMBOLOS) {
        fprintf(stderr, "Tabela de simbolos cheia!\n");
        exit(1);
    }
    strcpy(tabelaSimbolos[proximoSimbolo].nome, nome);
    tabelaSimbolos[proximoSimbolo].valor = valor;
    return proximoSimbolo++;
}

int procurar_simbolo(char *nome) {
    for (int i = 0; i < proximoSimbolo; i++) {
        if (strcmp(tabelaSimbolos[i].nome, nome) == 0) return i;
    }
    return -1;
}
