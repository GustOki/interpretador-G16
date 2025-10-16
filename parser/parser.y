// Arquivo: parser/parser.y (VERSÃO FINAL E PADRONIZADA)

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

int yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern int interpret_error;
%}

%define parse.error verbose

%union {
    int valor;
    char *str;
    struct AstNode* no;
}

%token <valor> NUM
%token <str> ID

// Tokens importantes que precisam de nomes
%token IF ELSE LBRACE RBRACE LPAREN RPAREN
%token EQ NE LT GT LE GE
%token PONTO_VIRGULA

%type <no> linha expressao atribuicao comando_if comando lista_comandos

%token NEWLINE PLUS MINUS TIMES DIVIDE IGUAL

%nonassoc IFX
%nonassoc ELSE
%left GT LT GE LE EQ NE
%left PLUS MINUS
%left TIMES DIVIDE

%%
programa:
    | programa linha
    ;

linha:
    NEWLINE { /* ignora linha vazia */ }
  | comando NEWLINE {
        interpret_error = 0;
        int resultado = interpretar($1);
        if ($1 && !interpret_error) {
            switch ($1->type) {
                case NODE_TYPE_ASSIGN:
                    printf("OK. Variável '%s' agora é %d\n", $1->data.children.left->data.nome, resultado);
                    break;
                case NODE_TYPE_OP:
                case NODE_TYPE_NUM:
                case NODE_TYPE_ID:
                    printf("Resultado: %d\n", resultado);
                    break;
                case NODE_TYPE_IF:
                    break;
            }
        }
        liberar_ast($1);
    }
  | error NEWLINE   { yyerrok; }
  ;

lista_comandos:
    /* a lista de comandos pode ser vazia */ { $$ = NULL; }
  | lista_comandos comando        {
                                    if ($1 == NULL) $$ = $2;
                                    else $$ = create_op_node(';', $1, $2);
                                  }
  ;

comando:
    expressao PONTO_VIRGULA    { $$ = $1; }
  | atribuicao PONTO_VIRGULA   { $$ = $1; }
  | comando_if                 { $$ = $1; }
  ;

comando_if:
    // <<< CORREÇÃO AQUI: Usando LPAREN e RPAREN em vez de '(' e ')'
    IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE %prec IFX {
        $$ = create_if_node($3, $6, NULL);
        $$->lineno = yylineno;
    }
  | IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE ELSE LBRACE lista_comandos RBRACE {
        $$ = create_if_node($3, $6, $10);
        $$->lineno = yylineno;
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

expressao:
    NUM { $$ = create_num_node($1); $$->lineno = yylineno; }
  | ID  { $$ = create_id_node($1); $$->lineno = yylineno; }
  | expressao PLUS expressao   { $$ = create_op_node('+', $1, $3); $$->lineno = $1->lineno; }
  | expressao MINUS expressao  { $$ = create_op_node('-', $1, $3); $$->lineno = $1->lineno; }
  | expressao TIMES expressao  { $$ = create_op_node('*', $1, $3); $$->lineno = $1->lineno; }
  | expressao DIVIDE expressao { $$ = create_op_node('/', $1, $3); $$->lineno = $1->lineno; }
  | expressao GT expressao     { $$ = create_op_node('>', $1, $3); $$->lineno = $1->lineno; }
  | expressao LT expressao     { $$ = create_op_node('<', $1, $3); $$->lineno = $1->lineno; }
  | expressao GE expressao     { $$ = create_op_node(1, $1, $3); $$->lineno = $1->lineno; }
  | expressao LE expressao     { $$ = create_op_node(2, $1, $3); $$->lineno = $1->lineno; }
  | expressao EQ expressao     { $$ = create_op_node(3, $1, $3); $$->lineno = $1->lineno; }
  | expressao NE expressao     { $$ = create_op_node(4, $1, $3); $$->lineno = $1->lineno; }
  // <<< CORREÇÃO AQUI: Usando LPAREN e RPAREN em vez de '(' e ')'
  | LPAREN expressao RPAREN    { $$ = $2; }
;

%%
/* SEÇÃO DE CÓDIGO C FINAL */
struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

void yyerror(const char *s) {
    fprintf(stderr, "Erro na linha %d: erro de sintaxe: %s\n", yylineno, s);
}
int inserir_simbolo(char *nome, int valor) {
    int indice = procurar_simbolo(nome);
    if (indice != -1) { tabelaSimbolos[indice].valor = valor; return indice; }
    if (proximoSimbolo >= MAX_SIMBOLOS) { fprintf(stderr, "Tabela de simbolos cheia!\n"); exit(1); }
    strcpy(tabelaSimbolos[proximoSimbolo].nome, nome);
    tabelaSimbolos[proximoSimbolo].valor = valor;
    return proximoSimbolo++;
}
int procurar_simbolo(char *nome) {
    for (int i = 0; i < proximoSimbolo; i++) { if (strcmp(tabelaSimbolos[i].nome, nome) == 0) return i; }
    return -1;
}