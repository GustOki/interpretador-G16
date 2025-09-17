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
=======

#define MAX_SIMBOLOS 100 
struct simbolo {
    char nome[50];
    int valor;
};

struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
int proximoSimbolo = 0; // Usado para rastrear o proximo indice livre na tabela

// Funções para gerenciar a tabela de símbolos
int inserir_simbolo(char *nome, int valor) {
    if (proximoSimbolo >= MAX_SIMBOLOS) {
        fprintf(stderr, "Erro: Tabela de símbolos cheia!\n");
        exit(1);
    }
    strcpy(tabelaSimbolos[proximoSimbolo].nome, nome);
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


/* Declara a função yylex (gerada pelo Flex) e 
   a função yyerror, para evitar avisos de declaração implícita. */

  int yylex(void);
  void yyerror(const char *s);
%}

// Definindo o tipo de dados para os valores semânticos (inteiro e string )
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

//Prioridade (mais alta em baixo)
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc LPAREN RPAREN

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
=======
    expressao NEWLINE { printf("Resultado: %d\n", $1); }
    | atribuicao NEWLINE
    | NEWLINE
    | error NEWLINE { yyerrok; }

    ;

atribuicao:
    ID IGUAL expressao {

        $$ = create_assign_node(create_id_node($1), $3);
        
    int indice = procurar_simbolo($1);
      if (indice == -1) {
        inserir_simbolo($1, $3);
      } 
      else {
        // Atualize o valor
        tabelaSimbolos[indice].valor = $3;
      }

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
=======
    NUM {$$=$1;}

  | ID { // pegar o valor da variavel
        int indice = procurar_simbolo($1);
        if (indice == -1) {
            fprintf(stderr, "Erro semantico: Variavel '%s' nao declarada\n", $1);
            exit(1);
        }
        $$ = tabelaSimbolos[indice].valor;
    }

  | expressao PLUS expressao {$$=$1+$3;}
  | expressao MINUS expressao {$$=$1-3;}
  | expressao TIMES expressao {$$=$1*$3;}
  | expressao DIVIDE expressao {
    if($3==0){
      fprintf(stderr, "Erro: Divisao por zero!\n");
            exit(1);
    }
    else{
      $$ = $1 / $3;
    }
  }
  | LPAREN expressao RPAREN {$$=$2;}
  ;
%%

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
=======
int main(void) {

  while(1){
    printf("Digite uma expressao: ");
    fflush(stdout); // Adiciona o printf automaticamente antes da chamada da função
    yyparse();
  }
    
  return 0;

}