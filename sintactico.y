%code requires
{
      #include "simbolo.h"
}
%{
#include <iostream>
#include <math.h>
#include "simbolo.h"
#include <stdio.h>
#include <string.h>

using namespace std;
extern int n_lineas;
extern int yylex();
extern FILE* yyin;
extern FILE* yyout;


//definición de procedimientos auxiliares
void yyerror(const char* s){         /*    llamada por cada error sintactico de yacc */
	cout << "Error sintáctico en línea "<< n_lineas<<": "<< s <<endl;	      
      //fprintf (stderr, "%s\n", s);
} 

%}

%error-verbose

%union
{
      int valor_entero;
      float valor_real;      
      char valor_cadena[100];   
      bool valor_booleano;   
      simbolo* valor_simbolo;
} 

%start programa;

/*Palabras reservadas*/
%token ESCRIBIR DEFINICIONES CONFIGURACION OBSTACULOS EJEMPLOS
/*Palabras reservadas para tipos*/
%token TENTERO TREAL TPOSICION TCADENA
/*Palabras reservadas para dimensiones*/
%token DIMENSION ENTRADA SALIDA PAUSA
/*Palabras reservadas osbtaculos.*/
%token OBSTACULO NORTE SUR ESTE OESTE
/*Bloque ejemplos*/
%token EJEMPLO FINEJEMPLO
/*Ciclos.*/
%token REPITE FINREPITE SI ENTONCES FINSI SINO

%token <valor_entero> NUMERO
%token <valor_real> REAL
%token SALIR
%token <valor_cadena> ID CADENA
%token MAYORIGUAL MENORIGUAL
%token AND OR
%token <valor_booleano> VERDADERO FALSO





%type <valor_simbolo> expr

%right '!'
%left AND OR
%left '<' '>' MENORIGUAL MAYORIGUAL
%left DESIGUAL IGUAL
%left '+' '-'   
%left '%' '*' '/'  
%right '^'
%left menos
%nonassoc SI_SIMPLE
%nonassoc SINO


%%

programa: 
       definiciones configuracion obstaculos ejemplos 
      |configuracion obstaculos ejemplos 
      ;

definiciones: 
      DEFINICIONES separador ldefinicion;

ldefinicion: 
       ldefinicion declaracion
      |ldefinicion constante
      |ldefinicion escribir separador
      |ldefinicion error separador {yyerrok;}
      |constante 
      |declaracion 
      |escribir separador
      |error separador {yyerrok;}
;

separador: 
       separador '\n'
      |'\n'
;

declaracion:
      tipo listaid separador
      ;

tipo:  TENTERO
      |TCADENA
      |TPOSICION
      |TREAL
;

listaid: 
      listaid ',' ID
      |ID
;

constante:
      ID '=' expr  separador
     |ID '=' coodernada  separador
;


configuracion:
      CONFIGURACION separador lconfiguracion
;

lconfiguracion: 
        lconfiguracion dimension
      | lconfiguracion entrada
      | lconfiguracion salida
      | lconfiguracion pausa
      | lconfiguracion escribir
      | lconfiguracion error separador {yyerrok; }
      | dimension 
      | entrada
      | salida
      | pausa
      | escribir
      | error separador {yyerrok; }
;


dimension:
      DIMENSION expr separador
;

entrada: 
      ENTRADA expr separador
     |ENTRADA coodernada separador
;

salida:
      SALIDA expr separador
     |SALIDA coodernada separador
;

pausa:
      PAUSA expr separador
;

/*-------------------------OBSTACULOS--------------------------------*/
obstaculos:
      OBSTACULOS separador lobstaculos
;

lobstaculos:
      lobstaculos obstaculo 
      |obstaculo
;

obstaculo:
       OBSTACULO expr separador
      |OBSTACULO coodernada separador
      |OBSTACULO separador
      |SUR expr separador
      |NORTE expr separador
      |OESTE expr separador
      |ESTE expr separador
      |constante
      |escribir separador
      |repetir separador
      |si separador
      | error separador {yyerrok; }
;



ejemplos:
      EJEMPLOS separador lejemplos
;

lejemplos:
       lejemplos ejemplo
      |lejemplos error separador {yyerrok;}
      |ejemplo 
      |error {yyerrok;}
;

ejemplo:
      EJEMPLO ID separador bloque FINEJEMPLO separador
;

bloque:
       bloque instruccion
      |instruccion 
;

instruccion:
       obstaculo
      
;

repetir:
      REPITE expr separador bloque FINREPITE
;

si:
      SI expr separador ENTONCES separador bloque %prec SI_SIMPLE FINSI
     |SI expr separador ENTONCES separador bloque SINO separador  bloque FINSI
;


coodernada:
       '<' expr ',' expr '>'{ ; }
;



escribir:
      ESCRIBIR expr           { ;  }
;

expr:    NUMERO 		      { ; }
       | REAL                 { ; }
       | VERDADERO            { ; }
       | FALSO                { ; }
       | expr '+' expr        { ; }
       | expr '-' expr    	{ ; }            
       | expr '*' expr        { ; } 
       | expr '/' expr        { ; } 
       | expr '%' expr        { ; } 
       | expr '^' expr        { ; } 
       | expr '<' expr        { ; }
       | expr '>' expr        { ; }
       | expr IGUAL expr      { ; }
       | expr DESIGUAL expr   { ; }
       | expr MAYORIGUAL expr { ; }
       | expr MENORIGUAL expr { ; }
       | '!' expr             { ; }
       | expr AND expr        { ; }
       | expr OR expr         { ; }
       |'-' expr %prec menos  { ; }
       | '(' expr ')'         { ; }  
       | ID                   { ; }  
       | CADENA               { ; }       
       ;
%%

int main(int argc, char *argv[])
{
      n_lineas = 1;
      if (argc != 3) 
      {
            printf("Ejecutar de la forma %s entrada salida\n",argv[0]);            
      }		
      else       
      {
            yyin=fopen(argv[1],"rt");           
            yyout = fopen(argv[2],"wt");
            n_lineas = 1;
            cout<<"--------------------------------------------------------------------"<<endl;
            yyparse();
            cout<<"--------------------------------------------------------------------"<<endl;
            //string dataTabla = tablaSimbolo->getCadenaData();            
            //fputs(dataTabla.data(), yyout);
            fprintf(yyout, "\n");
            return 0;
      }     
      return 0;
}
