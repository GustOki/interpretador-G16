#include <stdio.h>
#include "simbolo.h"

extern int yyparse(void);

int main(void) {
  tabela_iniciar();

  printf("Interpretador com AST. Pressione Ctrl+D para sair.\n");

  while(!feof(stdin)) {
      printf("> ");
      yyparse();
  }
  tabela_liberar();
  
  printf("\nAté mais!\n");
  return 0;
}