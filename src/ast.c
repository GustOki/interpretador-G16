#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include <string.h>


// Função para criar um nó de número
AstNode* create_num_node(int valor) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_NUM;
    no->op = 0; // Operador não aplicável
    no->data.valor = valor;
    return no;
}

// Função para criar um nó de identificador
AstNode* create_id_node(char* nome) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_ID;
    no->op = 0;
    no->data.nome = nome; // strdup já foi feito no lexer
    return no;
}

// Função para criar um nó de operação
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

// Função para criar um nó de atribuição
AstNode* create_assign_node(AstNode* left, AstNode* right) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    no->type = NODE_TYPE_ASSIGN;
    no->op = '=';
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}

// Função para criar um nó de condição
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




// Função para liberar a memória da AST
void liberar_ast(AstNode* no) {
    if (!no) return;
    switch (no->type) {
        case NODE_TYPE_OP:
        case NODE_TYPE_ASSIGN:
            liberar_ast(no->data.children.left);
            liberar_ast(no->data.children.right);
            break;

        case NODE_TYPE_ID:
            free(no->data.nome);
            break;

        case NODE_TYPE_VAR_DECL:
            free(no->data.var_decl.nome);
            liberar_ast(no->data.var_decl.valor);
            break;

        default:
            break;
    }
    free(no);
}

