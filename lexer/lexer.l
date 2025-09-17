%{
#include "parser.tab.h" 
#include <stdio.h>
#include <stdlib.h> 
%}

%%

[0-9]+ { yylval.valor=atoi(yytext); return NUM; } //Converte para numero
"+"    { return PLUS; }
"-"    { return MINUS; }
"*"    { return TIMES; }
"/"    { return DIVIDE; }
"("    { return LPAREN; }
")"    { return RPAREN; }
\n     { return NEWLINE; }

[a-zA-Z][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }
"="    { return IGUAL; }


[ \t]+   { /* ignora espaços e quebras de linha */ }

.          { printf("Caractere invalido: %s\n", yytext);  /* ignora qualquer outro caractere */ }

%%

int yywrap(void) {
    return 1;
}

