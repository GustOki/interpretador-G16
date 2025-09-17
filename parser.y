// Arquivo: parser/parser.y (COM A CORREÇÃO FINAL)

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h" // Contém os 'extern' e a definição da struct

int yylex(void);
void yyerror(const char *s);

// A variável ast_root não é mais necessária para esta lógica
// AstNode *ast_root = NULL;
%}

/* As definições das variáveis globais foram MOVIDAS para o final do arquivo */

%union {
    int valor;
    char *str;
    struct AstNode* no; 
}

%token <valor> NUM
%token <str> ID
%type <no> expressao atribuicao

%token NEWLINE PLUS MINUS TIMES DIVIDE LPAREN RPAREN IGUAL

%left PLUS MINUS
%left TIMES DIVIDE

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
    | NEWLINE             { /* Não faz nada */ }
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

%%
/*
 * SEÇÃO DE CÓDIGO C FINAL
 * As implementações de funções e as definições de variáveis globais vêm aqui.
 * Isso garante que elas sejam globais e visíveis para o linker.
 */

// <<< AS DEFINIÇÕES GLOBAIS ESTÃO AQUI AGORA! >>>
struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0;

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}

int inserir_simbolo(char *nome, int valor) {
    int indice = procurar_simbolo(nome);
    if (indice != -1) {
        tabelaSimbolos[indice].valor = valor;
        return indice;
    }
    if (proximoSimbolo >= MAX_SIMBOLOS) {
        fprintf(stderr, "Tabela de símbolos cheia!\n");
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