#include <stdio.h>
#include <stdlib.h>
#include "simbolo.h"
#include "ast.h" 

extern int yyparse(void);
extern FILE *yyin; 

extern AstNode* g_ast_root;
extern int interpret_error;


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso correto: %s <arquivo_do_script>\n", argv[0]);
        exit(1);
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", argv[1]);
        exit(1);
    }

    yyin = arquivo; 
    tabela_iniciar(); 

    printf("--- Executando script: %s ---\n\n", argv[1]);

    yyparse(); 

    if (g_ast_root) {
        interpretar(g_ast_root); // ...interpreta a árvore inteira
        liberar_ast(g_ast_root); // ...e libera a memória
    }
    
    if (interpret_error) {
         printf("\n--- Script interrompido por erro semântico ---\n");
    } else {
         printf("\n--- Fim do script ---\n");
    }

    tabela_liberar();
    fclose(arquivo);
    
    return 0;
}