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
{
      listaErrores.push_back(Error(n_lineas, n_lineas, string("Sintáctico"), string(s),string(s)));
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
      listaId * lista_id;
      Type * tipo;
      Bloque * bloque_instrucciones;
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
/*Fin*/

%token <valor_entero> NUMERO
%token <valor_real> REAL
%token SALIR
%token <valor_cadena> ID CADENA
%token MAYORIGUAL MENORIGUAL
%token AND OR
%token <valor_booleano> VERDADERO FALSO
%type <expresion> expr coodernada


%type <instruccion> escribir declaracion constante asignacion
%type <instruccion> si instruccion repetir
%type <bloque_instrucciones> bloque lobstaculos obstaculos

%type <instruccion> obstaculo 


%type <lista_id> listaid;
%type <tipo> tipo;

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
      | '\n' definiciones configuracion obstaculos ejemplos {/*Produccion para manejar los espacios en blanco al inicio*/}
      | '\n' configuracion obstaculos ejemplos              {/*Produccion para manejar los espacios en blanco al inicio*/}
      | configuracion obstaculos ejemplos 
      /*Producciones sin apartado de configuración*/
      | definiciones  obstaculos ejemplos 
      | '\n' definiciones  obstaculos ejemplos               {/*Produccion para manejar los espacios en blanco al inicio*/}
      | '\n'  obstaculos ejemplos                            {/*Produccion para manejar los espacios en blanco al inicio*/}
      | obstaculos ejemplos  
      | error '\n' {yyerrok;}  
      ;

definiciones: 
      DEFINICIONES '\n' ldefinicion;

ldefinicion: 
       ldefinicion declaracion
      |ldefinicion constante
      |ldefinicion escribir '\n'
      |ldefinicion error '\n' {yyerrok;}
      |constante 
      |declaracion 
      |escribir '\n'
      |error '\n' {yyerrok;}
;



declaracion:
      tipo listaid '\n'  {$$ = new Declaracion(n_lineas, n_lineas, *$1, $2); }
      ;

tipo:  TENTERO    {$$= new Type(TIPOENTERO);} 
      |TCADENA    {$$= new Type(TIPOCADENA);}
      |TPOSICION  {$$= new Type(TIPOPOSICION);}
      |TREAL      {$$= new Type(TIPOREAL);}
;

listaid: 
      listaid ',' ID    { $$=$1;  string id($3); $$->add(id);}
      |ID               {listaId *l = new listaId(n_lineas); string id($1); l->add(id); $$=l; }
;

constante:
      ID '=' expr  '\n'              { string id($1); $$= new Constante(n_lineas, n_lineas, id, $3); }
     |ID '=' coodernada  '\n'        { string id($1); $$= new Constante(n_lineas, n_lineas, id, $3); }
;


configuracion:
      CONFIGURACION '\n' lconfiguracion
;

lconfiguracion: 
        lconfiguracion dimension
      | lconfiguracion entrada
      | lconfiguracion salida
      | lconfiguracion pausa
      | lconfiguracion escribir
      | lconfiguracion error '\n' {yyerrok; }
      | dimension 
      | entrada
      | salida
      | pausa
      | escribir
      | error '\n' {yyerrok; }
;


dimension:
      DIMENSION expr '\n' {}
;

entrada: 
      ENTRADA expr '\n'
     |ENTRADA coodernada '\n'
;

salida:
      SALIDA expr '\n'
     |SALIDA coodernada '\n'
;

pausa:
      PAUSA expr '\n'
;


/*-------------------------OBSTACULOS--------------------------------*/
obstaculos:
      OBSTACULOS '\n' lobstaculos {$$= $3;}
;

lobstaculos:
       lobstaculos obstaculo  {$$= $1; $$->addInstruccion($2);}
      |obstaculo              {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
;

obstaculo:
       OBSTACULO expr '\n'          {$$= new PonerObstaculo(n_lineas, n_lineas, $2);}
      |OBSTACULO coodernada '\n'    {$$= new PonerObstaculo(n_lineas, n_lineas, $2);}
      |OBSTACULO '\n'               {Literal *tmp = NULL; 
                                    $$= new PonerObstaculo(n_lineas, n_lineas, tmp);
                                    }
      |SUR expr '\n'                {$$=new Obstaculo_movimiento(n_lineas, n_lineas, "sur", $2);}
      |NORTE expr '\n'              {$$=new Obstaculo_movimiento(n_lineas, n_lineas, "norte", $2);}
      |OESTE expr '\n'              {$$=new Obstaculo_movimiento(n_lineas, n_lineas, "oeste", $2);}
      |ESTE expr '\n'               {$$= new Obstaculo_movimiento(n_lineas, n_lineas, "este", $2);}      
      |asignacion '\n'              {$$=$1;}      
      |escribir '\n'                {$$=$1;}
      |repetir '\n'                 {$$=$1;}
      |si '\n'                      {$$=$1;}
      | error '\n'                  {yyerrok; }
;

asignacion:
      ID '=' expr                { string id($1); $$= new Asignacion(n_lineas, n_lineas, id, $3); }
     |ID '=' coodernada          { string id($1); $$= new Asignacion(n_lineas, n_lineas, id, $3); }
;


ejemplos:
      EJEMPLOS '\n' lejemplos
;

lejemplos:
       lejemplos ejemplo
      |lejemplos error '\n' {yyerrok;}
      |ejemplo 
      |error {yyerrok;}
;

ejemplo:
      EJEMPLO ID '\n' bloque FINEJEMPLO '\n'
;

bloque:
       bloque instruccion     {$$= $1; $$->addInstruccion($2);}
      |instruccion            {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
;

instruccion: obstaculo  {$$=$1;}
;

repetir:
      REPITE expr '\n' bloque FINREPITE { $$ = new Repetir(n_lineas, n_lineas, $2, $4);}
;

si:
      SI expr '\n' ENTONCES '\n' bloque %prec SI_SIMPLE FINSI   {Si *sinosi =NULL; $$ = new Si(n_lineas, n_lineas, $2, $6, sinosi); }
     |SI expr '\n' ENTONCES '\n' bloque SINO '\n'  bloque FINSI 
      {
            Literal *condicionTrue = new Literal(n_lineas,n_lineas, true);
            Si *sinosiAux = NULL;
            Si *sinosi = new Si(n_lineas, n_lineas, condicionTrue, $9, sinosiAux);
            $$ = new Si(n_lineas, n_lineas, $2, $6, sinosi);            
      }
;


coodernada:
       '<' expr ',' expr '>'{ $$ = new Posicion(n_lineas, n_lineas, $2, $4); }
;



escribir:
      ESCRIBIR expr           {  $$ = new Escribir(n_lineas, n_lineas, $2); /**/ }
    | ESCRIBIR coodernada     {  $$ = new Escribir(n_lineas, n_lineas, $2); /**/ }
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
       | ID                   { string id($1);  $$ = new Variable(n_lineas, n_lineas, id); }  
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
            if(yyin==0)
            {
                  printf("El fichero %s no existe\n",argv[1]);
                  return 0;
            }

            printf("\nComenzando el analisis.\n");
            global.iniciarTableroEntorno();
            cout<<"--------------------------------------------------------------------"<<endl;
            yyparse();
            if(raiz == NULL)
            {
                  printf("\nNada que ejecutar\n");                  
            }
            else
            {
                  if(raiz->esInstruccion())
                  {
                        raiz->ejecutar(&global);
                  }
            }
            cout<<"--------------------------------------------------------------------"<<endl;
            cout<<global.tabla.getCadenaData()<<endl;
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
