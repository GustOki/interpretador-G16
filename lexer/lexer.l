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
"for"       { return FOR; }
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
[0-9]+\.[0-9]*          { yylval.fval = atof(yytext); return FLOAT_NUM; }
'[^']'                  { yylval.cval = yytext[1]; return CHAR_LIT; }
\"[^\"]*\"              { yylval.str = strdup(yytext+1); yylval.str[strlen(yylval.str)-1] = '\0'; return STRING_LIT; } 

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }


[ \t\r\n]+              { /* Ignora todos os espaços em branco, tabs e newlines (incluindo \r) */ }

\/\/[^\r\n]* { /* Ignora comentários de linha única, parando antes de \r ou \n */ }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

.                       {
                            /* yytext contém o caractere inválido (terminado em '\0') */
                            fprintf(stderr, "Linha %d: Erro Léxico: Caractere inesperado '%s'\n", yylineno, yytext);
                        }

%%
