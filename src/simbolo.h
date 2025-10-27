#ifndef SIMBOLO_H
#define SIMBOLO_H

typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_CHAR,
    TIPO_STRING
} TipoSimbolo;

typedef struct {
    TipoSimbolo tipo;
    union {
        int i;
        float f;
        char c;
        char* s;
    } valor;
    int inicializado; 
} ValorSimbolo;

void tabela_iniciar();
void tabela_inserir(char* nome, ValorSimbolo valor);
int tabela_procurar(char* nome, ValorSimbolo* valor_encontrado);
void tabela_liberar();

#endif
