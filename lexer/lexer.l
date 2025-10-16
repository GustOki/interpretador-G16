/* Arquivo: lexer/lexer.l */

%option noyywrap
%option yylineno

%{
#include "parser.tab.h"
#include <string.h>
%}

%%

"if"         { return IF; }
"else"       { return ELSE; }

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


%%