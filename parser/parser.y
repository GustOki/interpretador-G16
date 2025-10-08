%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

int yylex(void);
void yyerror(const char *s);
<<<<<<< HEAD
extern int yylineno;
extern int interpret_error;
=======

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
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
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
%token PONTO_VIRGULA // <<< ADICIONADO AQUI
%token NEWLINE PLUS MINUS TIMES DIVIDE IGUAL

// <<< ADICIONADO 'comando' e 'lista_comandos' aqui
%type <no> linha expressao atribuicao comando_if comando lista_comandos

%right IF ELSE
%left GT LT GE LE EQ NE
%left PLUS MINUS
%left TIMES DIVIDE

<<<<<<< HEAD
=======

/* SECAO DE REGRAS */

>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
%%
programa:
    | programa linha
    ;

linha:
<<<<<<< HEAD
    expressao NEWLINE {
                          interpret_error = 0;
                          int resultado = interpretar($1);
                          if (!interpret_error) {
                              printf("Resultado: %d\n", resultado);
                          }
                          liberar_ast($1);
                      }
  | atribuicao NEWLINE {
                          interpret_error = 0;
                          int resultado = interpretar($1);
                          if (!interpret_error) {
                              printf("OK. '%s' agora é %d\n", $1->data.children.left->data.nome, resultado);
                          }
                          liberar_ast($1);
                       }
  | comando_if NEWLINE {
                          interpret_error = 0;
                          interpretar($1);
                          liberar_ast($1);
                       }
  | NEWLINE            { /* Nao faz nada */ }
  | error NEWLINE      { yyerrok; }
  ;

// --- REGRAS ADICIONADAS PARA SUPORTAR MÚLTIPLOS COMANDOS ---
lista_comandos:
    /* a lista de comandos pode ser vazia */ { $$ = NULL; }
  | lista_comandos comando        {
                                    if ($1 == NULL) $$ = $2;
                                    else $$ = create_op_node(';', $1, $2);
                                  }
  ;

comando:
    atribuicao PONTO_VIRGULA   { $$ = $1; }
  | expressao PONTO_VIRGULA    { $$ = $1; }
  | comando_if                 { $$ = $1; }
  ;
// --- FIM DAS REGRAS ADICIONADAS ---

// --- REGRA 'comando_if' MODIFICADA PARA USAR 'lista_comandos' ---
comando_if:
    IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE %prec IFX {
        AstNode* if_node = create_if_node($3, $6, NULL);
        if_node->lineno = yylineno;
        $$ = if_node;
    }
  | IF LPAREN expressao RPAREN LBRACE lista_comandos RBRACE ELSE LBRACE lista_comandos RBRACE {
        AstNode* if_node = create_if_node($3, $6, $10);
        if_node->lineno = yylineno;
        $$ = if_node;
    }
  ;
=======
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
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f


atribuicao:
    ID IGUAL expressao {
        AstNode* left = create_id_node($1);
<<<<<<< HEAD
        left->lineno = yylineno;
        $$ = create_assign_node(left, $3);
        $$->lineno = left->lineno;
=======
        left->lineno = yylineno; /* linha do ID token */
        $$ = create_assign_node(left, $3);
        $$->lineno = left->lineno; /* linha da atribuição = linha do id */
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
    }
;


expressao:
<<<<<<< HEAD
    NUM                 { $$ = create_num_node($1); $$->lineno = yylineno; }
  | ID                { $$ = create_id_node($1); $$->lineno = yylineno; }
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
  | LPAREN expressao RPAREN    { $$ = $2; }
;

%%
/* SECAO DE CODIGO C FINAL */
struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

void yyerror(const char *s) {
    fprintf(stderr, "Linha %d: Erro sintático: %s\n", yylineno, s);
}

=======
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

>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
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
