#include <stdio.h>
#include "simbolo.h"

extern int yyparse(void);

int main(void) {
<<<<<<< HEAD

=======
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
  tabela_iniciar();

  printf("Interpretador com AST. Pressione Ctrl+D para sair.\n");

  while(!feof(stdin)) {
      printf("> ");
      fflush(stdout); // <<< ADICIONE ESTA LINHA
      yyparse();
  }
<<<<<<< HEAD

=======
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f
  tabela_liberar();
  
  printf("\nAté mais!\n");
  return 0;
}