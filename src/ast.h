#ifndef AST_H
#define AST_H

typedef enum {
    NODE_TYPE_NUM,      // Nó para um número
    NODE_TYPE_ID,       // Nó identificador
    NODE_TYPE_OP,       // Nó de operação aritmética
    NODE_TYPE_ASSIGN,   // Nó de atribuição 
    NODE_TYPE_IF,       // Nó de if
    // NODE_TYPE_RELOP,    // Nó de operação relacional (==, <, >, etc.)
    NODE_TYPE_CMD_LIST,  // Nó para lista de comandos (blocos { })
    NODE_TYPE_VAR_DECL,
    NODE_TYPE_PRINTF 
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
            struct AstNode* left;
            struct AstNode* right;
        } assign;

        struct {           
            struct AstNode* condicao;
            struct AstNode* bloco_then;
            struct AstNode* bloco_else;
        } if_details;

        struct {           
            struct AstNode* first;
            struct AstNode* next;
        } cmd_list;
    
        struct {           
            int tipo;
            char* nome;
            struct AstNode* valor;  
        } var_decl;

    } data;
} AstNode;


/* Funções de criação */
AstNode* create_num_node(int valor);
AstNode* create_id_node(char* nome);
AstNode* create_op_node(char op, AstNode* left, AstNode* right);
AstNode* create_assign_node(AstNode* left, AstNode* right);
AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else);
// AstNode* create_relop_node(char op, AstNode* left, AstNode* right);
AstNode* create_command_list(AstNode* first, AstNode* next);
AstNode* append_command_list(AstNode* list, AstNode* cmd);
AstNode* create_var_decl_node(int tipo, char* nome, AstNode* valor);


/* Funções gerais */
void liberar_ast(AstNode* no);
int interpretar(AstNode* no);

#endif
