/* Arquivo: src/ast.h (Corrigido) */

#ifndef AST_H
#define AST_H

#include "simbolo.h" // <<< INCLUI AS DEFINIÇÕES DE TIPO

// --- Enum para identificar o tipo de cada nó na árvore ---
typedef enum {
    NODE_TYPE_NUM,
    NODE_TYPE_FLOAT,
    NODE_TYPE_CHAR,
    NODE_TYPE_STRING,
    NODE_TYPE_ID,
    NODE_TYPE_OP,
    NODE_TYPE_ASSIGN,
    NODE_TYPE_IF,
    NODE_TYPE_SWITCH,
    NODE_TYPE_CASE,
    NODE_TYPE_DEFAULT,
    NODE_TYPE_DO_WHILE,
    NODE_TYPE_WHILE,
    NODE_TYPE_FOR,     
    NODE_TYPE_BLOCK,    // (Ainda aqui, mas talvez não usado)
    NODE_TYPE_BREAK,
    NODE_TYPE_CMD_LIST, // (Ainda aqui, mas talvez não usado)
    NODE_TYPE_VAR_DECL,
    NODE_TYPE_PRINTF
} NodeType;

// --- Estrutura principal de um nó da AST ---
typedef struct AstNode {
    NodeType type;
    char op;
    int lineno;
    union {
        int valor;
        float fvalor;
        char cvalor;
        char* svalor;
        char* nome;
        struct { struct AstNode *left, *right; } children;
        struct { struct AstNode *left, *right; } assign;
        struct { struct AstNode *condicao, *bloco_then, *bloco_else; } if_details;
        struct { struct AstNode *first, *next; } cmd_list;
        struct { int tipo; char* nome; struct AstNode* valor; } var_decl;
        struct { struct AstNode *condicao, *casos; } switch_details;
        struct { struct AstNode *valor, *corpo, *proximo; } case_details;
        struct { struct AstNode *condicao, *corpo; } while_details;
        struct { struct AstNode *corpo, *condicao; } do_while_details;
        struct { struct AstNode *inicializacao, *condicao, *incremento, *corpo; } for_details;
    } data;
} AstNode;


/* --- PROTÓTIPOS DAS FUNÇÕES (Atualizados com lineno) --- */
AstNode* create_num_node(int valor, int lineno);
AstNode* create_float_node(float valor, int lineno);
AstNode* create_char_node(char valor, int lineno);
AstNode* create_string_node(char* valor, int lineno);
AstNode* create_id_node(char* nome, int lineno);
AstNode* create_op_node(char op, AstNode* left, AstNode* right, int lineno);
AstNode* create_assign_node(AstNode* left, AstNode* right, int lineno);
AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else, int lineno);
AstNode* create_while_node(AstNode* condicao, AstNode* corpo, int lineno);
AstNode* create_do_while_node(AstNode* corpo, AstNode* condicao, int lineno);
AstNode* create_for_node(AstNode* inicializacao, AstNode* condicao, AstNode* incremento, AstNode* corpo, int lineno);
AstNode* create_command_list(AstNode* first, AstNode* next);
AstNode* append_command_list(AstNode* list, AstNode* cmd);
AstNode* create_var_decl_node(int tipo, char* nome, AstNode* valor, int lineno);
AstNode* create_printf_node(AstNode* expr, int lineno);
AstNode* create_switch_node(AstNode* condicao, AstNode* casos, int lineno);
AstNode* create_case_node(AstNode* valor, AstNode* corpo, int lineno);
AstNode* create_default_node(AstNode* corpo, int lineno);
AstNode* create_break_node(int lineno);
AstNode* append_case_list(AstNode* head, AstNode* new_case);

void liberar_ast(AstNode* no);
ValorSimbolo interpretar(AstNode* no);
void imprimir_ast_principal(AstNode* no);

// <<< REMOVA A DEFINIÇÃO DUPLICADA DE ValorSimbolo DAQUI >>>
/* #ifndef SIMBOLO_H
typedef union { int i; float f; char c; char* s; } ValorUnion;
typedef struct {
    int tipo;
    ValorUnion valor;
    int inicializado;
} ValorSimbolo;
#endif
*/

#endif // AST_H