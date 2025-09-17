// Arquivo: src/ast.h

#ifndef AST_H
#define AST_H

// Enum para identificar o tipo de cada nó na árvore
typedef enum {
    NODE_TYPE_NUM,      // Nó para um número literal 
    NODE_TYPE_ID,       // Nó para um identificador
    NODE_TYPE_OP,       // Nó para uma operação binária 
    NODE_TYPE_ASSIGN    // Nó para uma atribuição 
} NodeType;

// Estrutura principal de um nó da AST
typedef struct AstNode {
    NodeType type;      // O tipo do nó
    char op;            // O operador para nós do tipo OP (+, -, *, /)
    
    // Union para armazenar dados específicos de cada tipo de nó
    union {
        // Se o tipo for NODE_TYPE_NUM
        int valor;

        // Se o tipo for NODE_TYPE_ID
        char* nome;

        // Se for uma operação ou atribuição
        struct {
            struct AstNode* left;
            struct AstNode* right;
        } children;

    } data;
} AstNode;

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções para criar nós (serão implementadas em ast.c)
AstNode* create_num_node(int valor);
AstNode* create_id_node(char* nome);
AstNode* create_op_node(char op, AstNode* left, AstNode* right);
AstNode* create_assign_node(AstNode* left, AstNode* right);
void liberar_ast(AstNode* no);

// Função do interpretador (será implementada em interpretador.c)
int interpretar(AstNode* no);

#endif