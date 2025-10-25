#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

void interpretar_printf(AstNode* expr);

extern int interpret_error;

void erro_tipo(const char* msg) {
    fprintf(stderr, "Erro semântico: %s\n", msg);
    interpret_error = 1;
}

// Interpretar nó da AST
int interpretar(AstNode* no) {
    if (!no || interpret_error) return 0;

    switch (no->type) {
        case NODE_TYPE_NUM:
            return no->data.valor;

        case NODE_TYPE_ID: {
            ValorSimbolo v;
            if (!tabela_procurar(no->data.nome, &v)) {
                erro_tipo("variável não declarada");
                return 0;
            }
            if (!v.inicializado) {
                erro_tipo("variável não inicializada");
                return 0;
            }

            switch (v.tipo) {
                case TIPO_INT: return v.valor.i;
                case TIPO_FLOAT: return (int)v.valor.f;
                case TIPO_CHAR: return (int)v.valor.c;
                case TIPO_STRING:
                    erro_tipo("não é possível usar string em expressão numérica");
                    return 0;
            }
        }

        case NODE_TYPE_ASSIGN: {
            char* nome = no->data.children.left->data.nome;
            ValorSimbolo v;
            if (!tabela_procurar(nome, &v)) {
                erro_tipo("variável não declarada");
                return 0;
            }

            int val = interpretar(no->data.children.right);
            if (interpret_error) return 0;

            switch (v.tipo) {
                case TIPO_INT: v.valor.i = val; break;
                case TIPO_FLOAT: v.valor.f = (float)val; break;
                case TIPO_CHAR: v.valor.c = (char)val; break;
                case TIPO_STRING:
                    erro_tipo("não é possível atribuir número a string");
                    return 0;
            }

            v.inicializado = 1;
            tabela_inserir(nome, v);
            return val;
        }

        case NODE_TYPE_OP: {
            int esq = interpretar(no->data.children.left);
            int dir = interpretar(no->data.children.right);
            if (interpret_error) return 0;

            switch (no->op) {
                case '+': return esq + dir;
                case '-': return esq - dir;
                case '*': return esq * dir;
                case '/':
                    if (dir == 0) { erro_tipo("divisão por zero"); return 0; }
                    return esq / dir;
                case '<': return esq < dir;
                case '>': return esq > dir;
                case 'L': return esq <= dir;
                case 'G': return esq >= dir;
                case 'E': return esq == dir;
                case 'N': return esq != dir;
            }
        }

        case NODE_TYPE_VAR_DECL: {
            ValorSimbolo v;
            v.tipo = no->data.var_decl.tipo;
            v.inicializado = no->data.var_decl.valor != NULL; // Inicializada se houver valor

            switch (v.tipo) {
                case TIPO_INT: v.valor.i = v.inicializado ? interpretar(no->data.var_decl.valor) : 0; break;
                case TIPO_FLOAT: v.valor.f = v.inicializado ? (float)interpretar(no->data.var_decl.valor) : 0.0; break;
                case TIPO_CHAR: v.valor.c = v.inicializado ? (char)interpretar(no->data.var_decl.valor) : '\0'; break;
                case TIPO_STRING: v.valor.s = NULL; break;
            }

            tabela_inserir(no->data.var_decl.nome, v);
            return 0;
        }

        case NODE_TYPE_CMD_LIST: {
            AstNode* temp = no;
            while (temp && !interpret_error) {
                interpretar(temp->data.cmd_list.first);
                temp = temp->data.cmd_list.next;
            }
            return 0;
        }

        case NODE_TYPE_IF: {
            int cond = interpretar(no->data.if_details.condicao);
            if (interpret_error) return 0;

            if (cond) interpretar(no->data.if_details.bloco_then);
            else if (no->data.if_details.bloco_else) interpretar(no->data.if_details.bloco_else);

            return 0;
        }

        case NODE_TYPE_PRINTF: {
            interpretar_printf(no->data.children.left);
            return 0;
        }

        default:
            erro_tipo("tipo de nó desconhecido");
            return 0;
    }
}

// Função printf segura
void interpretar_printf(AstNode* expr) {
    if (!expr) return;

    interpret_error = 0; // resetar para esta execução

    int val = 0;

    switch(expr->type) {
        case NODE_TYPE_ID: {
            ValorSimbolo v;
            if (!tabela_procurar(expr->data.nome, &v)) {
                erro_tipo("variável não declarada");
                return;
            }
            if (!v.inicializado) {
                erro_tipo("variável não inicializada");
                return;
            }

            switch (v.tipo) {
                case TIPO_INT: printf("%d\n", v.valor.i); break;
                case TIPO_FLOAT: printf("%f\n", v.valor.f); break;
                case TIPO_CHAR: printf("%c\n", v.valor.c); break;
                case TIPO_STRING: printf("%s\n", v.valor.s ? v.valor.s : "(null)"); break;
            }
            break;
        }

        case NODE_TYPE_NUM:
            printf("%d\n", expr->data.valor);
            break;

        default:
            val = interpretar(expr);
            if (!interpret_error) printf("%d\n", val);
            break;
    }
}
