#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolo.h" // Inclui seu próprio cabeçalho para consistência

#define TAMANHO_TABELA 256

// A struct agora é interna ao .c (detalhe de implementação)
typedef struct NoSimbolo {
    char* nome;
    int valor; // << ALTERADO para int
    struct NoSimbolo* proximo;
} NoSimbolo;

// A tabela é 'static', significando que só é visível dentro deste arquivo.
static NoSimbolo* tabela[TAMANHO_TABELA];

static unsigned long hash(const char* str) {
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

void tabela_inserir(char* nome, int valor) { // << ALTERADO para int
    unsigned long indice = hash(nome);
    NoSimbolo* atual = tabela[indice];

    // Procura se o símbolo já existe para atualizá-lo
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            atual->valor = valor;
            return; // Já atualizou, não precisa fazer mais nada
        }
        atual = atual->proximo;
    }

    // Se não encontrou, cria um novo nó e insere no início da lista
    NoSimbolo* novo_no = (NoSimbolo*)malloc(sizeof(NoSimbolo));
    if (!novo_no) {
        fprintf(stderr, "Erro de alocação de memória na tabela de símbolos.\n");
        exit(1);
    }
    
    novo_no->nome = strdup(nome); // << CORREÇÃO: Faz uma cópia segura do nome
    novo_no->valor = valor;
    novo_no->proximo = tabela[indice];
    tabela[indice] = novo_no;
}

int tabela_procurar(char* nome, int* valor_encontrado) { // << ALTERADO para int*
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
            free(temp->nome); // Libera a cópia do nome feita com strdup
            free(temp);
        }
        tabela[i] = NULL;
    }
}