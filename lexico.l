%{
#include "sintactico.h"
#include "simbolo.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
int n_lineas;


%}
%x COM1 
%option noyywrap

DIGITO [0-9]
ident ([a-zA-Z]|_)([a-zA-Z]|_|{DIGITO})*
cadena "\""[^"\""]*"\""
%%

<COM1>\n                       {n_lineas++; BEGIN(0); return (yytext[0]); }
<COM1>.                        {;}   
"#"                            {BEGIN(COM1);}
[0-9]+			                yylval.valor_entero = atoi(yytext);return NUMERO;
[0-9]+"."[0-9]+	                yylval.valor_real = atof(yytext);return REAL;
\n				                n_lineas++; return (yytext[0]);
"<="                            return MENORIGUAL;
">="                            return MAYORIGUAL;
"=="                            return IGUAL;
"!="                            return DESIGUAL;
"&&"                            return AND;
"||"                            return OR;
true                            yylval.valor_booleano = true; return VERDADERO;
false                           yylval.valor_booleano = false; return FALSO;
escribir                        return ESCRIBIR;
DEFINICIONES                    return DEFINICIONES;
CONFIGURACION                   return CONFIGURACION;
OBSTACULOS                      return OBSTACULOS;
EJEMPLOS                        return EJEMPLOS;
entero                          return TENTERO;
real                            return TREAL;
posicion                        return TPOSICION;
cadena                          return TCADENA;
[dD][iI][mM][eE][nN][sS][iI][oO][nN] return DIMENSION;
[eE][nN][tT][rR][aA][dD][aA]         return ENTRADA;
[sS][aA][lL][iI][dD][aA]             return SALIDA;
[pP][aA][uU][sS][aA]                 return PAUSA;
obstaculo                       return OBSTACULO;
sur                             return SUR;
este                            return ESTE;
oeste                           return OESTE;
norte                           return NORTE;
ejemplo                         return EJEMPLO;
finejemplo                      return FINEJEMPLO;
repite                          return REPITE;
finrepite                       return FINREPITE;
si                              return SI;
sino                            return SINO;
finsi                           return FINSI;
entonces                        return ENTONCES;
{ident}                         strcpy(yylval.valor_cadena,yytext); return ID;
{cadena}                        strcpy(yylval.valor_cadena,yytext); return CADENA;
[\t ]			                ;	
. 				                return (yytext[0]);
%%

