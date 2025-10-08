#ifndef SIMBOLO_H
#define SIMBOLO_H

<<<<<<< HEAD
#define MAX_SIMBOLOS 100
#define MAX_NOME 100  

=======
#define MAX_SIMBOLOS 100 
#define MAX_NOME 100     
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
struct simbolo {
    char nome[MAX_NOME];
    int valor;
};

extern struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
extern int proximoSimbolo;

<<<<<<< HEAD
int inserir_simbolo(char *nome, int valor);
=======
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
int procurar_simbolo(char *nome);
int inserir_simbolo(char *nome, int valor);

#endif 