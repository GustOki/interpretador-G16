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
            free(temp->nome);
            atual = atual->proximo;
            free(temp);
        }
        tabela[i] = NULL;
    }
}

// Função auxiliar para converter o TIPO em string
const char* get_tipo_str(int tipo) {
    switch(tipo) {
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_CHAR: return "char";
        case TIPO_STRING: return "string";
        default: return "desconhecido";
    }
}

// Função principal para imprimir a tabela de símbolos
void imprimir_tabela_simbolos() {
    printf("Tabela de Símbolos:\n");

    // Itera por toda a tabela hash
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoSimbolo* atual = tabela[i];
        // Percorre a lista ligada em cada balde do hash
        while (atual) {
            if (atual->valor.inicializado) {
                // Variável inicializada - mostra o valor
                switch (atual->valor.tipo) {
                    case TIPO_INT:
                        printf("Nome: %s, Tipo: %s, Valor: %d\n",
                               atual->nome,
                               get_tipo_str(atual->valor.tipo),
                               atual->valor.valor.i);
                        break;
                    case TIPO_FLOAT:
                        printf("Nome: %s, Tipo: %s, Valor: %.2f\n",
                               atual->nome,
                               get_tipo_str(atual->valor.tipo),
                               atual->valor.valor.f);
                        break;
                    case TIPO_CHAR:
                        printf("Nome: %s, Tipo: %s, Valor: '%c'\n",
                               atual->nome,
                               get_tipo_str(atual->valor.tipo),
                               atual->valor.valor.c);
                        break;
                    case TIPO_STRING:
                        printf("Nome: %s, Tipo: %s, Valor: \"%s\"\n",
                               atual->nome,
                               get_tipo_str(atual->valor.tipo),
                               atual->valor.valor.s);
                        break;
                    default:
                        printf("Nome: %s, Tipo: %s, Valor: ?\n",
                               atual->nome,
                               get_tipo_str(atual->valor.tipo));
                }
            } else {
                // Variável não inicializada
                printf("Nome: %s, Tipo: %s, Valor: NÃO INICIALIZADA\n",
                       atual->nome,
                       get_tipo_str(atual->valor.tipo));
            }
            atual = atual->proximo;
        }
    }
    printf("--------------------------------------------\n");
}