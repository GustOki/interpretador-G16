#ifndef SIMBOLO_H
#define SIMBOLO_H

#define TIPO_INT 1
#define TIPO_FLOAT 2
#define TIPO_CHAR 3
#define TIPO_STRING 4

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
    int is_array;
    int array_size;
    ValorUnion* array_data;
} ValorSimbolo;

void tabela_iniciar();
void tabela_liberar();
int tabela_procurar(char* nome, ValorSimbolo* v);
void tabela_inserir(char* nome, ValorSimbolo v);
const char* get_tipo_str(int tipo);
void imprimir_tabela_simbolos();

#endif
