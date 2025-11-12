%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"    
#include "simbolo.h" // Inclui as definições e protótipos da tabela

int yylex(void);

/* --- Variáveis Globais e yyerror --- */
extern int yylineno;
extern char *yytext;
int interpret_error = 0;
int last_error_lineno = 0;

/* --- Raiz global da AST --- */
AstNode* g_ast_root = NULL; // <<< DECLARAÇÃO ADICIONADA AQUI

void yyerror(const char *s) {
    last_error_lineno = yylineno;
    if (yytext && yytext[0] != '\0' && yytext[0] != '\n') {
        fprintf(stderr, "Linha %d: Erro Sintático: %s perto de '%s'\n",
                last_error_lineno, s, yytext);
    } else {
        fprintf(stderr, "Linha %d: Erro Sintático: %s (possivelmente no final da entrada)\n", last_error_lineno, s);
    }
}

// Protótipos das funções auxiliares (que estão no final deste arquivo)
AstNode* append_command_list(AstNode* list, AstNode* command);
AstNode* append_case_list(AstNode* list, AstNode* case_node);

// Definição dos tipos (assumindo que estão em simbolo.h ou ast.h)
#ifndef TIPO_INT
#define TIPO_INT 1
#define TIPO_FLOAT 2
#define TIPO_CHAR 3
#define TIPO_STRING 4
#endif

%}

%define parse.error verbose

%union {
    int valor;
    float fval;        
    char cval;
    char *str;
    struct AstNode* no;
    int tipo;
}

%token <valor> NUM
%token <fval> FLOAT_NUM
%token <cval> CHAR_LIT
%token <str> STRING_LIT
%token <str> ID
%token IF ELSE LBRACE RBRACE LPAREN RPAREN
%token EQ NE LT GT LE GE
%token PONTO_VIRGULA
%token PLUS MINUS TIMES DIVIDE IGUAL
%token INT FLOAT CHAR STRING PRINTF
%token SWITCH CASE BREAK DEFAULT COLON
%token DO WHILE
%token FOR
%token LBRACKET RBRACKET  
%token VIRGULA            

%type <no> programa stmt expressao comando_if lista_comandos declaracao
%type <no> switch_statement case_list case_bloco
%type <tipo> tipo
%type <no> comando_do_while
%type <no> comando_while
%type <no> comando_for
%type <no> comando_printf
%type <no> lista_valores  

%nonassoc IFX
%nonassoc ELSE
%right UMINUS
%right IGUAL  
%left GT LT GE LE EQ NE 
%left PLUS MINUS
%left TIMES DIVIDE 

%%


programa:
    lista_comandos { $$ = $1; g_ast_root = $1; } 
    ;

lista_comandos:
    /* Vazio */           { $$ = NULL; }
    | lista_comandos stmt { $$ = append_command_list($1, $2); }
    ;

stmt:
    expressao PONTO_VIRGULA     { $$ = $1; }
    | declaracao                { $$ = $1; }
    | comando_if                { $$ = $1; }
    | comando_while             { $$ = $1; }
    | comando_do_while          { $$ = $1; }
    | comando_printf            { $$ = $1; }
    | comando_for               { $$ = $1; } 
    | switch_statement          { $$ = $1; }
    | BREAK PONTO_VIRGULA       { $$ = create_break_node(yylineno); }
    | LBRACE lista_comandos RBRACE { $$ = $2; }
    ;

comando_printf:
    PRINTF LPAREN expressao RPAREN PONTO_VIRGULA {
        $$ = create_printf_node($3, yylineno);
    }
    ;

comando_while:
    WHILE LPAREN expressao RPAREN LBRACE lista_comandos RBRACE {
        $$ = create_while_node($3, $6, yylineno);
    }
    ;

comando_do_while:
    DO LBRACE lista_comandos RBRACE WHILE LPAREN expressao RPAREN PONTO_VIRGULA {
        $$ = create_do_while_node($3, $7, yylineno);
    }
    ;
comando_for:
    FOR LPAREN 
        expressao PONTO_VIRGULA     
        expressao PONTO_VIRGULA      
        expressao                   
    RPAREN 
    LBRACE lista_comandos RBRACE {  
        $$ = create_for_node($3, $5, $7, $10, yylineno);
    }
    ; 

declaracao:
    tipo ID PONTO_VIRGULA {
        $$ = create_var_decl_node($1, $2, NULL, yylineno);
    }
    | tipo ID IGUAL expressao PONTO_VIRGULA {
        $$ = create_var_decl_node($1, $2, $4, yylineno);
    }
    | tipo ID LBRACKET NUM RBRACKET PONTO_VIRGULA {
        $$ = create_array_decl_node($1, $2, $4, NULL, yylineno);
    }
    | tipo ID LBRACKET NUM RBRACKET IGUAL LBRACE lista_valores RBRACE PONTO_VIRGULA {
        $$ = create_array_decl_node($1, $2, $4, $8, yylineno);
    }
    ;

lista_valores:
    expressao {
        $$ = $1;
    }
    | lista_valores VIRGULA expressao {
        $$ = create_op_node(',', $1, $3, $1->lineno);
    }
    ;

tipo:
     INT      { $$ = TIPO_INT; }
    | FLOAT    { $$ = TIPO_FLOAT; }
    | CHAR     { $$ = TIPO_CHAR; }
    | STRING   { $$ = TIPO_STRING; }
    ;

expressao:
    NUM { $$ = create_num_node($1, yylineno); }
    | FLOAT_NUM { $$ = create_float_node($1, yylineno); }
    | CHAR_LIT { $$ = create_char_node($1, yylineno); }
    | STRING_LIT { $$ = create_string_node($1, yylineno); } 
    | ID  { $$ = create_id_node($1, yylineno); }
    | ID LBRACKET expressao RBRACKET {  
        $$ = create_array_access_node($1, $3, yylineno);
    }
    | ID LBRACKET expressao RBRACKET IGUAL expressao {  
        AstNode* access = create_array_access_node($1, $3, yylineno);
        $$ = create_assign_node(access, $6, yylineno);
    }
    | expressao PLUS expressao   { $$ = create_op_node('+', $1, $3, $1->lineno); }
    | expressao MINUS expressao  { $$ = create_op_node('-', $1, $3, $1->lineno); }
    | expressao TIMES expressao  { $$ = create_op_node('*', $1, $3, $1->lineno); }
    | expressao DIVIDE expressao { $$ = create_op_node('/', $1, $3, $1->lineno); }
    | LPAREN expressao RPAREN    { $$ = $2; }
    | expressao LT expressao     { $$ = create_op_node('<', $1, $3, $1->lineno); }
    | expressao GT expressao     { $$ = create_op_node('>', $1, $3, $1->lineno); }
    | expressao LE expressao     { $$ = create_op_node('L', $1, $3, $1->lineno); }
    | expressao GE expressao     { $$ = create_op_node('G', $1, $3, $1->lineno); }
    | expressao EQ expressao     { $$ = create_op_node('E', $1, $3, $1->lineno); }
    | expressao NE expressao     { $$ = create_op_node('N', $1, $3, $1->lineno); }
    | MINUS expressao %prec UMINUS {
          AstNode* zero = create_num_node(0, yylineno);
          $$ = create_op_node('-', zero, $2, yylineno);
      }
    | ID IGUAL expressao {
        AstNode* left = create_id_node($1, yylineno);
        $$ = create_assign_node(left, $3, yylineno);
    }
    ;

comando_if:
    IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE %prec IFX {
        $$ = create_if_node($3, $6, NULL, yylineno);
    }
    | IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE ELSE LBRACE lista_comandos RBRACE {
        $$ = create_if_node($3, $6, $10, yylineno);
    }
    ;

switch_statement:
    SWITCH LPAREN expressao RPAREN LBRACE case_list RBRACE {
        $$ = create_switch_node($3, $6, yylineno);
    }
    ;

case_list:
    /* Vazio */           { $$ = NULL; }
    | case_list case_bloco { $$ = append_case_list($1, $2); }
    ;

case_bloco:
    CASE NUM COLON lista_comandos {
        AstNode* num_node = create_num_node($2, yylineno);
        $$ = create_case_node(num_node, $4, yylineno);
    }
    | DEFAULT COLON lista_comandos {
        $$ = create_default_node($3, yylineno);
    }
    ;

%%
