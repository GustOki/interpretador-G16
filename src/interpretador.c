// Arquivo: src/interpretador.c

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h" // <<< PASSO CRUCIAL: Inclui a definição da struct!

// Precisamos acessar a tabela de símbolos que está no parser
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
                fprintf(stderr, "Erro semântico: Variável '%s' não declarada\n", no->data.nome);
                exit(1);
            }
            return tabelaSimbolos[indice].valor;
        }

        case NODE_TYPE_ASSIGN: {
            char* var_nome = no->data.children.left->data.nome;
            int valor_expr = interpretar(no->data.children.right);
            inserir_simbolo(var_nome, valor_expr);
            return valor_expr;
        }

        case NODE_TYPE_OP: {
            int val_esq = interpretar(no->data.children.left);
            int val_dir = interpretar(no->data.children.right);
            switch (no->op) {
                case '+': return val_esq + val_dir;
                case '-': return val_esq - val_dir;
                case '*': return val_esq * val_dir;
                case '/':
                    // ... (código da divisão)
                    return val_esq / val_dir;
            }
            // <<< O PROBLEMA ESTÁ AQUI
        }
    }
    return 0; // Se chegar aqui, algo deu errado
}