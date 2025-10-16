// Arquivo: src/ast.h

#ifndef AST_H
#define AST_H

typedef enum {
    NODE_TYPE_NUM, NODE_TYPE_ID, NODE_TYPE_OP,
    NODE_TYPE_ASSIGN, NODE_TYPE_IF
} NodeType;

typedef struct AstNode {
    NodeType type;
    char op;
    int lineno;
    union {
        int valor;
        char* nome;
        struct { struct AstNode *left, *right; } children;
        struct { struct AstNode *condicao, *bloco_then, *bloco_else; } if_details;
    } data;
} AstNode;

AstNode* create_num_node(int valor);
AstNode* create_id_node(char* nome);
AstNode* create_op_node(char op, AstNode* left, AstNode* right);
AstNode* create_assign_node(AstNode* left, AstNode* right);
AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else);
void liberar_ast(AstNode* no);
int interpretar(AstNode* no);

#endif