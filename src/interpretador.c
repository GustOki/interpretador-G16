// Arquivo: src/interpretador.c

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h"
#include "parser.tab.h"

int interpret_error = 0;

int interpretar(AstNode* no) {
    if (!no) return 0; // Bloco vazio {} ou fim da lista de comandos

    switch (no->type) {
        case NODE_TYPE_NUM:
            return no->data.valor;

        case NODE_TYPE_ID: {
            int indice = procurar_simbolo(no->data.nome);
            if (indice == -1) {
                interpret_error = 1;
                fprintf(stderr, "Erro semântico na linha %d: variável '%s' não declarada\n", no->lineno, no->data.nome);
                return 0;
            }
            return tabelaSimbolos[indice].valor;
        }

        case NODE_TYPE_ASSIGN: {
            char* var_nome = no->data.children.left->data.nome;
            int valor_expr = interpretar(no->data.children.right);
            if (interpret_error) return 0;
            inserir_simbolo(var_nome, valor_expr);
            return valor_expr;
        }

        case NODE_TYPE_OP: {
            if (no->op == ';') {
                interpretar(no->data.children.left);
                if (interpret_error) return 0;
                return interpretar(no->data.children.right);
            }

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
                        fprintf(stderr, "Erro semântico na linha %d: divisão por zero\n", no->lineno);
                        return 0;
                    }
                    return val_esq / val_dir;
                
                case '>':  return val_esq > val_dir;
                case '<':  return val_esq < val_dir;
                case 1: return val_esq >= val_dir; // GE
                case 2: return val_esq <= val_dir; // LE
                case 3: return val_esq == val_dir; // EQ
                case 4: return val_esq != val_dir; // NE

                default:
                    interpret_error = 1;
                    fprintf(stderr, "Erro semântico na linha %d: operador desconhecido '%c'\n", no->lineno, no->op);
                    return 0;
            }
        }
        
        case NODE_TYPE_IF: {
            int condicao_val = interpretar(no->data.if_details.condicao);
            if (interpret_error) return 0;

            if (condicao_val) {
                return interpretar(no->data.if_details.bloco_then);
            } 
            else if (no->data.if_details.bloco_else != NULL) {
                return interpretar(no->data.if_details.bloco_else);
            }
            return 0;
        }

        default:
            interpret_error = 1;
            fprintf(stderr, "Erro interno: nó inválido na AST (tipo %d)\n", no->type);
            return 0;
    }
}