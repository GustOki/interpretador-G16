#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include <string.h>


AstNode* create_num_node(int valor) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_NUM;
    no->op = 0; 
    no->data.valor = valor;
    return no;
}

AstNode* create_id_node(char* nome) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_ID;
    no->op = 0;
    no->data.nome = nome; 
    return no;
}

AstNode* create_op_node(char op, AstNode* left, AstNode* right) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_OP;
    no->op = op;
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}

AstNode* create_assign_node(AstNode* left, AstNode* right) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_ASSIGN;
    no->op = '=';
    no->data.assign.left = left; 
    no->data.assign.right = right;
    return no;
}

AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_IF;
    no->op = 0; // Não aplicável
    no->data.if_details.condicao = condicao;
    no->data.if_details.bloco_then = bloco_then;
    no->data.if_details.bloco_else = bloco_else;
    return no;
}

AstNode* create_relop_node(char op, AstNode* left, AstNode* right) {
    AstNode* node = malloc(sizeof(AstNode));
    node->type = NODE_TYPE_OP;
    node->op = op;
    node->data.children.left = left;
    node->data.children.right = right;
    return node;
}

AstNode* create_command_list(AstNode* first, AstNode* next) {
    AstNode* node = malloc(sizeof(AstNode));
    node->type = NODE_TYPE_CMD_LIST;
    node->data.cmd_list.first = first;
    node->data.cmd_list.next = next;
    return node;
}

AstNode* append_command_list(AstNode* list, AstNode* cmd) {
    if (!list) return create_command_list(cmd, NULL);
    AstNode* temp = list;
    while (temp->data.cmd_list.next)
        temp = temp->data.cmd_list.next;
    temp->data.cmd_list.next = create_command_list(cmd, NULL);
    return list;
}

AstNode* create_var_decl_node(int tipo, char* nome, AstNode* valor) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_VAR_DECL;
    no->data.var_decl.tipo = tipo;
    no->data.var_decl.nome = strdup(nome);
    no->data.var_decl.valor = valor;
    return no;
}

AstNode* create_printf_node(AstNode* expr) {
    AstNode* no = malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de memória\n"); exit(1); }
    no->type = NODE_TYPE_PRINTF;
    no->data.children.left = expr;
    no->data.children.right = NULL;
    return no;
}

AstNode* create_switch_node(AstNode* condicao, AstNode* casos) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_SWITCH;
    no->op = 0;
    no->data.switch_details.condicao = condicao;
    no->data.switch_details.casos = casos; 
    return no;
}

AstNode* create_case_node(AstNode* valor, AstNode* corpo) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_CASE;
    no->op = 0;
    no->data.case_details.valor = valor;
    no->data.case_details.corpo = corpo;
    no->data.case_details.proximo = NULL; 
    return no;
}

AstNode* create_default_node(AstNode* corpo) {
    return create_case_node(NULL, corpo);
}

AstNode* create_break_node() {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_BREAK;
    no->op = 0;
    return no;
}

AstNode* append_case_list(AstNode* head, AstNode* new_case) {
    if (!head) {
        return new_case;
    }
    
    AstNode* temp = head;
    while (temp->data.case_details.proximo) {
        temp = temp->data.case_details.proximo;
    }
    
    temp->data.case_details.proximo = new_case;
    return head; 
}
AstNode* create_while_node(AstNode* condicao, AstNode* corpo) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_WHILE;
    no->op = 0;
    no->data.while_details.condicao = condicao;
    no->data.while_details.corpo = corpo;
    return no;
}
AstNode* create_do_while_node(AstNode* corpo, AstNode* condicao) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_DO_WHILE;
    no->op = 0;
    no->data.do_while_details.corpo = corpo;
    no->data.do_while_details.condicao = condicao;
    return no;
}


void liberar_ast(AstNode* no) {
    if (!no) return;
    switch (no->type) {
        case NODE_TYPE_OP:
            liberar_ast(no->data.children.left);
            liberar_ast(no->data.children.right);
            break;

        case NODE_TYPE_ASSIGN:
            liberar_ast(no->data.assign.left);
            liberar_ast(no->data.assign.right);
            break;

        case NODE_TYPE_IF:
            liberar_ast(no->data.if_details.condicao);
            liberar_ast(no->data.if_details.bloco_then);
            liberar_ast(no->data.if_details.bloco_else);
            break;

        case NODE_TYPE_CMD_LIST:
            liberar_ast(no->data.cmd_list.first);
            liberar_ast(no->data.cmd_list.next);
            break;
            
        case NODE_TYPE_PRINTF:
            liberar_ast(no->data.children.left);
            break;

        case NODE_TYPE_ID:
            free(no->data.nome);
            break;

        case NODE_TYPE_VAR_DECL:
            free(no->data.var_decl.nome);
            liberar_ast(no->data.var_decl.valor);
            break;

        case NODE_TYPE_WHILE:
            liberar_ast(no->data.while_details.condicao);
            liberar_ast(no->data.while_details.corpo);
            break;

        case NODE_TYPE_DO_WHILE:
            liberar_ast(no->data.do_while_details.corpo);
            liberar_ast(no->data.do_while_details.condicao);
            break;
            
        case NODE_TYPE_SWITCH:
            liberar_ast(no->data.switch_details.condicao);
            liberar_ast(no->data.switch_details.casos); 
            break;

        case NODE_TYPE_CASE:
            liberar_ast(no->data.case_details.valor);
	    liberar_ast(no->data.case_details.corpo);
	    liberar_ast(no->data.case_details.proximo);
            break;

        case NODE_TYPE_NUM:
        case NODE_TYPE_BREAK:
            break;

        default:
            break;
    }
    free(no);
}


