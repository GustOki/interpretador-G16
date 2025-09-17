// Arquivo: src/ast.c

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

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

// Função para liberar a memória da AST
void liberar_ast(AstNode* no) {
    if (!no) return;
    if (no->type == NODE_TYPE_OP || no->type == NODE_TYPE_ASSIGN) {
        liberar_ast(no->data.children.left);
        liberar_ast(no->data.children.right);
    } else if (no->type == NODE_TYPE_ID) {
        free(no->data.nome); // Libera a string copiada pelo strdup
    }
    free(no);
}