// Arquivo: src/simbolo.h

#ifndef SIMBOLO_H
#define SIMBOLO_H

#define MAX_SIMBOLOS 100

// 1. A DEFINIÇÃO COMPLETA da struct fica aqui.
struct simbolo {
    char nome[50];
    int valor;
};

// 2. As DECLARAÇÕES "extern" (promessas) ficam aqui.
//    Elas dizem aos outros arquivos que essas variáveis existem em algum lugar.
extern struct simbolo tabelaSimbolos[MAX_SIMBOLOS];
extern int proximoSimbolo;

// 3. Protótipos das funções que manipulam a tabela.
int inserir_simbolo(char *nome, int valor);
int procurar_simbolo(char *nome);

#endif