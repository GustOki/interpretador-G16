// Arquivo: src/main.c

#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);

int main(void) {
  printf("Interpretador com AST. Pressione Ctrl+D para sair.\n");
  while(!feof(stdin)) {
      printf("> ");
      fflush(stdout);
      yyparse();
  }
  printf("\nAté mais!\n");
  return 0;
}