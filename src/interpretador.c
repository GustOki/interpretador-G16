#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h" 

extern struct simbolo tabelaSimbolos[];
extern int procurar_simbolo(char* nome);
extern int inserir_simbolo(char* nome, int valor);
extern int interpret_error;

int interpretar(AstNode* no) {
    if (!no) return 0; 

    switch (no->type) {

        case NODE_TYPE_NUM:
            return no->data.valor;

        case NODE_TYPE_ID: {
            int indice = procurar_simbolo(no->data.nome);
            if (indice == -1) {
                interpret_error = 1;
                fprintf(stderr, "Linha %d: Erro semântico: variável '%s' não declarada\n",
                        no->lineno, no->data.nome);
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
                        fprintf(stderr, "Erro: divisão por zero.\n");
                        interpret_error = 1;
                        return 0;
                    }
                    return val_esq / val_dir;

                case '<': return val_esq < val_dir;
                case '>': return val_esq > val_dir;
                case 'L': return val_esq <= val_dir;
                case 'G': return val_esq >= val_dir;
                case 'E': return val_esq == val_dir;
                case 'N': return val_esq != val_dir;

                default:
                    fprintf(stderr, "Erro: operador desconhecido '%c'\n", no->op);
                    interpret_error = 1;
                    return 0;
            }
        }

        case NODE_TYPE_IF: {
            int cond = interpretar(no->data.if_details.condicao);
            if (interpret_error) return 0;

            if (cond) {
                interpretar(no->data.if_details.bloco_then);
            } else if (no->data.if_details.bloco_else) {
                interpretar(no->data.if_details.bloco_else);
            }
            return 0;
        }

        case NODE_TYPE_CMD_LIST: {
            AstNode* temp = no;
            while (temp) {
                interpretar(temp->data.cmd_list.first);
                if (interpret_error) return 0;
                temp = temp->data.cmd_list.next;
            }
            return 0;
        }

        default:
            fprintf(stderr, "Erro: tipo de nó desconhecido (%d)\n", no->type);
            interpret_error = 1;
            return 0;
    }

    return 0; 
}
