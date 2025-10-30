/* Arquivo: src/ast.c (Corrigido com 'lineno' em todas as funções) */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h" // Inclui para TIPO_INT, etc.

/* --- Funções de Criação de Nó --- */

AstNode* create_num_node(int valor, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_NUM;
    no->op = 0;
    no->lineno = lineno;
    no->data.valor = valor;
    return no;
}

AstNode* create_id_node(char* nome, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_ID;
    no->op = 0;
    no->lineno = lineno;
    no->data.nome = nome; // strdup é feito no lexer
    return no;
}

AstNode* create_op_node(char op, AstNode* left, AstNode* right, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_OP;
    no->op = op;
    no->lineno = lineno;
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}

AstNode* create_assign_node(AstNode* left, AstNode* right, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_ASSIGN;
    no->op = '=';
    no->lineno = lineno;
    no->data.children.left = left;
    no->data.children.right = right;
    return no;
}

AstNode* create_if_node(AstNode* condicao, AstNode* bloco_then, AstNode* bloco_else, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_IF;
    no->op = 0;
    no->lineno = lineno;
    no->data.if_details.condicao = condicao;
    no->data.if_details.bloco_then = bloco_then;
    no->data.if_details.bloco_else = bloco_else;
    return no;
}

AstNode* create_while_node(AstNode* condicao, AstNode* corpo, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_WHILE;
    no->op = 0;
    no->lineno = lineno;
    no->data.while_details.condicao = condicao;
    no->data.while_details.corpo = corpo;
    return no;
}

AstNode* create_do_while_node(AstNode* corpo, AstNode* condicao, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_DO_WHILE;
    no->op = 0;
    no->lineno = lineno;
    no->data.do_while_details.corpo = corpo;
    no->data.do_while_details.condicao = condicao;
    return no;
}

AstNode* create_var_decl_node(int tipo, char* nome, AstNode* valor, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_VAR_DECL;
    no->op = 0;
    no->lineno = lineno;
    no->data.var_decl.tipo = tipo;
    no->data.var_decl.nome = nome;
    no->data.var_decl.valor = valor;
    return no;
}

AstNode* create_printf_node(AstNode* expr, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_PRINTF;
    no->op = 0;
    no->lineno = lineno;
    // Assumindo que você reutiliza a struct 'children' ou 'print_details'
    no->data.children.left = expr; 
    no->data.children.right = NULL;
    return no;
}

AstNode* create_switch_node(AstNode* condicao, AstNode* casos, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_SWITCH;
    no->op = 0;
    no->lineno = lineno;
    no->data.switch_details.condicao = condicao;
    no->data.switch_details.casos = casos;
    return no;
}

AstNode* create_case_node(AstNode* valor, AstNode* corpo, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_CASE;
    no->op = 0;
    no->lineno = lineno;
    no->data.case_details.valor = valor;
    no->data.case_details.corpo = corpo;
    no->data.case_details.proximo = NULL; // Inicializa 'proximo'
    return no;
}

AstNode* create_default_node(AstNode* corpo, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_DEFAULT;
    no->op = 0;
    no->lineno = lineno;
    no->data.case_details.valor = NULL; // Default não tem valor
    no->data.case_details.corpo = corpo;
    no->data.case_details.proximo = NULL; // Inicializa 'proximo'
    return no;
}

AstNode* create_break_node(int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_BREAK;
    no->op = 0;
    no->lineno = lineno;
    return no;
}

/* --- Funções Auxiliares (sem 'lineno' do parser) --- */

AstNode* append_command_list(AstNode* list, AstNode* cmd) {
    if (!list) return cmd;
    // Usa 'create_op_node' para encadear, passando a linha do primeiro comando
    return create_op_node(';', list, cmd, list->lineno); 
}

AstNode* append_case_list(AstNode* head, AstNode* new_case) {
    // Esta função agora constrói uma lista FIFO (ordem correta)
    
    if (!head) {
        return new_case; // Se a lista está vazia, o novo case é a cabeça
    }
    
    // Se a lista não está vazia, encontra o final da lista
    AstNode* temp = head;
    while (temp->data.case_details.proximo != NULL) {
        temp = temp->data.case_details.proximo;
    }
    
    // Adiciona o novo case no final
    temp->data.case_details.proximo = new_case;
    return head; // Retorna a CABEÇA original da lista
}
/* --- Função para Liberar a AST --- */
// (Esta função precisa ser completa para evitar leaks)
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
            free(no->data.nome); // Só libera se o lexer usou strdup
            break;
        case NODE_TYPE_VAR_DECL:
            free(no->data.var_decl.nome); // strdup do ID
            liberar_ast(no->data.var_decl.valor); // Libera valor inicial, se houver
            break;
        case NODE_TYPE_PRINTF:
            liberar_ast(no->data.children.left); // Ou print_details.expressao
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
        case NODE_TYPE_DEFAULT:
            liberar_ast(no->data.case_details.corpo);
            liberar_ast(no->data.case_details.proximo);
            break;
        case NODE_TYPE_NUM:
        case NODE_TYPE_BREAK:
            // Não têm filhos para liberar
            break;
        
        // Cuidado com tipos não tratados (CMD_LIST, BLOCK)
        case NODE_TYPE_CMD_LIST: // Se você usa ';' como OP, isso pode não ser necessário
             liberar_ast(no->data.cmd_list.first);
             liberar_ast(no->data.cmd_list.next);
             break;
        case NODE_TYPE_BLOCK:
             // Depende de como NODE_TYPE_BLOCK é usado
             break;
    }
    free(no);
}