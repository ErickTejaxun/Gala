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
    if(yylineno==1)
    {
        printf("\n%i)\t",yylineno);
    }
    printf("%s ", yytext);
}


%}
%x COM1 COM SALTO
%option noyywrap
%option yylineno

DIGITO [0-9]
ident ([a-zA-Z]|_)([a-zA-Z]|_|{DIGITO})*
cadena "\""[^"\""]*"\""
%%

<COM1>[\n]+                    {n_lineas+=yyleng; BEGIN(COM);}
<COM1>.                        {;}   

<COM>\n                        {n_lineas++;}
<COM>#                         {BEGIN(COM1);}
<COM>[\t ]			           ;
<COM>.                         {yyless(0); BEGIN(SALTO);}

<SALTO>#                       {BEGIN(COM1);}
<SALTO>[\t ]			       ;
<SALTO>.                       {yyless(0);BEGIN(0); /*printf("\n%i)\t",n_lineas);*/ return('\n');}

#    		                    BEGIN(COM1);
[\n]+		                    {n_lineas+=yyleng;BEGIN(SALTO);}
[0-9]+			                yylval.valor_entero = atoi(yytext);  return NUMERO;
[0-9]+"."[0-9]*	                yylval.valor_real = atof(yytext);  return REAL;
"<="                             return MENORIGUAL;
">="                             return MAYORIGUAL;
"=="                             return IGUAL;
"!="                             return DESIGUAL;
"&&"                             return AND;
"||"                             return OR;
true                            yylval.valor_booleano = true;   return VERDADERO;
false                           yylval.valor_booleano = false;   return FALSO;
escribir                         return ESCRIBIR;
DEFINICIONES                     return DEFINICIONES;
CONFIGURACION                    return CONFIGURACION;
OBSTACULOS                       return OBSTACULOS;
EJEMPLOS                         return EJEMPLOS;
entero                           return TENTERO;
real                             return TREAL;
posicion                         return TPOSICION;
cadena                           return TCADENA;
[dD][iI][mM][eE][nN][sS][iI][oO][nN]   return DIMENSION;
[eE][nN][tT][rR][aA][dD][aA]           return ENTRADA;
[sS][aA][lL][iI][dD][aA]               return SALIDA;
[pP][aA][uU][sS][aA]                   return PAUSA;
obstaculo                         return OBSTACULO;
sur                               return SUR;
este                              return ESTE;
oeste                             return OESTE;
norte                             return NORTE;
ejemplo                           return EJEMPLO;
finejemplo                        return FINEJEMPLO;
repite                            return REPITE;
finrepite                         return FINREPITE;
si                                return SI;
sino                              return SINO;
finsi                             return FINSI;
entonces                          return ENTONCES;
{ident}                         strcpy(yylval.valor_cadena,yytext);   return ID;
{cadena}                        {strcpy(yylval.valor_cadena,&yytext[1]); yylval.valor_cadena[yyleng-2] = '\0';   return CADENA;/*Quitamos las comilas*/}
[\t ]			                ;	
<<EOF>>                         yyterminate();
. 				                  return (yytext[0]);
%%

