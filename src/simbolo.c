#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolo.h"

#define TAMANHO_TABELA 256

typedef struct NoSimbolo {
    char* nome;
    ValorSimbolo valor;
    struct NoSimbolo* proximo;
} NoSimbolo;

static NoSimbolo* tabela[TAMANHO_TABELA];

static unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TAMANHO_TABELA;
}

void tabela_iniciar() {
    for (int i = 0; i < TAMANHO_TABELA; i++)
        tabela[i] = NULL;
}

void tabela_inserir(char* nome, ValorSimbolo valor) {
    unsigned long indice = hash(nome);
    NoSimbolo* atual = tabela[indice];

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            if (atual->valor.tipo == TIPO_STRING && atual->valor.valor.s)
                free(atual->valor.valor.s);
            atual->valor = valor;
            return;
        }
        atual = atual->proximo;
    }

    NoSimbolo* novo = malloc(sizeof(NoSimbolo));
    novo->nome = strdup(nome);
    novo->valor = valor;
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

int tabela_procurar(char* nome, ValorSimbolo* valor_encontrado) {
    unsigned long indice = hash(nome);
    NoSimbolo* atual = tabela[indice];

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            *valor_encontrado = atual->valor;
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

void tabela_liberar() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoSimbolo* atual = tabela[i];
        while (atual) {
            NoSimbolo* temp = atual;
            if (temp->valor.tipo == TIPO_STRING && temp->valor.valor.s)
                free(temp->valor.valor.s);

            if (temp->valor.is_array && temp->valor.array_data) {
                if (temp->valor.tipo == TIPO_STRING) {
                    for (int j = 0; j < temp->valor.array_size; j++) {
                        if (temp->valor.array_data[j].s)
                            free(temp->valor.array_data[j].s);
                    }
                }
                free(temp->valor.array_data);
            }
            free(temp->nome);
            atual = atual->proximo;
            free(temp);
        }
        tabela[i] = NULL;
    }
}

const char* get_tipo_str(int tipo) {
    switch(tipo) {
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_CHAR: return "char";
        case TIPO_STRING: return "string";
        default: return "desconhecido";
    }
}

static void tipo_format(const ValorSimbolo* v, char* out, size_t outlen) {
    if (!v || !out) return;
    if (v->is_array) {
        snprintf(out, outlen, "vetor<%s, %d>", get_tipo_str(v->tipo), v->array_size);
    } else {
        snprintf(out, outlen, "%s", get_tipo_str(v->tipo));
    }
}

void imprimir_tabela_simbolos() {
    printf("Tabela de Símbolos:\n");
    char tipo_buf[64];

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoSimbolo* atual = tabela[i];
        while (atual) {
            tipo_format(&atual->valor, tipo_buf, sizeof(tipo_buf));

            if (atual->valor.is_array) {
                printf("Nome: %s, Tipo: %s, ", atual->nome, tipo_buf);

                if (!atual->valor.inicializado || !atual->valor.array_data) {
                    printf("Valores: NÃO INICIALIZADOS\n");
                } else {
                    printf("Valores: {");
                    for (int j = 0; j < atual->valor.array_size; j++) {
                        switch (atual->valor.tipo) {
                            case TIPO_INT:
                                printf("%d", atual->valor.array_data[j].i);
                                break;
                            case TIPO_FLOAT:
                                printf("%g", atual->valor.array_data[j].f);
                                break;
                            case TIPO_CHAR:
                                printf("'%c'", atual->valor.array_data[j].c);
                                break;
                            case TIPO_STRING:
                                printf("\"%s\"", atual->valor.array_data[j].s ? atual->valor.array_data[j].s : "");
                                break;
                            default:
                                printf("?");
                        }
                        if (j < atual->valor.array_size - 1) printf(", ");
                    }
                    printf("}\n");
                }

            } else {
                if (atual->valor.inicializado) {
                    switch (atual->valor.tipo) {
                        case TIPO_INT:
                            printf("Nome: %s, Tipo: %s, Valor: %d\n",
                                   atual->nome, tipo_buf,
                                   atual->valor.valor.i);
                            break;
                        case TIPO_FLOAT:
                            printf("Nome: %s, Tipo: %s, Valor: %g\n",
                                   atual->nome, tipo_buf,
                                   atual->valor.valor.f);
                            break;
                        case TIPO_CHAR:
                            printf("Nome: %s, Tipo: %s, Valor: '%c'\n",
                                   atual->nome, tipo_buf,
                                   atual->valor.valor.c);
                            break;
                        case TIPO_STRING:
                            printf("Nome: %s, Tipo: %s, Valor: \"%s\"\n",
                                   atual->nome, tipo_buf,
                                   atual->valor.valor.s ? atual->valor.valor.s : "");
                            break;
                        default:
                            printf("Nome: %s, Tipo: %s, Valor: ?\n",
                                   atual->nome, tipo_buf);
                    }
                } else {
                    printf("Nome: %s, Tipo: %s, Valor: NÃO INICIALIZADA\n",
                           atual->nome, tipo_buf);
                }
            }

            atual = atual->proximo;
        }
    }
    printf("--------------------------------------------\n");
}
