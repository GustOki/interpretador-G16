// src/interpretador.c
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h"

extern int yylineno; /* só se quiser usar linha do lexer nas mensagens (opcional) */

/* Flag global que indica se houve um erro durante a interpretação.
   Definida aqui (definição única). O parser declara 'extern' para consultar. */
int interpret_error = 0;

extern struct simbolo tabelaSimbolos[];
extern int procurar_simbolo(char* nome);
extern int inserir_simbolo(char* nome, int valor);

int interpretar(AstNode* no) {
    if (!no) return 0;

    switch (no->type) {
        case NODE_TYPE_NUM:
            return no->data.valor;

        case NODE_TYPE_ID: {
            int indice = procurar_simbolo(no->data.nome);
            if (indice == -1) {
                interpret_error = 1;
                fprintf(stderr, "Linha %d: Erro semântico: variável '%s' não declarada\n", no->lineno, no->data.nome);
                return 0;
            }
            return tabelaSimbolos[indice].valor;
        }

        case NODE_TYPE_ASSIGN: {
            char* var_nome = no->data.children.left->data.nome;
            int valor_expr = interpretar(no->data.children.right);
            if (interpret_error) return 0; /* se já houve erro dentro da expressão, propaga sem sobrescrever tabela */
            inserir_simbolo(var_nome, valor_expr);
            return valor_expr;
        }

        case NODE_TYPE_OP: {
            int val_esq = interpretar(no->data.children.left);
            if (interpret_error) return 0;
            int val_dir = interpretar(no->data.children.right);
            if (interpret_error) return 0;

            switch (no->op) {
                case '+': return val_esq + val_dir;
                case '-': return val_esq - val_dir;
                case '*': return val_esq * val_dir;
                case '/':
                    if (val_dir == 0) {
                        interpret_error = 1;
                        fprintf(stderr, "Linha %d: Erro semântico: divisão por zero\n", yylineno);
                        return 0;
                    }
                    return val_esq / val_dir;
                default:
                    interpret_error = 1;
                    fprintf(stderr, "Linha %d: Erro semântico: operador desconhecido '%c'\n", yylineno, no->op);
                    return 0;
            }
        }
    }
    /* Nó inválido */
    interpret_error = 1;
    fprintf(stderr, "Linha %d: Erro interno: nó inválido na AST\n", yylineno);
    return 0;
}
