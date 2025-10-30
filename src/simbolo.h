/* Arquivo: src/simbolo.h (Versão Corrigida) */

#ifndef SIMBOLO_H
#define SIMBOLO_H

// --- Definições de Tipos (DEFINIDAS AQUI) ---
#define TIPO_INT 1
#define TIPO_FLOAT 2
#define TIPO_CHAR 3
#define TIPO_STRING 4

// --- Estrutura da Tabela de Símbolos (DEFINIDA AQUI) ---
typedef union { 
    int i; 
    float f; 
    char c; 
    char* s; 
} ValorUnion;

typedef struct {
    int tipo;
    ValorUnion valor;
    int inicializado;
} ValorSimbolo;

// --- Protótipos das Funções da Tabela ---
// (Estas são as funções que o seu interpretador.c espera)
void tabela_iniciar();
void tabela_liberar();
int tabela_procurar(char* nome, ValorSimbolo* v);
void tabela_inserir(char* nome, ValorSimbolo v);

#endif // SIMBOLO_H