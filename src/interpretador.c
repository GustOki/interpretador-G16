#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolo.h"

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif


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

ValorSimbolo interpretar(AstNode* no) {
    ValorSimbolo resultado;
    resultado.inicializado = 0;
    resultado.tipo = TIPO_INT;
    
    /* --- ATUALIZADO: Verifica g_break_flag --- */
    if (!no || interpret_error || g_break_flag) {
        return resultado;
    }

    int lineno = no->lineno;

    switch (no->type) {
        /* --- SEUS CASOS EXISTENTES (MANTIDOS) --- */
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
            resultado.valor.s = strdup(no->data.svalor);
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
            if (no->data.children.left->type == NODE_TYPE_ID) {
                char* nome = no->data.children.left->data.nome;
                ValorSimbolo var = {0};
                if (!tabela_procurar(nome, &var)) {
                    fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' não declarada.\n", lineno, nome);
                    interpret_error = 1;
                    break;
                }
                
                ValorSimbolo valor_direita = interpretar(no->data.children.right);
                if (interpret_error || !valor_direita.inicializado) {
                    break;
                }
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
                } else if (var.tipo == TIPO_CHAR) {
                    if (valor_direita.tipo == TIPO_CHAR) {
                        var.valor.c = valor_direita.valor.c;
                    } else if (valor_direita.tipo == TIPO_INT) {
                        var.valor.c = (char)valor_direita.valor.i;
                    }
                } else if (var.tipo == TIPO_STRING) {
                    if (valor_direita.tipo == TIPO_STRING) {
                        if (var.valor.s) free(var.valor.s);
                        var.valor.s = strdup(valor_direita.valor.s);
                    } else {
                        fprintf(stderr, "Linha %d: Erro Semântico: Não é possível atribuir não-string a string.\n", lineno);
                        interpret_error = 1;
                        break;
                    }
                }
                
                tabela_inserir(nome, var);
                resultado = valor_direita;
                
            } else if (no->data.children.left->type == NODE_TYPE_ARRAY_ACCESS) {
                AstNode* array_node = no->data.children.left;
                char* nome = array_node->data.array_access.nome;
                
                ValorSimbolo array;
                if (!tabela_procurar(nome, &array)) {
                    fprintf(stderr, "Linha %d: Erro Semântico: Array '%s' não declarado.\n", lineno, nome);
                    interpret_error = 1;
                    break;
                }
                
                if (!array.is_array) {
                    fprintf(stderr, "Linha %d: Erro Semântico: '%s' não é um array.\n", lineno, nome);
                    interpret_error = 1;
                    break;
                }
                
                ValorSimbolo indice_val = interpretar(array_node->data.array_access.indice);
                if (!indice_val.inicializado || interpret_error) break;
                
                int indice = (indice_val.tipo == TIPO_INT) ? indice_val.valor.i : (int)indice_val.valor.f;
                
                if (indice < 0 || indice >= array.array_size) {
                    fprintf(stderr, "Linha %d: Erro Semântico: Índice %d fora dos limites do array (0-%d).\n", 
                            lineno, indice, array.array_size - 1);
                    interpret_error = 1;
                    break;
                }
                
                ValorSimbolo valor_direita = interpretar(no->data.children.right);
                if (interpret_error || !valor_direita.inicializado) break;
                
                if (array.tipo == TIPO_INT) {
                    array.array_data[indice].i = (valor_direita.tipo == TIPO_INT) ? 
                        valor_direita.valor.i : (int)valor_direita.valor.f;
                } else if (array.tipo == TIPO_FLOAT) {
                    array.array_data[indice].f = (valor_direita.tipo == TIPO_FLOAT) ? 
                        valor_direita.valor.f : (float)valor_direita.valor.i;
                } else if (array.tipo == TIPO_CHAR) {
                    array.array_data[indice].c = (valor_direita.tipo == TIPO_CHAR) ? 
                        valor_direita.valor.c : (char)valor_direita.valor.i;
                } else if (array.tipo == TIPO_STRING) {
                    if (valor_direita.tipo == TIPO_STRING) {
                        if (array.array_data[indice].s) free(array.array_data[indice].s);
                        array.array_data[indice].s = strdup(valor_direita.valor.s);
                    } else {
                        fprintf(stderr, "Linha %d: Erro Semântico: Não é possível atribuir não-string a elemento string.\n", lineno);
                        interpret_error = 1;
                        break;
                    }
                }
                
                tabela_inserir(nome, array);
                resultado = valor_direita;
                
            } else {
                fprintf(stderr, "Linha %d: Erro Semântico: Lado esquerdo da atribuição deve ser uma variável ou acesso a array.\n", lineno);
                interpret_error = 1;
                break;
            }
            
            break;
        }

        case NODE_TYPE_OP: {
            if (no->op == ';') {
                interpretar(no->data.children.left);
                /* --- ATUALIZADO: Checa g_break_flag --- */
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
                        resultado.valor.i = 0;
                        break;
                    
                    default:
                        erro_tipo("operador desconhecido", lineno);
                        resultado.inicializado = 0;
                }
                
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
            
            ValorSimbolo v_nova = {0};
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
                } else if (v_nova.tipo == TIPO_CHAR) {
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
            
            if (no->data.for_details.inicializacao) {
                interpretar(no->data.for_details.inicializacao);
                if (interpret_error) break;
            }
            
            while (!interpret_error && !g_break_flag) {
                if (no->data.for_details.condicao) {
                    ValorSimbolo cond = interpretar(no->data.for_details.condicao);
                    if (interpret_error || !cond.inicializado) break;
                    
                    int cond_valor;
                    if (cond.tipo == TIPO_FLOAT) {
                        cond_valor = (cond.valor.f != 0.0f);
                    } else {
                        cond_valor = (cond.valor.i != 0);
                    }
                    
                    if (!cond_valor) break;
                }
                
                interpretar(no->data.for_details.corpo);
                if (interpret_error || g_break_flag) break;
                
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

        case NODE_TYPE_DO_WHILE: {
            int old_break_flag = g_break_flag;
            g_break_flag = 0;

            do {
                interpretar(no->data.do_while_details.corpo);

                if (interpret_error || g_break_flag) break;

                ValorSimbolo cond = interpretar(no->data.do_while_details.condicao);
                if (interpret_error || !cond.inicializado) break;

                int cond_valor = (cond.tipo == TIPO_FLOAT)
                    ? (cond.valor.f != 0.0f)
                    : (cond.valor.i != 0);

                if (!cond_valor) break;

            } while (1);

            g_break_flag = old_break_flag;

            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        /* --- ATUALIZADO: Lida com CMD_LIST e checa BREAK --- */
        case NODE_TYPE_CMD_LIST: {
            AstNode* temp = no;
            while (temp && !interpret_error && !g_break_flag) {
                interpretar(temp->data.cmd_list.first);
                temp = temp->data.cmd_list.next;
            }
            
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_BLOCK:
            if (no->data.children.left) interpretar(no->data.children.left);
            if (no->data.children.right) interpretar(no->data.children.right);
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        
        case NODE_TYPE_ARRAY_DECL: {
            ValorSimbolo v_existente;
            if (tabela_procurar(no->data.array_decl.nome, &v_existente)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Variável '%s' já foi declarada.\n", 
                        lineno, no->data.array_decl.nome);
                interpret_error = 1;
                break;
            }
            
            ValorSimbolo v_array = {0};
            v_array.tipo = no->data.array_decl.tipo;
            v_array.is_array = 1;
            v_array.array_size = no->data.array_decl.tamanho;
            v_array.array_data = calloc(v_array.array_size, sizeof(ValorUnion));
            v_array.inicializado = 1;
            
            for (int i = 0; i < v_array.array_size; i++) {
                switch (v_array.tipo) {
                    case TIPO_INT: v_array.array_data[i].i = 0; break;
                    case TIPO_FLOAT: v_array.array_data[i].f = 0.0f; break;
                    case TIPO_CHAR: v_array.array_data[i].c = '\0'; break;
                    case TIPO_STRING: v_array.array_data[i].s = NULL; break;
                }
            }
            
            if (no->data.array_decl.valores_iniciais) {
                int idx = 0;
                AstNode* lista_atual = no->data.array_decl.valores_iniciais;
                
                while (lista_atual && idx < v_array.array_size) {
                    ValorSimbolo val;
                    
                    if (lista_atual->type == NODE_TYPE_OP && lista_atual->op == ',') {
                        AstNode* temp_lista = lista_atual;
                        int stack_size = 0;
                        AstNode* stack[100];
                        
                        while (temp_lista && temp_lista->type == NODE_TYPE_OP && temp_lista->op == ',') {
                            stack[stack_size++] = temp_lista->data.children.right;
                            temp_lista = temp_lista->data.children.left;
                        }
                        
                        if (temp_lista) {
                            val = interpretar(temp_lista);
                            if (!interpret_error && val.inicializado && idx < v_array.array_size) {
                                if (v_array.tipo == TIPO_INT) {
                                    v_array.array_data[idx].i = (val.tipo == TIPO_INT) ? val.valor.i : (int)val.valor.f;
                                } else if (v_array.tipo == TIPO_FLOAT) {
                                    v_array.array_data[idx].f = (val.tipo == TIPO_FLOAT) ? val.valor.f : (float)val.valor.i;
                                }
                                idx++;
                            }
                        }
                        
                        for (int i = stack_size - 1; i >= 0 && idx < v_array.array_size; i--) {
                            val = interpretar(stack[i]);
                            if (!interpret_error && val.inicializado) {
                                if (v_array.tipo == TIPO_INT) {
                                    v_array.array_data[idx].i = (val.tipo == TIPO_INT) ? val.valor.i : (int)val.valor.f;
                                } else if (v_array.tipo == TIPO_FLOAT) {
                                    v_array.array_data[idx].f = (val.tipo == TIPO_FLOAT) ? val.valor.f : (float)val.valor.i;
                                }
                                idx++;
                            }
                        }
                        break;
                    } else {
                        val = interpretar(lista_atual);
                        if (!interpret_error && val.inicializado) {
                            if (v_array.tipo == TIPO_INT) {
                                v_array.array_data[idx].i = (val.tipo == TIPO_INT) ? val.valor.i : (int)val.valor.f;
                            } else if (v_array.tipo == TIPO_FLOAT) {
                                v_array.array_data[idx].f = (val.tipo == TIPO_FLOAT) ? val.valor.f : (float)val.valor.i;
                            }
                            idx++;
                        }
                        break;
                    }
                }
            }
            
            tabela_inserir(no->data.array_decl.nome, v_array);
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        case NODE_TYPE_ARRAY_ACCESS: {
            ValorSimbolo array;
            if (!tabela_procurar(no->data.array_access.nome, &array)) {
                fprintf(stderr, "Linha %d: Erro Semântico: Array '%s' não declarado.\n", 
                        lineno, no->data.array_access.nome);
                interpret_error = 1;
                break;
            }
            if (!array.is_array) {
                fprintf(stderr, "Linha %d: Erro Semântico: '%s' não é um array.\n", 
                        lineno, no->data.array_access.nome);
                interpret_error = 1;
                break;
            }
            
            ValorSimbolo indice_val = interpretar(no->data.array_access.indice);
            if (!indice_val.inicializado || interpret_error) break;
            
            int indice = (indice_val.tipo == TIPO_INT) ? indice_val.valor.i : (int)indice_val.valor.f;
            
            if (indice < 0 || indice >= array.array_size) {
                fprintf(stderr, "Linha %d: Erro Semântico: Índice %d fora dos limites do array (0-%d).\n", 
                        lineno, indice, array.array_size - 1);
                interpret_error = 1;
                break;
            }
            
            resultado.tipo = array.tipo;
            resultado.inicializado = 1;
            resultado.valor = array.array_data[indice];
            break;
        }

        /* --- REINTRODUZIDO: BREAK --- */
        case NODE_TYPE_BREAK:
            g_break_flag = 1;
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;

        /* --- REINTRODUZIDO: SWITCH --- */
        case NODE_TYPE_SWITCH: {
            ValorSimbolo cond = interpretar(no->data.switch_details.condicao);
            if (interpret_error || !cond.inicializado) break;

            int old_break_flag = g_break_flag;
            g_break_flag = 0;

            int match_encontrado = 0;
            AstNode* caso_atual = no->data.switch_details.casos;
            AstNode* default_corpo = NULL;

            // Loop pela lista de cases
            while (caso_atual && !g_break_flag) {
                // DEFAULT
                if (caso_atual->data.case_details.valor == NULL) {
                    default_corpo = caso_atual->data.case_details.corpo;
                }
                // CASE
                else {
                    if (!match_encontrado) {
                        ValorSimbolo case_val = interpretar(caso_atual->data.case_details.valor);
                        if (interpret_error) break;

                        // Comparação baseada no tipo
                        int eh_igual = 0;
                        if (cond.tipo == TIPO_INT && case_val.tipo == TIPO_INT) {
                            eh_igual = (cond.valor.i == case_val.valor.i);
                        } else if (cond.tipo == TIPO_FLOAT && case_val.tipo == TIPO_FLOAT) {
                            eh_igual = (cond.valor.f == case_val.valor.f);
                        }
                        // (Adicione mais tipos se necessário)

                        if (eh_igual) match_encontrado = 1;
                    }
                }

                // Se encontrou match (agora ou antes), executa (fall-through)
                if (match_encontrado) {
                    interpretar(caso_atual->data.case_details.corpo);
                }
                
                caso_atual = caso_atual->data.case_details.proximo;
            }

            // Se não houve match e existe default, executa default
            if (!match_encontrado && default_corpo && !g_break_flag) {
                interpretar(default_corpo);
            }

            g_break_flag = old_break_flag;
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.valor.i = 0;
            break;
        }

        default:
            fprintf(stderr, "Linha %d: Erro Semântico: tipo de nó desconhecido: %d\n", lineno, no->type);
            interpret_error = 1;
    }
    
    return resultado;
}

void interpretar_printf(AstNode* expr) {
    if (!expr) return;
    interpret_error = 0;
    
    ValorSimbolo val = {0};
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
        
        case NODE_TYPE_CHAR:
            val.tipo = TIPO_CHAR;
            val.valor.c = expr->data.cvalor;
            val.inicializado = 1;
            break;

        case NODE_TYPE_STRING: // Adicionado para suporte a string no printf
             val.tipo = TIPO_STRING;
             val.valor.s = expr->data.svalor;
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
