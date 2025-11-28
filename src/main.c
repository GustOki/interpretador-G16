#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolo.h"
#include "ast.h" 

extern int yyparse(void);
extern FILE *yyin; 

extern AstNode* g_ast_root;
extern int interpret_error;

extern void imprimir_tabela_simbolos();
extern void imprimir_ast_principal(AstNode* no);
extern void tabela_iniciar();
extern void tabela_liberar();
extern ValorSimbolo interpretar(AstNode* no);

void executar_e_capturar_saida(AstNode* no) {
    printf("--------------------------------------------\n");
    interpretar(no);
    printf("--------------------------------------------\n");
}

int main(int argc, char **argv) {
    FILE *arquivo = NULL;

    if (argc == 2) {
        arquivo = fopen(argv[1], "r");
        if (!arquivo) {
            fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", argv[1]);
            exit(1);
        }
        yyin = arquivo;
        printf("--- Executando script: %s ---\n\n", argv[1]);
    } 
    
    else if (argc == 1) {
        yyin = stdin; 
        printf("=== Interpretador Interativo ===\n");
        printf("Digite o código. Pressione Ctrl+D para finalizar e executar.\n\n");
    } 
    else {
        fprintf(stderr, "Uso: %s [arquivo_do_script]\n", argv[0]);
        exit(1);
    }

    tabela_iniciar(); 

    int parse_result = yyparse(); 

    if (parse_result == 0 && g_ast_root) {
        imprimir_ast_principal(g_ast_root);

        printf("Execução:\n");
        interpretar(g_ast_root);
        printf("--------------------------------------------\n");

        imprimir_tabela_simbolos();

        liberar_ast(g_ast_root);
    
    } else if (parse_result != 0) {
        printf("\n--- Script interrompido por erro de sintaxe ---\n");
    }
    
    if (interpret_error) {
        printf("\n--- Script concluído com erros semânticos ---\n");
    } else if (parse_result == 0) {
        printf("\n--- Script concluído com sucesso ---\n");
    }

    tabela_liberar();
    
    if (arquivo) {
        fclose(arquivo);
    }
    
    return (parse_result != 0 || interpret_error);
}