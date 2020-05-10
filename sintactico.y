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
#include <vector>
using namespace std;
extern int n_lineas;
extern int yylex();
extern FILE* yyin;
extern FILE* yyout;
NodoAST *raiz;

vector<Error> listaErrores;
Entorno global = new Entorno(NULL);

//definición de procedimientos auxiliares
void yyerror(const char* s)
{         /*    llamada por cada error sintactico de yacc */
      listaErrores.push_back(Error(n_lineas, n_lineas, string("Sintáctico"), string(s),string(s)));
	//cout << "Error sintáctico en línea "<< n_lineas<<": "<< s <<endl;	      
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
      Expresion *expresion;
      Instruccion *instruccion;
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


%type <instruccion> escribir
%type <expresion> expr

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
      | separador definiciones configuracion obstaculos ejemplos {/*Produccion para manejar los espacios en blanco al inicio*/}
      | separador configuracion obstaculos ejemplos              {/*Produccion para manejar los espacios en blanco al inicio*/}
      | configuracion obstaculos ejemplos 
      /*Producciones sin apartado de configuración*/
      | definiciones  obstaculos ejemplos 
      | separador definiciones  obstaculos ejemplos               {/*Produccion para manejar los espacios en blanco al inicio*/}
      | separador  obstaculos ejemplos                            {/*Produccion para manejar los espacios en blanco al inicio*/}
      | obstaculos ejemplos       
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
      ESCRIBIR expr           {  $$ = new Escribir(n_lineas, n_lineas, $2);  $$->ejecutar(&global); }
;

expr:    NUMERO 		      { $$ = new Literal(n_lineas,n_lineas, $1); }
       | REAL                 { $$ = new Literal(n_lineas,n_lineas, $1); }
       | VERDADERO            { $$ = new Literal(n_lineas,n_lineas, $1); }
       | FALSO                { $$ = new Literal(n_lineas,n_lineas, $1); }
       | expr '+' expr        { $$ = new Suma(n_lineas, n_lineas, $1, $3); }
       | expr '-' expr    	{ $$ = new Resta(n_lineas, n_lineas, $1, $3); }            
       | expr '*' expr        { $$ = new Multiplicacion(n_lineas, n_lineas, $1,$3); } 
       | expr '/' expr        { $$ = new Division(n_lineas, n_lineas, $1,$3); } 
       | expr '%' expr        { $$ = new Modulo(n_lineas, n_lineas, $1,$3); } 
       | expr '^' expr        { $$ = new Potencia(n_lineas, n_lineas, $1,$3); } 
       | expr '<' expr        { $$ = new Relacional(n_lineas, n_lineas, $1, "<", $3); }
       | expr '>' expr        { $$ = new Relacional(n_lineas, n_lineas, $1, ">", $3); }
       | expr IGUAL expr      { $$ = new Relacional(n_lineas, n_lineas, $1, "==", $3); }
       | expr DESIGUAL expr   { $$ = new Relacional(n_lineas, n_lineas, $1, "!=", $3); }
       | expr MAYORIGUAL expr { $$ = new Relacional(n_lineas, n_lineas, $1, ">=", $3); }
       | expr MENORIGUAL expr { $$ = new Relacional(n_lineas, n_lineas, $1, "<=", $3); }
       | '!' expr             { $$ = new Negacion(n_lineas,n_lineas,$2); }
       | expr AND expr        { $$ = new Logica(n_lineas, n_lineas, $1, "&&", $3); }
       | expr OR expr         { $$ = new Logica(n_lineas, n_lineas, $1, "||", $3); }
       |'-' expr %prec menos  { $$ = new Menos(n_lineas,n_lineas,$2); }
       | '(' expr ')'         { $$ = $2; }  
       | ID                   { string id($1);   }  
       | CADENA               { string cad($1); $$ = new Literal(n_lineas, n_lineas, cad );}
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
            
            yyparse();
            if(raiz == NULL)
            {
                  printf("Nada que ejecutar\n");                  
            }
            else
            {
                  if(raiz->esInstruccion())
                  {
                        raiz->ejecutar(&global);
                  }
            }
            cout<<"--------------------------------------------------------------------"<<endl;
            /*Ahora imprimimos todos los errores*/
            for(auto e: listaErrores)
            {
                  cout<< e.getMensaje()<<endl;
            }
            cout<<"--------------------------------------------------------------------\n"<<endl;            
            return 0;
      }     
      return 0;
}
