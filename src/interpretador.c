#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h" // <<< PASSO CRUCIAL: Inclui a definição da struct!

// Precisamos acessar a tabela de símbolos que está no parser
extern struct simbolo tabelaSimbolos[];
extern int procurar_simbolo(char* nome);
extern int inserir_simbolo(char* nome, int valor);

int interpretar(AstNode* no) {
    if (!no) return 0; // Um bloco {} vazio ou o fim da lista não retorna nada

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
            inserir_simbolo(var_nome, valor_expr);
            return valor_expr;
        }

        case NODE_TYPE_OP: {
            // --- LÓGICA ADICIONADA PARA O PONTO-E-VÍRGULA ---
            // Se o operador for ';', tratamos como uma sequência de comandos
            if (no->op == ';') {
                interpretar(no->data.children.left); // Executa o primeiro comando (lado esquerdo)
                if (interpret_error) return 0;
                return interpretar(no->data.children.right); // Executa o próximo e retorna seu valor
            }
            // --- FIM DA LÓGICA ADICIONADA ---

            // A lógica abaixo, para os outros operadores, continua a mesma
            int val_esq = interpretar(no->data.children.left);
            if (interpret_error) return 0;
            int val_dir = interpretar(no->data.children.right);
            if (interpret_error) return 0;

            switch (no->op) {
                // Aritméticos
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

