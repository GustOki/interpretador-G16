// Arquivo: src/interpretador.c (COMPLETO E CORRIGIDO)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"    // Assume definição dos nós e tipos (TIPO_INT, etc.)
#include "simbolo.h" // Assume definição de ValorSimbolo e funções da tabela

// Protótipos
void interpretar_printf(AstNode* expr);
int tabela_procurar(char* nome, ValorSimbolo* v);
void tabela_inserir(char* nome, ValorSimbolo v);
void tabela_iniciar();
void tabela_liberar();

extern int interpret_error;
static int g_break_flag = 0;

void erro_tipo(const char* msg, int lineno) {
    fprintf(stderr, "Linha %d: Erro Semântico: %s\n", lineno, msg);
    interpret_error = 1;
}

// Interpretar nó da AST
int interpretar(AstNode* no) {
    if (!no || interpret_error || g_break_flag) return 0;

    int lineno = no ? no->lineno : 0;

    switch (no->type) {
        case NODE_TYPE_NUM:
            return no->data.valor;

        case NODE_TYPE_ID: {
            ValorSimbolo v;
            if (!tabela_procurar(no->data.nome, &v)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não foi definida.\n", lineno, no->data.nome);
                interpret_error = 1; return 0;
            }
            if (!v.inicializado) {
                 fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não inicializada.\n", lineno, no->data.nome);
                 interpret_error = 1; return 0;
            }
            switch (v.tipo) {
                case TIPO_INT: return v.valor.i;
                case TIPO_FLOAT: return (int)v.valor.f;
                case TIPO_CHAR: return (int)v.valor.c;
                case TIPO_STRING:
                    fprintf(stderr, "Linha %d: Erro Semântico: Não é possível usar string em contexto numérico.\n", lineno);
                    interpret_error = 1; return 0;
                default:
                     fprintf(stderr, "Linha %d: Erro Interno: Tipo desconhecido para variável '%s'.\n", lineno, no->data.nome);
                     interpret_error = 1; return 0;
            }
        }
        case NODE_TYPE_ASSIGN: {
             if (no->data.children.left->type != NODE_TYPE_ID) {
                 fprintf(stderr, "Linha %d: Erro Semântico: Lado esquerdo da atribuição deve ser uma variável.\n", lineno);
                 interpret_error = 1; return 0;
             }
            char* nome = no->data.children.left->data.nome;
            ValorSimbolo v;
            if (!tabela_procurar(nome, &v)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não declarada.\n", lineno, nome);
                interpret_error = 1; return 0;
            }
            int val = interpretar(no->data.children.right);
            if (interpret_error) return 0;
            switch (v.tipo) {
                case TIPO_INT: v.valor.i = val; break;
                case TIPO_FLOAT: v.valor.f = (float)val; break;
                case TIPO_CHAR: v.valor.c = (char)val; break;
                case TIPO_STRING:
                    fprintf(stderr, "Linha %d: Erro Semântico: Não é possível atribuir número a string.\n", lineno);
                    interpret_error = 1; return 0;
                default:
                    fprintf(stderr, "Linha %d: Erro Interno: Tipo desconhecido para variável '%s'.\n", lineno, nome);
                    interpret_error = 1; return 0;
            }
            v.inicializado = 1;
            tabela_inserir(nome, v);
            return val;
        }

        case NODE_TYPE_OP: {
             if (no->op == ';') {
                 interpretar(no->data.children.left);
                 if (interpret_error || g_break_flag) return 0;
                 return interpretar(no->data.children.right);
             }
             if (no->op == ':') { // Usado pelo parser para construir lista de cases
                 interpretar(no->data.children.left);
                 if (interpret_error || g_break_flag) return 0;
                 return interpretar(no->data.children.right);
             }

            int esq = interpretar(no->data.children.left);
            if (interpret_error) return 0;
            int dir = interpretar(no->data.children.right);
            if (interpret_error) return 0;

            switch (no->op) {
                case '+': return esq + dir;
                case '-': return esq - dir;
                case '*': return esq * dir;
                case '/':
                    if (dir == 0) { erro_tipo("divisão por zero", lineno); return 0; }
                    return esq / dir;
                case '<': return esq < dir;
                case '>': return esq > dir;
                case 'L': return esq <= dir;
                case 'G': return esq >= dir;
                case 'E': return esq == dir;
                case 'N': return esq != dir;
                default:
                     erro_tipo("operador desconhecido", lineno);
                     return 0;
            }
        }

        case NODE_TYPE_VAR_DECL: {
             ValorSimbolo v_existente;
             if (tabela_procurar(no->data.var_decl.nome, &v_existente)) {
                  fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' já foi declarada.\n", lineno, no->data.var_decl.nome);
                  interpret_error = 1; return 0;
             }
            ValorSimbolo v_nova;
            v_nova.tipo = no->data.var_decl.tipo;
            v_nova.inicializado = no->data.var_decl.valor != NULL;
            if (v_nova.inicializado) {
                 int val_inicial = interpretar(no->data.var_decl.valor);
                 if (interpret_error) return 0;
                 switch (v_nova.tipo) {
                     case TIPO_INT: v_nova.valor.i = val_inicial; break;
                     case TIPO_FLOAT: v_nova.valor.f = (float)val_inicial; break;
                     case TIPO_CHAR: v_nova.valor.c = (char)val_inicial; break;
                     case TIPO_STRING:
                         fprintf(stderr, "Linha %d: Erro Semântico: Inicialização de string com número não suportada.\n", lineno);
                         interpret_error = 1; return 0;
                 }
            } else {
                 switch (v_nova.tipo) {
                     case TIPO_INT: v_nova.valor.i = 0; break;
                     case TIPO_FLOAT: v_nova.valor.f = 0.0f; break;
                     case TIPO_CHAR: v_nova.valor.c = '\0'; break;
                     case TIPO_STRING: v_nova.valor.s = NULL; break;
                 }
            }
            tabela_inserir(no->data.var_decl.nome, v_nova);
            return 0;
        }

        case NODE_TYPE_CMD_LIST: {
            AstNode* temp = no;
            while (temp && !interpret_error && !g_break_flag) {
                interpretar(temp->data.cmd_list.first);
                temp = temp->data.cmd_list.next;
            }
            return 0;
        }

        case NODE_TYPE_IF: {
            int cond = interpretar(no->data.if_details.condicao);
            if (interpret_error) return 0;
            if (cond) interpretar(no->data.if_details.bloco_then);
            else if (no->data.if_details.bloco_else) interpretar(no->data.if_details.bloco_else);
            return 0;
        }

        case NODE_TYPE_PRINTF: {
            interpretar_printf(no->data.children.left);
            return 0;
        }

        case NODE_TYPE_BREAK: {
            g_break_flag = 1;
            return 0;
        }

        /* ========================================================================= */
        /* <<< BLOCO DE SWITCH CORRIGIDO (O que você postou + correção fall-through) */
        /* ========================================================================= */
        case NODE_TYPE_SWITCH: {
            int cond_valor = interpretar(no->data.switch_details.condicao);
            if (interpret_error) return 0;

            int old_break_flag = g_break_flag;
            g_break_flag = 0; 
            int match_encontrado = 0;
            
            AstNode* caso_atual = no->data.switch_details.casos;
            AstNode* default_corpo = NULL;

            // Percorre a lista ligada
            while (caso_atual && !g_break_flag) {
                
                if (caso_atual->type == NODE_TYPE_DEFAULT) {
                    default_corpo = caso_atual->data.case_details.corpo;
                }
                else if (caso_atual->type == NODE_TYPE_CASE) {
                    
                    // Passo 1: Verifica se este é o case correto, *apenas se ainda não achou*
                    if (!match_encontrado) {
                        int case_valor = interpretar(caso_atual->data.case_details.valor);
                        if (interpret_error) break; 
                        if (case_valor == cond_valor) {
                            match_encontrado = 1;
                        }
                    }
                    
                    // Passo 2: Se já achou (neste loop ou em um anterior) E não houve break,
                    // EXECUTA o corpo do case (lógica do fall-through)
                    if (match_encontrado && !g_break_flag) { 
                        interpretar(caso_atual->data.case_details.corpo);
                    }
                }
                 else {
                      fprintf(stderr, "Linha %d: Erro Interno: Nó inesperado (%d) na lista de cases do switch.\n", lineno, caso_atual->type);
                      interpret_error = 1; break;
                 }
                
                // Avança para o próximo item da lista ligada
                caso_atual = caso_atual->data.case_details.proximo;
            }

            if (!match_encontrado && default_corpo && !g_break_flag) {
                interpretar(default_corpo);
            }

            g_break_flag = old_break_flag;
            return 0;
        }
        
        case NODE_TYPE_WHILE: {
             int old_break_flag = g_break_flag; g_break_flag = 0;
             while (!interpret_error && !g_break_flag) {
                 int cond = interpretar(no->data.while_details.condicao);
                 if (interpret_error || !cond) break;
                 interpretar(no->data.while_details.corpo);
             }
             g_break_flag = old_break_flag;
             return 0;
        }
        
        case NODE_TYPE_DO_WHILE: {
             int old_break_flag = g_break_flag; g_break_flag = 0;
             int cond;
             do {
                 interpretar(no->data.do_while_details.corpo);
                 if (interpret_error || g_break_flag) break;
                 cond = interpretar(no->data.do_while_details.condicao);
                 if (interpret_error) break;
             } while (cond && !interpret_error);
             g_break_flag = old_break_flag;
             return 0;
        } 
        
        case NODE_TYPE_CASE: return 0;
        case NODE_TYPE_DEFAULT: return 0;
        case NODE_TYPE_BLOCK:
             return interpretar(no->data.children.left);

        default:
            erro_tipo("tipo de nó desconhecido", lineno);
            return 0;
    }
}

// Função printf segura
void interpretar_printf(AstNode* expr) {
    if (!expr) return;
    interpret_error = 0;
    int val = 0;
    int lineno_printf = expr ? expr->lineno : 0;

    switch(expr->type) {
        case NODE_TYPE_ID: {
            ValorSimbolo v;
            if (!tabela_procurar(expr->data.nome, &v)) {
                erro_tipo("variável não declarada no printf", lineno_printf);
                return;
            }
            if (!v.inicializado) {
                erro_tipo("variável não inicializada no printf", lineno_printf);
                return;
            }
            switch (v.tipo) {
                case TIPO_INT: printf("%d\n", v.valor.i); break;
                case TIPO_FLOAT: printf("%f\n", v.valor.f); break;
                case TIPO_CHAR: printf("%c\n", v.valor.c); break;
                case TIPO_STRING: printf("%s\n", v.valor.s ? v.valor.s : "(null)"); break;
                 default:
                     fprintf(stderr, "Linha %d: Erro Interno: Tipo desconhecido (%d) ao imprimir '%s'.\n", lineno_printf, v.tipo, expr->data.nome);
                     interpret_error = 1; break;
            }
            break;
        }
        case NODE_TYPE_NUM:
            printf("%d\n", expr->data.valor);
            break;
        default:
            val = interpretar(expr);
            if (!interpret_error) printf("%d\n", val);
            break;
    }
}