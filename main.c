// Arquivo: src/main.c (COM A CORREÇÃO)

#include <stdio.h>

// Declaramos apenas a função que inicia tudo.
extern int yyparse(void);

int main(void) {
  printf("Interpretador com AST. Pressione Ctrl+D para sair.\n");

  // O loop agora é controlado pelo próprio yyparse, que pedirá input
  // até encontrar o fim do arquivo (Ctrl+D).
  // A cada linha, o prompt será impresso.
  // A lógica de interpretar e imprimir foi movida para o parser.y.
  while(!feof(stdin)) {
      printf("> ");
      yyparse();
  }
  
  printf("\nAté mais!\n");
  return 0;
}