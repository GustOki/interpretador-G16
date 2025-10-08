#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolo.h"

#define TAMANHO_TABELA 256


typedef struct NoSimbolo {
    char* nome;
    double valor;
    struct NoSimbolo* proximo;
} NoSimbolo;


static NoSimbolo* tabela[TAMANHO_TABELA];

static unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % TAMANHO_TABELA;
}

void tabela_iniciar() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela[i] = NULL;
    }
}

void tabela_inserir(char* nome, double valor) {
    unsigned long indice = hash(nome);
    NoSimbolo* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            atual->valor = valor;
            free(nome); 
            return;
        }
        atual = atual->proximo;
    }

    // Se não encontrou, cria um novo nó e insere no início da lista
    NoSimbolo* novo_no = (NoSimbolo*)malloc(sizeof(NoSimbolo));
    novo_no->nome = nome; 
    novo_no->valor = valor;
    novo_no->proximo = tabela[indice];
    tabela[indice] = novo_no;
}

int tabela_procurar(char* nome, double* valor_encontrado) {
    unsigned long indice = hash(nome);
    NoSimbolo* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            *valor_encontrado = atual->valor;
            return 1; // Encontrou
        }
        atual = atual->proximo;
    }

    return 0; // Não encontrou
}

void tabela_liberar() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoSimbolo* atual = tabela[i];
        while (atual != NULL) {
            NoSimbolo* temp = atual;
            atual = atual->proximo;
            free(temp->nome);
            free(temp);
        }
        tabela[i] = NULL;
    }
}