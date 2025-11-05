// Arquivo: src/interpretador.c (COMPLETO E CORRIGIDO)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"    // Assume definição dos nós e tipos (TIPO_INT, etc.)
#include "simbolo.h" // Assume definição de ValorSimbolo e funções da tabela

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

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
ValorSimbolo interpretar(AstNode* no) {
    ValorSimbolo resultado;
    resultado.inicializado = 0;
    resultado.tipo = TIPO_INT;
    
    if (!no || interpret_error || g_break_flag) {
        return resultado;
    }

    int lineno = no->lineno;

    switch (no->type) {
        case NODE_TYPE_NUM:
            resultado.tipo = TIPO_INT;
            resultado.valor.i = no->data.valor;
            resultado.inicializado = 1;
            break;

        case NODE_TYPE_CHAR:
            resultado.tipo = TIPO_CHAR;
            resultado.valor.c = no->data.cvalor;
            resultado.inicializado = 1;
            break;
            
        case NODE_TYPE_FLOAT:
            resultado.tipo = TIPO_FLOAT;
            resultado.valor.f = no->data.fvalor;
            resultado.inicializado = 1;
            break;

        case NODE_TYPE_STRING:
            resultado.tipo = TIPO_STRING;
            resultado.valor.s = strdup(no->data.svalor); // Faz cópia
            resultado.inicializado = 1;
            break;
            
        case NODE_TYPE_ID: {
            if (!tabela_procurar(no->data.nome, &resultado)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não foi definida.\n", lineno, no->data.nome);
                interpret_error = 1;
            }
            break;
        }
        
        case NODE_TYPE_ASSIGN: {
            if (no->data.children.left->type != NODE_TYPE_ID) {
                fprintf(stderr, "Linha %d: Erro Semântico: Lado esquerdo da atribuição deve ser uma variável.\n", lineno);
                interpret_error = 1;
                break;
            }
            
            char* nome = no->data.children.left->data.nome;
            ValorSimbolo var;
            if (!tabela_procurar(nome, &var)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não declarada.\n", lineno, nome);
                interpret_error = 1;
                break;
            }
            
            ValorSimbolo valor_direita = interpretar(no->data.children.right);
            if (interpret_error || !valor_direita.inicializado) {
                break;
            }
            
            // Atualiza o valor mantendo o tipo original da variável
            var.inicializado = 1;
            if (var.tipo == TIPO_FLOAT) {
                if (valor_direita.tipo == TIPO_FLOAT) {
                    var.valor.f = valor_direita.valor.f;
                } else if (valor_direita.tipo == TIPO_INT) {
                    var.valor.f = (float)valor_direita.valor.i;
                }
            } else if (var.tipo == TIPO_INT) {
                if (valor_direita.tipo == TIPO_INT) {
                    var.valor.i = valor_direita.valor.i;
                } else if (valor_direita.tipo == TIPO_FLOAT) {
                    var.valor.i = (int)valor_direita.valor.f;
                }
            } else if (var.tipo == TIPO_CHAR) {  // ← ADICIONE ESTE BLOCO
                if (valor_direita.tipo == TIPO_CHAR) {
                    var.valor.c = valor_direita.valor.c;
                } else if (valor_direita.tipo == TIPO_INT) {
                    var.valor.c = (char)valor_direita.valor.i;
                }
            } else if (var.tipo == TIPO_STRING) {
                if (valor_direita.tipo == TIPO_STRING) {
                // Libera string anterior se existir
                if (var.valor.s) free(var.valor.s);
                    var.valor.s = strdup(valor_direita.valor.s);
            } else {
                fprintf(stderr, "Linha %d: Erro Semântico: Não é possível atribuir não-string a string.\n", lineno);
                interpret_error = 1;
                break;
            }
            }
            
            // Para char e string, adicione similarmente
            
            tabela_inserir(nome, var);
            resultado = valor_direita; // Retorna o valor atribuído
            break;
        }

        case NODE_TYPE_OP: {
            if (no->op == ';') {
                interpretar(no->data.children.left);
                if (interpret_error || g_break_flag) break;
                resultado = interpretar(no->data.children.right);
                break;
            }
            if (no->op == ':') {
                interpretar(no->data.children.left);
                if (interpret_error || g_break_flag) break;
                resultado = interpretar(no->data.children.right);
                break;
            }

            ValorSimbolo esq = interpretar(no->data.children.left);
            if (interpret_error || !esq.inicializado) break;
            
            ValorSimbolo dir = interpretar(no->data.children.right);
            if (interpret_error || !dir.inicializado) break;

            resultado.inicializado = 1;
            
            // Operações com tipos mistos
            if (esq.tipo == TIPO_FLOAT || dir.tipo == TIPO_FLOAT) {
                resultado.tipo = TIPO_FLOAT;
                float esq_f = (esq.tipo == TIPO_FLOAT) ? esq.valor.f : (float)esq.valor.i;
                float dir_f = (dir.tipo == TIPO_FLOAT) ? dir.valor.f : (float)dir.valor.i;
                
                switch (no->op) {
                    case '+': resultado.valor.f = esq_f + dir_f; break;
                    case '-': resultado.valor.f = esq_f - dir_f; break;
                    case '*': resultado.valor.f = esq_f * dir_f; break;
                    case '/':
                        if (dir_f == 0.0f) {
                            erro_tipo("divisão por zero", lineno);
                            resultado.inicializado = 0;
                            break;
                        }
                        resultado.valor.f = esq_f / dir_f;
                        break;
                    case '<': 
                    case '>': 
                    case 'L': 
                    case 'G': 
                    case 'E': 
                    case 'N': 
                        resultado.tipo = TIPO_INT;
                        resultado.valor.i = 0; // Será definido abaixo
                        break;
                    default:
                        erro_tipo("operador desconhecido", lineno);
                        resultado.inicializado = 0;
                }
                
                // Operadores de comparação para float
                if (resultado.inicializado && resultado.tipo == TIPO_INT) {
                    switch (no->op) {
                        case '<': resultado.valor.i = esq_f < dir_f; break;
                        case '>': resultado.valor.i = esq_f > dir_f; break;
                        case 'L': resultado.valor.i = esq_f <= dir_f; break;
                        case 'G': resultado.valor.i = esq_f >= dir_f; break;
                        case 'E': resultado.valor.i = esq_f == dir_f; break;
                        case 'N': resultado.valor.i = esq_f != dir_f; break;
                    }
                }
            } else {
                // Ambos são int
                resultado.tipo = TIPO_INT;
                switch (no->op) {
                    case '+': resultado.valor.i = esq.valor.i + dir.valor.i; break;
                    case '-': resultado.valor.i = esq.valor.i - dir.valor.i; break;
                    case '*': resultado.valor.i = esq.valor.i * dir.valor.i; break;
                    case '/':
                        if (dir.valor.i == 0) {
                            erro_tipo("divisão por zero", lineno);
                            resultado.inicializado = 0;
                            break;
                        }
                        resultado.valor.i = esq.valor.i / dir.valor.i;
                        break;
                    case '<': resultado.valor.i = esq.valor.i < dir.valor.i; break;
                    case '>': resultado.valor.i = esq.valor.i > dir.valor.i; break;
                    case 'L': resultado.valor.i = esq.valor.i <= dir.valor.i; break;
                    case 'G': resultado.valor.i = esq.valor.i >= dir.valor.i; break;
                    case 'E': resultado.valor.i = esq.valor.i == dir.valor.i; break;
                    case 'N': resultado.valor.i = esq.valor.i != dir.valor.i; break;
                    default:
                        erro_tipo("operador desconhecido", lineno);
                        resultado.inicializado = 0;
                }
            }
            break;
        }

        case NODE_TYPE_VAR_DECL: {
            ValorSimbolo v_existente;
            if (tabela_procurar(no->data.var_decl.nome, &v_existente)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' já foi declarada.\n", lineno, no->data.var_decl.nome);
                interpret_error = 1;
                break;
            }
            
            ValorSimbolo v_nova;
            v_nova.tipo = no->data.var_decl.tipo;
            v_nova.inicializado = (no->data.var_decl.valor != NULL);
            
            if (v_nova.inicializado) {
                ValorSimbolo valor_inicial = interpretar(no->data.var_decl.valor);
                if (interpret_error || !valor_inicial.inicializado) break;
                
                if (v_nova.tipo == TIPO_FLOAT) {
                    if (valor_inicial.tipo == TIPO_FLOAT) {
                        v_nova.valor.f = valor_inicial.valor.f;
                    } else if (valor_inicial.tipo == TIPO_INT) {
                        v_nova.valor.f = (float)valor_inicial.valor.i;
                    }
                } else if (v_nova.tipo == TIPO_INT) {
                    if (valor_inicial.tipo == TIPO_INT) {
                        v_nova.valor.i = valor_inicial.valor.i;
                    } else if (valor_inicial.tipo == TIPO_FLOAT) {
                        v_nova.valor.i = (int)valor_inicial.valor.f;
                    }
                } else if (v_nova.tipo == TIPO_CHAR) {  // ← ADICIONE ESTE BLOCO
                    if (valor_inicial.tipo == TIPO_CHAR) {
                        v_nova.valor.c = valor_inicial.valor.c;
                    } else if (valor_inicial.tipo == TIPO_INT) {
                        v_nova.valor.c = (char)valor_inicial.valor.i;
                    }
                } else if (v_nova.tipo == TIPO_STRING) {
                    if (valor_inicial.tipo == TIPO_STRING) {
                        v_nova.valor.s = strdup(valor_inicial.valor.s);
                    } else {
                        fprintf(stderr, "Linha %d: Erro Semântico: Não é possível inicializar string com não-string.\n", lineno);
                        interpret_error = 1;
                        break;
                    }
                }
            } else {
                // Valores padrão
                switch (v_nova.tipo) {
                    case TIPO_INT: v_nova.valor.i = 0; break;
                    case TIPO_FLOAT: v_nova.valor.f = 0.0f; break;
                    case TIPO_CHAR: v_nova.valor.c = '\0'; break;
                    case TIPO_STRING: v_nova.valor.s = NULL; break;
                }
            }
            
            tabela_inserir(no->data.var_decl.nome, v_nova);
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_PRINTF: {
            interpretar_printf(no->data.children.left);
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_IF: {
            ValorSimbolo cond = interpretar(no->data.if_details.condicao);
            if (interpret_error || !cond.inicializado) break;
            
            int cond_valor = (cond.tipo == TIPO_FLOAT) ? (cond.valor.f != 0.0f) : (cond.valor.i != 0);
            
            if (cond_valor) {
                interpretar(no->data.if_details.bloco_then);
            } else if (no->data.if_details.bloco_else) {
                interpretar(no->data.if_details.bloco_else);
            }
            
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_WHILE: {
            int old_break_flag = g_break_flag;
            g_break_flag = 0;
            
            while (!interpret_error && !g_break_flag) {
                ValorSimbolo cond = interpretar(no->data.while_details.condicao);
                if (interpret_error || !cond.inicializado) break;
                
                int cond_valor = (cond.tipo == TIPO_FLOAT) ? (cond.valor.f != 0.0f) : (cond.valor.i != 0);
                if (!cond_valor) break;
                
                interpretar(no->data.while_details.corpo);
            }
            
            g_break_flag = old_break_flag;
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_FOR: {
    int old_break_flag = g_break_flag;
    g_break_flag = 0;
    
    // Executa a inicialização
    if (no->data.for_details.inicializacao) {
        interpretar(no->data.for_details.inicializacao);
        if (interpret_error) break;
    }
    
    // Loop principal
    while (!interpret_error && !g_break_flag) {
        // Verifica a condição (se existir)
        if (no->data.for_details.condicao) {
            ValorSimbolo cond = interpretar(no->data.for_details.condicao);
            if (interpret_error || !cond.inicializado) break;
            
            // Converte condição para booleano
            int cond_valor;
            if (cond.tipo == TIPO_FLOAT) {
                cond_valor = (cond.valor.f != 0.0f);
            } else {
                cond_valor = (cond.valor.i != 0);
            }
            
            if (!cond_valor) break; // Sai do loop se condição falsa
        }
        
        // Executa o corpo do loop
        interpretar(no->data.for_details.corpo);
        if (interpret_error || g_break_flag) break;
        
        // Executa o incremento
        if (no->data.for_details.incremento) {
            interpretar(no->data.for_details.incremento);
            if (interpret_error) break;
        }
    }
    
    g_break_flag = old_break_flag;
    resultado.inicializado = 1;
    resultado.tipo = TIPO_INT;
    resultado.valor.i = 0;
    break;
}


        // Casos simples que retornam 0
        case NODE_TYPE_BREAK:
            g_break_flag = 1;
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;

        case NODE_TYPE_CMD_LIST:
        case NODE_TYPE_SWITCH:
        case NODE_TYPE_CASE:
        case NODE_TYPE_DEFAULT:
        case NODE_TYPE_DO_WHILE:
        case NODE_TYPE_BLOCK:
            // Executa mas retorna 0
            if (no->data.children.left) interpretar(no->data.children.left);
            if (no->data.children.right) interpretar(no->data.children.right);
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;

        default:
            fprintf(stderr, "Linha %d: Erro Semântico: tipo de nó desconhecido: %d\n", lineno, no->type);
            interpret_error = 1;
    }
    
    return resultado;
}

// Função printf segura
void interpretar_printf(AstNode* expr) {
    if (!expr) return;
    interpret_error = 0;
    
    ValorSimbolo val;
    val.inicializado = 0;
    int lineno_printf = expr ? expr->lineno : 0;

    switch(expr->type) {
        case NODE_TYPE_ID: {
            if (!tabela_procurar(expr->data.nome, &val)) {
                erro_tipo("variável não declarada no printf", lineno_printf);
                return;
            }
            if (!val.inicializado) {
                erro_tipo("variável não inicializada no printf", lineno_printf);
                return;
            }
            break;
        }
        case NODE_TYPE_NUM:
            val.tipo = TIPO_INT;
            val.valor.i = expr->data.valor;
            val.inicializado = 1;
            break;
        case NODE_TYPE_FLOAT:
            val.tipo = TIPO_FLOAT;
            val.valor.f = expr->data.fvalor;
            val.inicializado = 1;
            break;
        case NODE_TYPE_CHAR:  // ← ADICIONE ESTE CASO
            val.tipo = TIPO_CHAR;
            val.valor.c = expr->data.cvalor;
            val.inicializado = 1;
            break;

        default:
            val = interpretar(expr);
            if (!val.inicializado || interpret_error) return;
            break;
    }

    if (val.inicializado) {
        switch (val.tipo) {
            case TIPO_INT: printf("%d\n", val.valor.i); break;
            case TIPO_FLOAT: printf("%.2f\n", val.valor.f); break;
            case TIPO_CHAR: printf("%c\n", val.valor.c); break;
            case TIPO_STRING: printf("%s\n", val.valor.s ? val.valor.s : "(null)"); break;
            default:
                fprintf(stderr, "Linha %d: Erro Interno: Tipo desconhecido (%d) ao imprimir.\n", lineno_printf, val.tipo);
                interpret_error = 1;
        }
    }
}