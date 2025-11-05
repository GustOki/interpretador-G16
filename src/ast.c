/* Arquivo: src/ast.c (Corrigido com 'lineno' em todas as funções) */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "simbolo.h" // Inclui para TIPO_INT, etc.
#include <string.h> 

// Se strdup não estiver disponível, defina uma versão própria
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

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

AstNode* create_float_node(float valor, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_FLOAT;
    no->op = 0;
    no->lineno = lineno;
    no->data.fvalor = valor;
    return no;
}

AstNode* create_char_node(char valor, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_CHAR;
    no->op = 0;
    no->lineno = lineno;
    no->data.cvalor = valor;
    return no;
}

AstNode* create_string_node(char* valor, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_STRING;
    no->op = 0;
    no->lineno = lineno;
    no->data.svalor = strdup(valor); // Faz cópia da string
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

AstNode* create_for_node(AstNode* inicializacao, AstNode* condicao, AstNode* incremento, AstNode* corpo, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_FOR;
    no->op = 0;
    no->lineno = lineno;
    no->data.for_details.inicializacao = inicializacao;
    no->data.for_details.condicao = condicao;
    no->data.for_details.incremento = incremento;
    no->data.for_details.corpo = corpo;
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

AstNode* create_array_decl_node(int tipo, char* nome, int tamanho, AstNode* valores, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_ARRAY_DECL;
    no->op = 0;
    no->lineno = lineno;
    no->data.array_decl.tipo = tipo;
    no->data.array_decl.nome = nome;
    no->data.array_decl.tamanho = tamanho;
    no->data.array_decl.valores_iniciais = valores;
    return no;
}

AstNode* create_array_access_node(char* nome, AstNode* indice, int lineno) {
    AstNode* no = (AstNode*) malloc(sizeof(AstNode));
    if (!no) { fprintf(stderr, "Erro de alocação\n"); exit(1); }
    no->type = NODE_TYPE_ARRAY_ACCESS;
    no->op = 0;
    no->lineno = lineno;
    no->data.array_access.nome = nome;
    no->data.array_access.indice = indice;
    return no;
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
            free(no->data.nome);
            break;
        case NODE_TYPE_VAR_DECL:
            free(no->data.var_decl.nome);
            liberar_ast(no->data.var_decl.valor);
            break;
        case NODE_TYPE_ARRAY_DECL:  // <<< ADICIONE ESTE CASO
            free(no->data.array_decl.nome);
            liberar_ast(no->data.array_decl.valores_iniciais);
            break;
        case NODE_TYPE_ARRAY_ACCESS:  // <<< ADICIONE ESTE CASO
            free(no->data.array_access.nome);
            liberar_ast(no->data.array_access.indice);
            break;
        case NODE_TYPE_PRINTF:
            liberar_ast(no->data.children.left);
            break;
        case NODE_TYPE_WHILE:
            liberar_ast(no->data.while_details.condicao);
            liberar_ast(no->data.while_details.corpo);
            break;
        case NODE_TYPE_DO_WHILE:
            liberar_ast(no->data.do_while_details.corpo);
            liberar_ast(no->data.do_while_details.condicao);
            break;
        case NODE_TYPE_FOR:
            liberar_ast(no->data.for_details.inicializacao);
            liberar_ast(no->data.for_details.condicao);
            liberar_ast(no->data.for_details.incremento);
            liberar_ast(no->data.for_details.corpo);
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
        case NODE_TYPE_STRING:
            free(no->data.svalor);
            break;
        case NODE_TYPE_NUM:
        case NODE_TYPE_FLOAT:
        case NODE_TYPE_CHAR:
        case NODE_TYPE_BREAK:
        case NODE_TYPE_ARRAY_INIT_LIST:  // <<< ADICIONE ESTE CASO
            // Não têm filhos para liberar
            break;
        case NODE_TYPE_CMD_LIST:
             liberar_ast(no->data.cmd_list.first);
             liberar_ast(no->data.cmd_list.next);
             break;
        case NODE_TYPE_BLOCK:
             break;
    }
    free(no);
}

// Declaração antecipada da função recursiva
static void imprimir_ast_recursivo(AstNode* no, int indent);

// Função auxiliar para imprimir a indentação
static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("    "); // 4 espaços por nível
    }
}

// Função auxiliar para imprimir o operador
static void print_op(char op) {
    switch(op) {
        case 'L': printf(" <= "); break; // Assumindo 'L'
        case 'G': printf(" >= "); break; // Assumindo 'G'
        case 'E': printf(" == "); break; // Assumindo 'E'
        case 'N': printf(" != "); break; // Assumindo 'N'
        default: printf(" %c ", op); // Para +, -, *, /, <, >
    }
}

// Função recursiva principal
static void imprimir_ast_recursivo(AstNode* no, int indent) {
    if (!no) return;

    // Imprime a indentação para a linha atual
    print_indent(indent);

    switch (no->type) {
        case NODE_TYPE_NUM:
            printf("%d", no->data.valor);
            break;
        case NODE_TYPE_FLOAT: 
            printf("%.2f", no->data.fvalor);
            break;
        case NODE_TYPE_CHAR:  
            printf("'%c'", no->data.cvalor);
            break;
        case NODE_TYPE_STRING:
            printf("\"%s\"", no->data.svalor); 
            break;
        case NODE_TYPE_ID:
            printf("%s", no->data.nome);
            break;
        
        case NODE_TYPE_OP:
            if (no->op == ';') { // Lista de comandos
                imprimir_ast_recursivo(no->data.children.left, indent);
                printf("\n"); // Nova linha para o próximo comando
                imprimir_ast_recursivo(no->data.children.right, indent);
            } else if (no->op == ':') { // Lista de cases
                imprimir_ast_recursivo(no->data.children.left, indent);
                printf("\n");
                imprimir_ast_recursivo(no->data.children.right, indent);
            } else { // Operador binário
                printf("(");
                imprimir_ast_recursivo(no->data.children.left, 0); // 0 indent
                print_op(no->op);
                imprimir_ast_recursivo(no->data.children.right, 0); // 0 indent
                printf(")");
            }
            break;
        
        case NODE_TYPE_ASSIGN:
            printf("(");
            imprimir_ast_recursivo(no->data.children.left, 0);
            printf(" = ");
            imprimir_ast_recursivo(no->data.children.right, 0);
            printf(")");
            break;

        case NODE_TYPE_VAR_DECL:
            printf("%s %s", get_tipo_str(no->data.var_decl.tipo), no->data.var_decl.nome);
            if (no->data.var_decl.valor) {
                printf(" = ");
                imprimir_ast_recursivo(no->data.var_decl.valor, 0);
            }
            break;

        case NODE_TYPE_PRINTF: // Ou PRINT
            printf("printf(");
            imprimir_ast_recursivo(no->data.children.left, 0);
            printf(")");
            break;

        case NODE_TYPE_IF:
            printf("if (");
            imprimir_ast_recursivo(no->data.if_details.condicao, 0);
            printf(") {\n");
            imprimir_ast_recursivo(no->data.if_details.bloco_then, indent + 1);
            printf("\n");
            print_indent(indent);
            printf("}");
            if (no->data.if_details.bloco_else) {
                printf(" else {\n");
                imprimir_ast_recursivo(no->data.if_details.bloco_else, indent + 1);
                printf("\n");
                print_indent(indent);
                printf("}");
            }
            break;

        case NODE_TYPE_WHILE:
            printf("while (");
            imprimir_ast_recursivo(no->data.while_details.condicao, 0);
            printf(") {\n");
            imprimir_ast_recursivo(no->data.while_details.corpo, indent + 1);
            printf("\n");
            print_indent(indent);
            printf("}");
            break;

        case NODE_TYPE_DO_WHILE:
            printf("do {\n");
            imprimir_ast_recursivo(no->data.do_while_details.corpo, indent + 1);
            printf("\n");
            print_indent(indent);
            printf("} while (");
            imprimir_ast_recursivo(no->data.do_while_details.condicao, 0);
            printf(")");
            break;
        case NODE_TYPE_FOR:
            printf("for(");
            
            if (no->data.for_details.inicializacao && no->data.for_details.inicializacao->type == NODE_TYPE_ASSIGN) {
                AstNode* assign = no->data.for_details.inicializacao;
                printf("%s = ", assign->data.children.left->data.nome);
                imprimir_ast_recursivo(assign->data.children.right, 0);
            }
            
            printf("; ");
            
            if (no->data.for_details.condicao && no->data.for_details.condicao->type == NODE_TYPE_OP) {
                AstNode* cond = no->data.for_details.condicao;
                printf("%s", cond->data.children.left->data.nome);
                print_op(cond->op);
                imprimir_ast_recursivo(cond->data.children.right, 0);
            }
            
            printf("; ");
            
            if (no->data.for_details.incremento && no->data.for_details.incremento->type == NODE_TYPE_ASSIGN) {
                AstNode* inc = no->data.for_details.incremento;
                printf("%s = %s + 1", inc->data.children.left->data.nome, inc->data.children.left->data.nome);
            }
            
            printf(") {\n");
            imprimir_ast_recursivo(no->data.for_details.corpo, indent + 1);
            printf("\n");
            print_indent(indent);
            printf("}");
            break;
            
        case NODE_TYPE_SWITCH:
            printf("switch (");
            imprimir_ast_recursivo(no->data.switch_details.condicao, 0);
            printf(") {\n");
            imprimir_ast_recursivo(no->data.switch_details.casos, indent + 1);
            printf("\n");
            print_indent(indent);
            printf("}");
            break;

        case NODE_TYPE_CASE:
            printf("case ");
            imprimir_ast_recursivo(no->data.case_details.valor, 0);
            printf(":\n");
            imprimir_ast_recursivo(no->data.case_details.corpo, indent + 1);
            // O 'proximo' é tratado pela lista ligada (ver parser.y)
            break;

        case NODE_TYPE_DEFAULT:
            printf("default:\n");
            imprimir_ast_recursivo(no->data.case_details.corpo, indent + 1);
            // O 'proximo' é tratado pela lista ligada (ver parser.y)
            break;
            
        case NODE_TYPE_BREAK:
            printf("break");
            break;
        case NODE_TYPE_ARRAY_DECL:
            printf("%s %s[%d]", 
                   get_tipo_str(no->data.array_decl.tipo), 
                   no->data.array_decl.nome,
                   no->data.array_decl.tamanho);
            if (no->data.array_decl.valores_iniciais) {
                printf(" = {");
                imprimir_ast_recursivo(no->data.array_decl.valores_iniciais, 0);
                printf("}");
            }
            break;
            
        case NODE_TYPE_ARRAY_ACCESS:
            printf("%s[", no->data.array_access.nome);
            imprimir_ast_recursivo(no->data.array_access.indice, 0);
            printf("]");
            break;
            
        case NODE_TYPE_ARRAY_INIT_LIST:
            // Tratado como parte de array_decl
            break;
            
        default:
            // Ignora tipos desconhecidos ou não imprimíveis (como CMD_LIST ou BLOCK)
            break; 
    }
}

// Função principal que o main.c chama
void imprimir_ast_principal(AstNode* no) {
    printf("Árvore Sintática Abstrata (AST):\n");
    imprimir_ast_recursivo(no, 0); // Começa com indentação 0
    printf("\n--------------------------------------------\n");
}