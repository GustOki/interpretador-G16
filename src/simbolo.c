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
