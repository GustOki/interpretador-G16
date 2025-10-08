%option noyywrap
%option yylineno

%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
%}

%%

[0-9]+                  { yylval.valor = atoi(yytext); return NUM; }

<<<<<<< HEAD
"if"                    { return IF; }
"else"                  { return ELSE; }

"{"          { return LBRACE; }
"}"          { return RBRACE; }
"=="         { return EQ; }
"!="         { return NE; }
"<"          { return LT; }
">"          { return GT; }
"<="         { return LE; }
">="         { return GE; }

";"          { return PONTO_VIRGULA; }

[0-9]+       { yylval.valor = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

"+"          { return PLUS; }
"-"          { return MINUS; }
"*"          { return TIMES; }
"/"          { return DIVIDE; }
"="          { return IGUAL; }
"("          { return LPAREN; }
")"          { return RPAREN; }
\n           { return NEWLINE; }
[ \t]+       { /* ignora espaco */ }

.            { fprintf(stderr, "Linha %d: caractere inválido '%s'\n", yylineno, yytext); }
=======
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"\n"                    { return NEWLINE; }

[ \t]+                  { /* ignora espaços e tabs */ }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return ID; }
"="                     { return IGUAL; }

.                       {
                            /* yytext contém o caractere inválido (terminado em '\0') */
                            fprintf(stderr, "Linha %d: caractere inválido '%s'\n", yylineno, yytext);
                        }
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f

%%
