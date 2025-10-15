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
"if"        { return IF; }
"else"      { return ELSE; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return PONTO_VIRGULA; }
"=="        { return EQ; }
"!="        { return NE; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }
"="         { return IGUAL; }


[0-9]+                  { yylval.valor = atoi(yytext); return NUM; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"\n"                    { return NEWLINE; }

[ \t]+                  { /* ignora espaços e tabs */ }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return ID; }

.                       {
                            /* yytext contém o caractere inválido (terminado em '\0') */
                            fprintf(stderr, "Linha %d: caractere inválido '%s'\n", yylineno, yytext);
                        }

%%
