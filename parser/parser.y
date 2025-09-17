// Arquivo: parser/parser.y

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

int yylex(void);
void yyerror(const char *s);
%}

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

/* ========================================================================= */
/* AQUI COMECA A SECAO DE REGRAS - NOTE O SEPARADOR %%                    */
/* ========================================================================= */
%%

programa:
    | programa linha
    ;

linha:
    expressao NEWLINE   {
                            int resultado = interpretar($1);
                            printf("Resultado: %d\n", resultado);
                            liberar_ast($1);
                        }
    | atribuicao NEWLINE  {
                            int resultado = interpretar($1);
                            printf("Resultado: %d\n", resultado);
                            liberar_ast($1);
                        }
    | NEWLINE             { /* Nao faz nada com uma linha em branco */ }
    | error NEWLINE       { yyerrok; }
    ;

atribuicao:
    ID IGUAL expressao {
        $$ = create_assign_node(create_id_node($1), $3);
    }
;

expressao:
    NUM                 { $$ = create_num_node($1); }
    | ID                { $$ = create_id_node($1); }
    | expressao PLUS expressao   { $$ = create_op_node('+', $1, $3); }
    | expressao MINUS expressao  { $$ = create_op_node('-', $1, $3); }
    | expressao TIMES expressao  { $$ = create_op_node('*', $1, $3); }
    | expressao DIVIDE expressao { $$ = create_op_node('/', $1, $3); }
    | LPAREN expressao RPAREN    { $$ = $2; }
;

/* ========================================================================= */
/* AQUI COMECA A SECAO DE CODIGO C FINAL - NOTE O SEPARADOR %%             */
/* ========================================================================= */
%%

/*
 * Implementacoes de funcoes e definicoes de variaveis globais vem aqui.
 * Isso garante que elas sejam globais e visiveis para o linker.
 */

// Definicoes globais da tabela de simbolos
struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico: %s\n", s);
}

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