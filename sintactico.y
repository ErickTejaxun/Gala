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
#include <fstream>
using namespace std;
extern int n_lineas;
extern int yylex();
extern FILE* yyin;
extern FILE* yyout;
NodoAST *raiz;
extern int yylineno;

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
      Programa * programa_gala;
} 

%start inicio;

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
%type <programa_gala> programa
%type <bloque_instrucciones> bloque lobstaculos obstaculos lconfiguracion configuracion
%type <bloque_instrucciones> ldefinicion definiciones lejemplos ejemplos 
%type <instruccion> obstaculo  dimension entrada salida pausa 
%type <instruccion> ejemplo 
%type <lista_id> listaid;
%type <tipo> tipo;
%type <bloque_instrucciones> bloque_ejemplo
%type <instruccion> instruccion_ejemplo


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

inicio: programa {raiz = $1;}
;

programa:
        definiciones configuracion obstaculos ejemplos      {$$ = new Programa(n_lineas, n_lineas);$$->setDefiniciones($1); $$->setConfiguraciones($2); $$->setObstaculos($3); $$->setEjemplos($4);}
      | '\n' definiciones configuracion obstaculos ejemplos {$$ = new Programa(n_lineas, n_lineas);$$->setDefiniciones($2); $$->setConfiguraciones($3); $$->setObstaculos($4); $$->setEjemplos($5);}
      | '\n' configuracion obstaculos ejemplos              {$$ = new Programa(n_lineas, n_lineas);$$->setConfiguraciones($2); $$->setObstaculos($3); $$->setEjemplos($4);}
      | configuracion obstaculos ejemplos                   {$$ = new Programa(n_lineas, n_lineas);$$->setConfiguraciones($1); $$->setObstaculos($2); $$->setEjemplos($3);}
      /*--------------Producciones sin apartado de configuración---------*/
      | definiciones  obstaculos ejemplos                   {$$ = new Programa(n_lineas, n_lineas);$$->setDefiniciones($1); $$->setObstaculos($2); $$->setEjemplos($3);}
      | '\n' definiciones  obstaculos ejemplos               {$$ = new Programa(n_lineas, n_lineas);$$->setDefiniciones($2); $$->setObstaculos($3); $$->setEjemplos($4);}      
      | '\n'  obstaculos ejemplos                            {$$ = new Programa(n_lineas, n_lineas);$$->setObstaculos($2); $$->setEjemplos($3);}      
      | obstaculos ejemplos                                  {$$ = new Programa(n_lineas, n_lineas);$$->setObstaculos($1); $$->setEjemplos($2);}  
      | error '\n' {yyerrok;}                                {$$ = new Programa(n_lineas, n_lineas);}  
      ;


/*-----------------------------------AREA DE DEFINICIONES-------------------*/
definiciones: 
      DEFINICIONES '\n' ldefinicion {$$= $3;}
; 

ldefinicion: 
       ldefinicion declaracion   {$$= $1; $$->addInstruccion($2);}
      |ldefinicion constante     {$$= $1; $$->addInstruccion($2);}
      |ldefinicion escribir '\n' {$$= $1; $$->addInstruccion($2);}
      |ldefinicion error '\n' {yyerrok; $$=$1;}
      |constante        {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}   
      |declaracion      {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}   
      |escribir '\n'    {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}   
      |error '\n' {yyerrok; $$= new Bloque(n_lineas,n_lineas); }
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

/*------------------------------AREA DE CONFIGURACION------------------------------------*/
configuracion:
      CONFIGURACION '\n' lconfiguracion {$$=$3;}
;

lconfiguracion: 
        lconfiguracion dimension{$$= $1; $$->addInstruccion($2);}
      | lconfiguracion entrada  {$$= $1; $$->addInstruccion($2);}
      | lconfiguracion salida   {$$= $1; $$->addInstruccion($2);}
      | lconfiguracion pausa    {$$= $1; $$->addInstruccion($2);}    
      | lconfiguracion escribir {$$= $1; $$->addInstruccion($2);}
      | lconfiguracion error '\n' {yyerrok; $$=$1;}
      | dimension  {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
      | entrada    {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
      | salida     {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
      | pausa      {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
      | escribir   {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}          
      | error '\n' {yyerrok; } {yyerrok;$$= new Bloque(n_lineas,n_lineas); }
;


dimension:
      DIMENSION expr '\n' {$$ = new Establecer_dimension(n_lineas, n_lineas, $2);}
;

entrada: 
      ENTRADA expr '\n'       {$$ = new Establecer_entrada(n_lineas, n_lineas, $2);}
     |ENTRADA coodernada '\n' {$$ = new Establecer_entrada(n_lineas, n_lineas, $2);}
;

salida:
      SALIDA expr '\n'        {$$ = new Establecer_salida(n_lineas, n_lineas, $2);}
     |SALIDA coodernada '\n'  {$$ = new Establecer_salida(n_lineas, n_lineas, $2);}
;

pausa:
      PAUSA expr '\n'     {$$ = new Establecer_pausa(n_lineas, n_lineas, $2);}
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
      | error '\n'                  {yyerrok; Obstaculo_movimiento * nuevo =NULL; $$=nuevo; }
;

asignacion:
      ID '=' expr                { string id($1); $$= new Asignacion(n_lineas, n_lineas, id, $3); }
     |ID '=' coodernada          { string id($1); $$= new Asignacion(n_lineas, n_lineas, id, $3); }
;


ejemplos:
      EJEMPLOS '\n' lejemplos {$$ = $3;}
;

lejemplos:
       lejemplos '\n' ejemplo {$$= $1; $$->addInstruccion($3);}
      |lejemplos '\n' error '\n' {yyerrok; $$=$1;}
      |ejemplo  {$$= new Bloque(n_lineas, n_lineas); $$->addInstruccion($1);}
      |error {yyerrok;} {yyerrok;$$= new Bloque(n_lineas, n_lineas); }
;

ejemplo:
      EJEMPLO ID '\n' bloque_ejemplo FINEJEMPLO  {string id($2); $$ = new Ejemplo(n_lineas, n_lineas, id, $4);}      
;


bloque_ejemplo:
       bloque_ejemplo instruccion_ejemplo {$$= $1; $$->addInstruccion($2);}
      |instruccion_ejemplo {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}
      
;
instruccion_ejemplo:
       SUR expr '\n'                {$$=new Movimiento_jugador(n_lineas, n_lineas, "sur", $2);}
      |NORTE expr '\n'              {$$=new Movimiento_jugador(n_lineas, n_lineas, "norte", $2);}
      |OESTE expr '\n'              {$$=new Movimiento_jugador(n_lineas, n_lineas, "oeste", $2);}
      |ESTE expr '\n'               {$$= new Movimiento_jugador(n_lineas, n_lineas, "este", $2);}      
      |asignacion '\n'              {$$=$1;}      
      |escribir '\n'                {$$=$1;}
      |repetir '\n'                 {$$=$1;}
      |si '\n'                      {$$=$1;}
      | error '\n'                  {yyerrok; Movimiento_jugador * nuevo =NULL; $$=nuevo; }


instruccion: obstaculo  {$$=$1;}
;

bloque:
       bloque instruccion     {$$= $1; $$->addInstruccion($2);}      
      |instruccion            {$$= new Bloque(n_lineas,n_lineas); $$->addInstruccion($1);}      
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
void escribirEncabezado(string path);
void escribirFinal(string path);
void crear_fichero(string path);

int main(int argc, char *argv[])
{
      n_lineas = 1;
      yylineno = 1;
      if (argc != 2) 
      {
            printf("Ejecutar de la forma %s fichero_entrada \n",argv[0]);            
      }		
      else       
      {
            yyin=fopen(argv[1],"rt");
            string path_fichero_entrada(argv[1]);
            string path = path_fichero_entrada.substr(0, path_fichero_entrada.find(".")) +".cpp"; 
            cout<< "Archivo de salida: "<<path<<endl;
            n_lineas = 1;
            if(yyin==0)
            {
                  printf("El fichero %s no existe\n",argv[1]);
                  return 0;
            }

            global.iniciarTableroEntorno();
            cout<<"-------------------------------------------------------------------------------------"<<endl;
            yyparse();
            if(raiz == NULL)
            {
                  printf("Nada que ejecutar\n");                  
            }
            else
            {                      
                  crear_fichero(path);            
                  global.path_fichero = path;
                  if(raiz->esInstruccion())
                  {
                        raiz->ejecutar(&global);                                          
                  }              
            }
            cout<<"--------------------------Tabla de símbolos------------------------------------------"<<endl;
            cout<<global.tabla.getCadenaData()<<endl;
            cout<<"--------------------------------------------------------------------------------------"<<endl;
            /*Ahora imprimimos todos los errores*/
            for(auto e: listaErrores)
            {
                  cout<< e.getMensaje()<<endl;
            }
            cout<<"-------------------------------------------------------------------------------------\n"<<endl;                        
            return 0;
      }     
      return 0;
}


void crear_fichero(string path)
{
      ofstream fichero;
      fichero.open (path);
      fichero << "";
      fichero.close();                
}


