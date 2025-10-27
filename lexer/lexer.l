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
"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"string"    { return STRING; }
"printf"    { return PRINTF; }

"if"        { return IF; }
"else"      { return ELSE; }
"do"        { return DO; }
"while"     { return WHILE; }
"switch"    { return SWITCH; }
"case"      { return CASE; }
"break"     { return BREAK; }
"default"   { return DEFAULT; }

"{"         { return LBRACE; }
"}"         { return RBRACE; }
":"         { return COLON; }
";"         { return PONTO_VIRGULA; }

"!="        { return NE; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }
"="         { return IGUAL; }
"=="        { return EQ; }


[0-9]+                  { yylval.valor = atoi(yytext); return NUM; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }

[ \t\n]+                  { /* ignora espaços e tabs */ }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return ID; }

.                       {
                            /* yytext contém o caractere inválido (terminado em '\0') */
                            fprintf(stderr, "Linha %d: caractere inválido '%s'\n", yylineno, yytext);
                        }

%%
