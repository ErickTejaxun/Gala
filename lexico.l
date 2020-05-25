%{
#include "sintactico.h"
#include "simbolo.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
int n_lineas;

void imprimirToken()
{
    printf("%s ", yytext);
}


%}
%x COM1 COM SALTO
%option noyywrap

DIGITO [0-9]
ident ([a-zA-Z]|_)([a-zA-Z]|_|{DIGITO})*
cadena "\""[^"\""]*"\""
%%

<COM1>\n                       {yyless(0); BEGIN(COM);}
<COM1>.                        {;}   
<COM>\n                        {n_lineas++;}
<COM>#                         {BEGIN(COM1);}
<COM>.                         {yyless(0); BEGIN(SALTO);}
<SALTO>#                       {BEGIN(COM1);}
<SALTO>[\t ]			       ;
<SALTO>.                       {yyless(0);BEGIN(0); printf("\n%i\t",n_lineas); return('\n');}
#    		                    BEGIN(COM1);
[\n]+		                    n_lineas+=yyleng; yyless(yyleng); BEGIN(SALTO);
[0-9]+			                yylval.valor_entero = atoi(yytext);imprimirToken(); return NUMERO;
[0-9]+"."[0-9]*	                yylval.valor_real = atof(yytext);imprimirToken(); return REAL;
"<="                            imprimirToken(); return MENORIGUAL;
">="                            imprimirToken(); return MAYORIGUAL;
"=="                            imprimirToken(); return IGUAL;
"!="                            imprimirToken(); return DESIGUAL;
"&&"                            imprimirToken(); return AND;
"||"                            imprimirToken(); return OR;
true                            yylval.valor_booleano = true; imprimirToken(); return VERDADERO;
false                           yylval.valor_booleano = false; imprimirToken(); return FALSO;
escribir                        imprimirToken(); return ESCRIBIR;
DEFINICIONES                    imprimirToken(); return DEFINICIONES;
CONFIGURACION                   imprimirToken(); return CONFIGURACION;
OBSTACULOS                      imprimirToken(); return OBSTACULOS;
EJEMPLOS                        imprimirToken(); return EJEMPLOS;
entero                          imprimirToken(); return TENTERO;
real                            imprimirToken(); return TREAL;
posicion                        imprimirToken(); return TPOSICION;
cadena                          imprimirToken(); return TCADENA;
[dD][iI][mM][eE][nN][sS][iI][oO][nN] imprimirToken(); return DIMENSION;
[eE][nN][tT][rR][aA][dD][aA]         imprimirToken(); return ENTRADA;
[sS][aA][lL][iI][dD][aA]             imprimirToken(); return SALIDA;
[pP][aA][uU][sS][aA]                 imprimirToken(); return PAUSA;
obstaculo                       imprimirToken(); return OBSTACULO;
sur                             imprimirToken(); return SUR;
este                            imprimirToken(); return ESTE;
oeste                           imprimirToken(); return OESTE;
norte                           imprimirToken(); return NORTE;
ejemplo                         imprimirToken(); return EJEMPLO;
finejemplo                      imprimirToken(); return FINEJEMPLO;
repite                          imprimirToken(); return REPITE;
finrepite                       imprimirToken(); return FINREPITE;
si                              imprimirToken(); return SI;
sino                            imprimirToken(); return SINO;
finsi                           imprimirToken(); return FINSI;
entonces                        imprimirToken(); return ENTONCES;
{ident}                         strcpy(yylval.valor_cadena,yytext); imprimirToken(); return ID;
{cadena}                        strcpy(yylval.valor_cadena,&yytext[1]); yylval.valor_cadena[yyleng-2] = '\0'; imprimirToken(); return CADENA;
[\t ]			                ;	
. 				                imprimirToken(); return (yytext[0]);
%%

