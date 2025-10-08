// Arquivo: src/simbolo.h

#ifndef SIMBOLO_H
#define SIMBOLO_H

#define MAX_SIMBOLOS 100
#define MAX_NOME 100  

struct simbolo {
    char nome[MAX_NOME];
    int valor;
};

extern struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
extern int proximoSimbolo;

int inserir_simbolo(char *nome, int valor);
int procurar_simbolo(char *nome);

#endif