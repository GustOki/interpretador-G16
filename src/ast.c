// Arquivo: src/ast.c

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

AstNode* create_num_node(int valor) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_NUM;
    no->op = 0; no->data.valor = valor;
    return no;
}
AstNode* create_id_node(char* nome) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_ID;
    no->op = 0; no->data.nome = nome;
    return no;
}
AstNode* create_op_node(char op, AstNode* left, AstNode* right) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_OP;
    no->op = op;
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}
AstNode* create_assign_node(AstNode* left, AstNode* right) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_ASSIGN;
    no->op = '=';
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}
AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    no->type = NODE_TYPE_IF;
    no->op = 0;
    no->data.if_details.condicao = condicao;
    no->data.if_details.bloco_then = bloco_then;
    no->data.if_details.bloco_else = bloco_else;
    return no;
}
void liberar_ast(AstNode* no) {
    if (!no) return;
    switch (no->type) {
        case NODE_TYPE_OP:
        case NODE_TYPE_ASSIGN:
            liberar_ast(no->data.children.left);
            liberar_ast(no->data.children.right);
            break;
        case NODE_TYPE_IF:
            liberar_ast(no->data.if_details.condicao);
            liberar_ast(no->data.if_details.bloco_then);
            liberar_ast(no->data.if_details.bloco_else);
            break;
        case NODE_TYPE_ID:
            free(no->data.nome);
            break;
        case NODE_TYPE_NUM:
            break;
    }
    free(no);
}