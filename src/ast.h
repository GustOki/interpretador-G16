#ifndef AST_H
#define AST_H

typedef enum {
    NODE_TYPE_NUM, // Nó para um número
    NODE_TYPE_ID, // Nó identificador
    NODE_TYPE_OP, // Nó de operacao
    NODE_TYPE_ASSIGN, // Nó de atribuição 
    NODE_TYPE_IF
} NodeType;

typedef struct AstNode {
    NodeType type;
    char op;
    int lineno; 
    union {
        int valor;
        char* nome;
        struct {
            struct AstNode* left;
            struct AstNode* right;
        } children;
         struct { 
            struct AstNode* condicao;
            struct AstNode* bloco_then;
            struct AstNode* bloco_else;
        } if_details;
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
