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

/* --- Raiz global da AST --- */
AstNode* g_ast_root = NULL;

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
%token PLUS MINUS TIMES DIVIDE IGUAL
%token INT FLOAT CHAR STRING PRINTF
%token SWITCH CASE BREAK DEFAULT COLON

%right IF ELSE
%left GT LT GE LE EQ NE
%left PLUS MINUS

/* --- 'programa' é a nossa nova raiz --- */
%type <no> programa stmt expressao atribuicao comando_if lista_comandos declaracao
%type <no> switch_statement case_list case_bloco
%type <tipo> tipo

%%

/* --- A regra 'programa' agora constrói a árvore inteira --- */
programa:
    lista_comandos {
        g_ast_root = $1; // Salva a árvore completa na variável global
        $$ = $1;
    }
    ;

/* --- 'linha:' foi removida --- */
/* Ela era para o REPL; agora 'lista_comandos' é o topo */


/* --- 'lista_comandos' agora permite programas vazios --- */
lista_comandos:
    /* Vazio */           { $$ = NULL; } 
    | lista_comandos stmt { $$ = append_command_list($1, $2); }
    ;

/* ---------- stmt: constrói nós (NÃO interpretam nada aqui) ---------- */
stmt:
    expressao PONTO_VIRGULA { $$ = $1; }
    | atribuicao PONTO_VIRGULA { $$ = $1; }
    | declaracao { $$ = $1; }
    | comando_if { $$ = $1; }
    | PRINTF LPAREN expressao RPAREN PONTO_VIRGULA {
        $$ = create_printf_node($3);
    }
    | switch_statement { $$ = $1; }
    | BREAK PONTO_VIRGULA { $$ = create_break_node(); $$->lineno = yylineno; }
    /* --- Permite blocos { ... } como um statement --- */
    | LBRACE lista_comandos RBRACE { $$ = $2; }
    ;

/* ---------- atribuição, declaração, tipo ---------- */
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

/* ---------- expressões ---------- */
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

/* ---------- if/else ---------- */
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

/* ---------- Switch-Case ---------- */
switch_statement:
    SWITCH LPAREN expressao RPAREN LBRACE case_list RBRACE {
        $$ = create_switch_node($3, $6);
        $$->lineno = yylineno;
    }
    ;

case_list:
    { $$ = NULL; } 
    | case_list case_bloco { $$ = append_case_list($1, $2); }
    ;

case_bloco:
    CASE NUM COLON lista_comandos {
        $$ = create_case_node(create_num_node($2), $4);
        $$->lineno = yylineno;
    }
    | DEFAULT COLON lista_comandos {
        $$ = create_default_node($3);
        $$->lineno = yylineno;
    }
    ;

%%

/* --- resto (implementações auxiliares da tabela de símbolos) --- */
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