#ifndef SIMBOLO_H
#define SIMBOLO_H

void tabela_iniciar();
void tabela_liberar();


#define MAX_SIMBOLOS 100
#define MAX_NOME 100  


struct simbolo {
    char nome[MAX_NOME];
    int valor;
};

extern struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
extern int proximoSimbolo;

int procurar_simbolo(char *nome);
int inserir_simbolo(char *nome, int valor);

#endif 