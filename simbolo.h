/*
Estructuras para el manejo e implementación de la tabla de símbolos
Erick Tejaxún 
Teoría de Lenguajes UEx 2020
*/

#ifndef simbolo_H
# define simbolo_H
#include <string.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;
class Expresion;
class Entorno;
class Error;


/* Variables globales de configuración del tablero*/
extern vector<Error> listaErrores;


enum TYPES { TIPOENTERO, TIPOREAL, TIPOLOGICO, TIPOCADENA, TIPOPOSICION, TIPOERROR};
enum ESTADO { CASILLA_VACIA, CASILLA_OBSTACULO, CASILLA_SALIDA, CASILLA_ENTRADA };
enum ESTADO_JUGADOR {NO_GANADO, GANADO, PERDIDO};
class Error
{
    public:
        int linea;
        int columna;
        string descripcion;
        string tipo;
        string id;
        Error(int l, int c, string t, string i, string desc):linea(l), columna(c), tipo(t), descripcion(desc), id(i){linea = l; columna = c;}        
        string getMensaje()
        {
            return "Error " + tipo + " en línea "+ to_string(linea) + ":\t" +descripcion;
        }

        static void registrarErrorSemantico(int l, int c, string i, string d)
        {
            //printf("Registrando error\n");
            listaErrores.push_back(Error(l,c,string("Semántico"),i,d));
        }
};


class listaId
{
    public:
        vector<string> data;
        int linea ;
        listaId(int l)
        {
            linea = l;
        }
        void add(string s)
        {
            data.push_back(s);
        }   
};

class Type
{        
    public:
        TYPES tipo;
        Type(TYPES t)
        {
            tipo = t;           
        }  

        Type(){tipo = TIPOERROR;}
        TYPES getTipo(){return tipo;}
        void setTipo(TYPES t){tipo = t;}

        bool esEntero(){ return tipo == TIPOENTERO;};        
        bool esReal(){ return tipo ==TIPOREAL;};        
        bool esLogico(){ return tipo == TIPOLOGICO;};        
        bool esCadena(){ return tipo ==TIPOCADENA;}        
        bool esPosicion(){return tipo==TIPOPOSICION;}  
        bool esError(){return tipo==TIPOERROR;}   
        bool esNumerico(){ return tipo == TIPOENTERO|| tipo ==TIPOREAL;}                      
        string getNombre()
        {
            if(esEntero()){return "Entero";}
            if(esReal()){return "Real";}
            if(esLogico()){return "Lógico";}
            if(esCadena()){return "Cadena";}
            if(esPosicion()){return "Posicion";}
            if(esError()){return "Error";}
        }


};


class simbolo 
{
    public:       
        Type tipo = Type();
        int valor_entero=0;
        float valor_real=0;   
        bool valor_booleano=0;
        int  valor_posicion[2];
        string valor_cadena;
        string id ="";
        int linea;
        int columna;
        bool constante = false;
        bool simbolo_juego = false;
        /*Constructores*/
        simbolo() {} ;
        simbolo(string i, int v, int l, int c): id(i), valor_entero(v), linea(l), columna(c){tipo = Type(TIPOENTERO);};
        simbolo(string i, float v, int l, int c): id(i), valor_real(v), linea(l), columna(c){tipo = Type(TIPOREAL);};
        simbolo(string i, bool v, int l, int c): id(i), valor_booleano(v), linea(l), columna(c){tipo = Type(TIPOLOGICO);};                
        simbolo(string i, string v, int l, int c): id(i), valor_cadena(v), linea(l), columna(c){tipo = Type(TIPOCADENA);};
        simbolo(string i, int y, int x, int l, int c): id(i), linea(l), columna(c){ tipo =Type(TIPOPOSICION); valor_posicion[0] = y; valor_posicion[1] = x;}
        string getCadenaLogico(){return valor_booleano? "verdadero": "falso";}   
        Type getTipo(Entorno *e){ return tipo;}    

        /*Manejo de consantes*/
        bool esConstante(){return constante;}        
        string rol()
        {
            if(constante)
            {
                return "Constante";
            }
            return "Variable";
        }
};


class TablaSimbolos
{
    public:
        map<string, simbolo> tabla;  

        /*Esta función devuelve una cadena con el reporte de la tabla de símbolos. */ 
        string getCadenaData()
        {
            string DATACADENA = "Nombre\t\t\tTipo\t\t\tRol\t\t\tValor";
            map<string, simbolo>::iterator item;
            for(item = tabla.begin(); item != tabla.end(); item++)
            {
                if(!item->second.simbolo_juego)
                {
                    switch (item->second.tipo.getTipo())
                    {
                        case TIPOENTERO:
                            DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                            +item->second.rol()+"\t\t\t"+to_string(item->second.valor_entero);
                            break;                
                        case TIPOREAL:
                            DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                            +item->second.rol()+"\t\t\t"+to_string(item->second.valor_real);
                            break;                
                        case TIPOLOGICO:
                            DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                            +item->second.rol()+"\t\t\t"+item->second.getCadenaLogico();
                            break;   
                        case TIPOCADENA:
                            DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                            +item->second.rol()+"\t\t\t"+item->second.valor_cadena;
                            break;
                        case TIPOPOSICION:
                            DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                            +item->second.rol()+"\t\t\ty:" +to_string(item->second.valor_posicion[0]) +"   x: " +to_string(item->second.valor_posicion[1]);
                            break;                       
                        default:
                            break;
                    }                    
                }                
            }
            return DATACADENA;
        }

        /*Este método busca un símbolo (variable/constante)
        *string id identificador del símbolo
        *int linea Linea desde donde se ejecuta la búsqueda, para reporte de errores.
        */
        simbolo * obtenerSimbolo(string id, int linea)
        {                        
            auto it = tabla.find(id);            
            if (it != tabla.end())
            {
                simbolo *tmp = & it->second;     
                return tmp;            
            }
            simbolo *tmp = new simbolo();
            tmp->tipo.setTipo(TIPOERROR);
            Error::registrarErrorSemantico(linea, linea, id, "Símbolo " +id+" no encontrado en el entorno actual.");                
            return tmp;            
        }   

        /*Este método busca un símbolo (variable/constante) pero sin reporte de errores
        Sirve para el manejo interno de las variables del juego.
        *string id identificador del símbolo
        *int linea Linea desde donde se ejecuta la búsqueda
        */
        simbolo * obtenerSimboloLocal(string id, int linea)
        {                        
            auto it = tabla.find(id);            
            if (it != tabla.end())
            {
                simbolo *tmp = & it->second;     
                return tmp;            
            }            
            return NULL;            
        }              

        /*Este método regista un nuevo símbolo en la tabla de símbolos haciendo las validaciones semánticas necesarias.
        * simbolo *s  Contiene todo los datos del nuevo símbolo a registrar.
        */
        int insertarSimbolo(simbolo *s)
        {                           
            simbolo simb = simbolo();
            simb.id = s->id; 
            simb.linea = s->linea; 
            simb.columna = s->columna; 
            simb.tipo= Type(s->tipo.tipo);
            simb.constante = s->constante;
            simb.simbolo_juego = s->simbolo_juego;
            switch (s->tipo.getTipo())
            {
                case TIPOENTERO:
                    simb.valor_entero = s->valor_entero;                                  
                    break;
                case TIPOREAL:           
                    simb.valor_real = s->valor_real;                    
                    break;       
                case TIPOLOGICO:             
                    simb.valor_booleano = s->valor_booleano;
                    break;   
                case TIPOCADENA:
                    simb.valor_cadena = s->valor_cadena;
                    break;
                case TIPOPOSICION:
                    simb.valor_posicion[0] = s->valor_posicion[0];
                    simb.valor_posicion[1] = s->valor_posicion[1];
                    break;                                                                    
            }  

            /*Vemos si existe el símbolo*/
            simbolo *tmp = obtenerSimboloLocal(s->id, s->linea);         
            if(tmp!=NULL)
            {
                Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                    "La "+tmp->rol() +" "+s->id+" ya ha sido declarada");
                return 0;
            }                        
            tabla.insert( pair<string,simbolo>(s->id,simb));
            return 1;
        }

        /*Método que permite actualizar el valor de un símbolo variable. Realiza todas las verificaciones necesarias.
        simbolo *s contiene el identificador, valor a actualizar, tipo del valor a actualizar y línea desde donde se ejecuta.
        */
        int actualizarSimbolo(simbolo *s)
        {
            simbolo *tmp = obtenerSimbolo(s->id, s->linea);
            if(tmp!=NULL)
            {
                if(tmp->constante)
                {
                    Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                        "Error en asignación: El valor del símbolo " +s->id+ " es constante ");
                    return -1;
                }
                if(s->tipo.tipo == tmp->tipo.tipo)
                {
                    s->linea = tmp->linea;
                    switch (s->tipo.getTipo())
                    {
                        case TIPOENTERO:
                            tmp->valor_entero = s->valor_entero;                                  
                            break;
                        case TIPOREAL:           
                            tmp->valor_real = s->valor_real;                    
                            break;       
                        case TIPOLOGICO:             
                            tmp->valor_booleano = s->valor_booleano;
                            break;   
                        case TIPOCADENA:
                            tmp->valor_cadena = s->valor_cadena;
                            break;
                        case TIPOPOSICION:
                            tmp->valor_posicion[0] = s->valor_posicion[0];
                            tmp->valor_posicion[1] = s->valor_posicion[1];
                            break;                                                                    
                    }                    
                }           
                else
                {
                    //Otra posibilidad es que un entero se asigne a un real
                    if(s->tipo.esNumerico() && tmp->tipo.esNumerico())
                    {
                        if(s->tipo.esReal() && tmp->tipo.esEntero())
                        {
                            tmp->valor_entero = s->valor_real;
                        }
                        else
                        {
                            tmp->valor_real = s->valor_entero;
                        }
                    }
                    else
                    {
                        Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                            "La variable " +s->id+ " es de tipo " +tmp->tipo.getNombre()+", Error tratando de asignar un valor de tipo " +s->tipo.getNombre());                    
                    }                    
                } 
            }
            else
            {
                    Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                        "La variable " +s->id+ " no ha sido declarada .");                   
            }
        }
};

/*clase para manejar los entornos para futuras expansiones cuando sea necesario 
inducir nuevos Entornos*/
class Entorno
{
    public:
        TablaSimbolos tabla;
        Entorno *padre; /*Puntero hacia el entorno padre.*/
        string path_fichero = "";
        ofstream fichero;
        int *tablero;
        int estado_juego = NO_GANADO;
        bool puede_continuar = true;
        Entorno(Entorno * p)
        {
            padre = p;
            tabla = TablaSimbolos();
            if(padre==NULL)
            {
                iniciarTableroEntorno();
            }
        }     

        /*Este método inicializa las variables que */
        void iniciarTableroEntorno()
        {
            simbolo *tamano_tab = new simbolo("TAMANO_TABLERO_INIT",10, 0,0);            
            simbolo *pausa = new simbolo("PAUSA_TABLERO",0,0,0);    
            pausa->valor_real = 0.5;
            pausa->tipo = Type(TIPOREAL);
            simbolo *entrada_tablero = new simbolo("ENTRADA_TABLERO_INIT",0,0,0,0);
            simbolo *salida_tablero = new simbolo("SALIDA_TABLERO_INIT",9,9, 0,0);
            simbolo *posicion_relativa = new simbolo("POSICION_RELATIVA_OBSTACULO",1,0,0,0); 
            tamano_tab->simbolo_juego = true;
            tabla.insertarSimbolo(tamano_tab);
            pausa->simbolo_juego = true;
            tabla.insertarSimbolo(pausa);
            entrada_tablero->simbolo_juego = true;
            tabla.insertarSimbolo(entrada_tablero);
            salida_tablero->simbolo_juego = true;
            tabla.insertarSimbolo(salida_tablero);
            posicion_relativa->simbolo_juego = true;
            tabla.insertarSimbolo(posicion_relativa);
            tablero = new int[10*10];
            for(int i =0; i < 10*10; i++)
            {
            	tablero[i] = CASILLA_VACIA;
            }
        } 
        void abrirFichero()
        {
            /*Primero limpiamos el archivo*/
            ofstream fichero2;
            fichero2.open (path_fichero);
            fichero2 << " ";
            fichero2.close();

            /*Abrimos en modo de escritura*/
            fichero.open (path_fichero, ios::app);
        }

        void cerrarFichero()
        {
        	fichero.close();
        }
        void escribir_fichero(string cadena)
        {
        	//cout<<<cadena<<endl;
            fichero << cadena;            
        } 

};

/*Clase NodoAST es una clase abstracta que nos permitirá definir las principales operaciones
que tienen que realizar los Nodos del AST como salida del análisis sintáctico.*/
class NodoAST
{
    public:
        //virtual ~NodoAST() { };
        virtual bool esInstruccion() = 0;
        virtual simbolo getValor(Entorno *e) = 0;
        virtual void ejecutar(Entorno *e) = 0;
        int linea, columna;        
        int getLinea() { return linea;}
        int getColumna() { return columna;}
};

/*
Toda parte de código que devuelve un valor explícito y tiene un tipo asociado.
*/

class Expresion: public NodoAST
{
    public:  
        Type tipo; 
        simbolo valor; 
        void ejecutar(Entorno *e) override {;}       
        virtual Type getTipo(Entorno *e) = 0;
        bool esInstruccion(){ return false;}   
        //virtual ~Expresion() { };      
};

/*Representa toda instrucción soportadas por nuestro lenguaje*/
class Instruccion: public NodoAST
{
    public:
        simbolo getValor(Entorno *e) override{;}
        bool esInstruccion(){ return true;}   
        //virtual ~Instruccion() { };           
};

/*Expresiones------------------------------------------------------------>*/
class Literal: public Expresion
{
    public:
        Literal(int l, int c, int val)
        {
            valor = simbolo("",val,l,c);            
            linea = l;
            columna = c;            
        }
        /*booleano*/
        Literal(int l, int c, bool val)
        {
            valor = simbolo("",val,l,c);
            linea = l;
            columna = c; 
                       
        }
        /*TIPOREAL*/
        Literal(int l, int c, float val)
        {
            valor = simbolo("",val,l,c);
            linea = l;
            columna = c;            
        }
        /*TIPOCADENA*/
        Literal(int l, int c, string val)
        {
            valor = simbolo("",val,l,c);
            linea = l;
            columna = c;            
        }

        simbolo getValor(Entorno *e) override
        {
            return valor;
        }

        Type getTipo (Entorno *e) override
        {
            return valor.tipo;
        }
};

class Posicion: public Expresion
{
    public: 
        Expresion *y_coordenada;
        Expresion *x_coordenada;
        Posicion(int l, int c, Expresion *y, Expresion *x)
        {
            linea = l;
            columna = c;
            y_coordenada = y;
            x_coordenada = x;
        }        

        simbolo getValor(Entorno *e) override
        {
            simbolo valor = simbolo();
            valor.linea = linea;
            valor.columna = columna;
            valor.tipo = getTipo(e);
            if(!valor.tipo.esError())
            {
                valor.valor_posicion[0] = y_coordenada->getTipo(e).esReal() ? y_coordenada->getValor(e).valor_real: y_coordenada->getValor(e).valor_entero;
                valor.valor_posicion[1] = x_coordenada->getTipo(e).esReal() ? x_coordenada->getValor(e).valor_real: x_coordenada->getValor(e).valor_entero;
            }
            return valor;
        }

        Type getTipo(Entorno *e) override
        {
            if(y_coordenada->getTipo(e).esNumerico() && x_coordenada->getTipo(e).esNumerico())
            {
                return Type(TIPOPOSICION);
            }
            return Type(TIPOERROR);
        }
};


class Suma: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
               
        Suma(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {            
            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }              
            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);

            if(tmp.esCadena()) // Resultado TIPOCADENA
            {                
                string cad1, cad2;
                /*Obtenemos el valor del operando izquierdo*/
                if(valor_izquierdo.getTipo(e).esCadena()){cad1 = valor_izquierdo.valor_cadena;}
                if(valor_izquierdo.getTipo(e).esEntero()){cad1 = to_string(valor_izquierdo.valor_entero);}
                if(valor_izquierdo.getTipo(e).esReal()){cad1 = to_string(valor_izquierdo.valor_real);}
                if(valor_izquierdo.getTipo(e).esLogico()){cad1 = valor_izquierdo.getCadenaLogico();}
                if(valor_izquierdo.getTipo(e).esPosicion()){cad1 = to_string(valor_izquierdo.valor_entero);}

                /*Obtenemos el valor del operando derecho*/
                if(valor_derecho.getTipo(e).esCadena()){cad2 = valor_derecho.valor_cadena;}
                if(valor_derecho.getTipo(e).esEntero()){cad2 = to_string(valor_derecho.valor_entero);}
                if(valor_derecho.getTipo(e).esReal()){cad2 = to_string(valor_derecho.valor_real);}
                if(valor_derecho.getTipo(e).esLogico()){cad2 = valor_derecho.getCadenaLogico();}
                if(valor_derecho.getTipo(e).esPosicion()){cad2 = to_string(valor_derecho.valor_entero);}                                                
                simbolo resultado = simbolo("",cad1+cad2, linea, columna);                
                resultado.tipo = Type(TIPOCADENA);
                //cout <<"---------------------<"<< cad1 << cad2 <<endl;
                return resultado;
            }            
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = tmp;

            if(tmp.esReal())
            {
                
                resultado.valor_real = (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero) +
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero);                                                    
            }                       
            if(tmp.esEntero())
            {                                
                resultado.valor_entero = valor_izquierdo.valor_entero + valor_derecho.valor_entero;                                
            }                

            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            
            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho =  derecho->getTipo(e);
            if(!tipoIzquierdo.esError() && !tipoDerecho.esError())         
            {
                if(tipoIzquierdo.esCadena() || tipoDerecho.esCadena())
                {
                    return Type(TIPOCADENA);
                }
                if(tipoIzquierdo.esLogico() || tipoDerecho.esLogico())
                {              
                    Error::registrarErrorSemantico(linea, columna," " ,
                    "Error en operación (+): No es permitida la operación " + tipoIzquierdo.getNombre() + " + " + tipoDerecho.getNombre());                             
                    return Type(TIPOERROR); // TIPOERROR;
                }
                if(tipoIzquierdo.esReal() || tipoDerecho.esReal())
                {
                    return Type(TIPOREAL);
                }
                if(tipoIzquierdo.esEntero() ||tipoDerecho.esEntero())
                {
                    return Type(TIPOENTERO);
                }                
            }            
            //Error::registrarErrorSemantico(linea, columna," " ,
            //"Error en operación (+): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " + " + derecho->getTipo(e).getNombre()); 
            return Type(TIPOERROR);
        }
};



class Resta: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
               
        Resta(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {
            Type tmp = getTipo(e); 
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }

            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = tmp;
            if(resultado.getTipo(e).esReal()) // TIPOREAL
            {
                
                resultado.valor_real = (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero) -
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero);                                    
            }            
            if(resultado.getTipo(e).esEntero())
            {                                
                resultado.valor_entero = valor_izquierdo.valor_entero - valor_derecho.valor_entero;                                
            }     
                                                 
            return resultado;


        }

        Type getTipo(Entorno *e) override
        {
            Type tipoIzquierdo =izquierdo->getTipo(e);
            Type tipoDerecho =  derecho->getTipo(e);
            if(tipoIzquierdo.esError() || tipoDerecho.esError())
            {
                return Type(TIPOERROR);
            }            
            if(tipoIzquierdo.esCadena() || tipoDerecho.esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (-): No es permitida la operación " + tipoIzquierdo.getNombre() + " - " + tipoDerecho.getNombre());                
                return Type(TIPOERROR);
            }
            if(tipoIzquierdo.esLogico() || tipoDerecho.esLogico())
            {   
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (-): No es permitida la operación " + tipoIzquierdo.getNombre() + " - " + tipoDerecho.getNombre());                                        
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(tipoIzquierdo.esReal() || tipoDerecho.esReal())
            {
                return Type(TIPOREAL);
            }
            if(tipoIzquierdo.esEntero() || tipoDerecho.esEntero())
            {
                return Type(TIPOENTERO);
            }
        }
};


class Multiplicacion: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
               
        Multiplicacion(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {
            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }

            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esReal()) // TIPOREAL
            {
                
                resultado.valor_real = (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero) *
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero);                                    
            }            
            if(resultado.getTipo(e).esEntero())
            {                                
                resultado.valor_entero = valor_izquierdo.valor_entero * valor_derecho.valor_entero;                                
            }       
            return resultado;


        }

        Type getTipo(Entorno *e) override
        {
            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho = derecho->getTipo(e);
            if(tipoIzquierdo.esError() || tipoDerecho.esError())
            {
                return Type(TIPOERROR);
            }             
            if(tipoIzquierdo.esCadena() || tipoDerecho.esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (*): No es permitida la operación " + tipoIzquierdo.getNombre() + " * " + tipoDerecho.getNombre());                 
                return Type(TIPOERROR);
            }
            if(tipoIzquierdo.esLogico() || tipoDerecho.esLogico())
            {                   
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (*): No es permitida la operación " + tipoIzquierdo.getNombre() + " * " + tipoDerecho.getNombre());                         
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(tipoIzquierdo.esReal() || tipoDerecho.esReal())
            {
                return Type(TIPOREAL);
            }
            if(tipoIzquierdo.esEntero() ||tipoDerecho.esEntero())
            {
                return Type(TIPOENTERO);
            }

        }
};


class Division: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
               
        Division(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {

            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }

            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esReal()) // TIPOREAL
            {
                
                resultado.valor_real = (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero) /
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero);                                    
            }            
            if(resultado.getTipo(e).esEntero())
            {                                
                resultado.valor_entero = valor_izquierdo.valor_entero / valor_derecho.valor_entero;                                
            }   

            return resultado;


        }

        Type getTipo(Entorno *e) override
        {

            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho = derecho->getTipo(e);
            if(tipoIzquierdo.esError() || tipoDerecho.esError())
            {
                return Type(TIPOERROR);
            }   
            if(tipoIzquierdo.esCadena() || tipoDerecho.esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (/): No es permitida la operación " +  tipoIzquierdo.getNombre() + " / " + tipoDerecho.getNombre());                
                return Type(TIPOERROR);
            }
            if(tipoIzquierdo.esLogico() || tipoDerecho.esLogico())
            {            
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (/): No es permitida la operación " +  tipoIzquierdo.getNombre() + " / " +  tipoDerecho.getNombre());                               
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(tipoIzquierdo.esReal() || tipoDerecho.esReal())
            {
                return Type(TIPOREAL);
            }
            if(tipoIzquierdo.esEntero() ||tipoDerecho.esEntero())
            {
                return Type(TIPOENTERO);
            }

        }
};





class Potencia: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
               
        Potencia(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {

            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }

            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esReal()) // TIPOREAL
            {
                
                resultado.valor_real = pow (
                                        (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero),
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero));                           
            }    
            return resultado;


        }

        Type getTipo(Entorno *e) override
        {

            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho = derecho->getTipo(e);
            if(tipoIzquierdo.esError() || tipoDerecho.esError())
            {
                return Type(TIPOERROR);
            } 

            if(tipoIzquierdo.esCadena() || tipoDerecho.esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (^): No es permitida la operación " + tipoIzquierdo.getNombre() + " ^ " + tipoDerecho.getNombre());                
                return Type(TIPOERROR);
            }
            if(tipoIzquierdo.esLogico() || tipoDerecho.esLogico())
            {       
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (^): No es permitida la operación " + tipoIzquierdo.getNombre() + " ^ " + tipoDerecho.getNombre());                                    
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(tipoIzquierdo.esReal() || tipoDerecho.esReal())
            {
                return Type(TIPOREAL);
            }
            if(tipoIzquierdo.esEntero() ||tipoDerecho.esEntero())
            {
                return Type(TIPOREAL);
            }

        }
};



class Modulo: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho; 
        Modulo(int l, int c, Expresion *i, Expresion *d)
        {
            izquierdo = i;
            derecho  = d;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {
            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }            
            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esEntero()) // TIPOREAL
            {
                
                resultado.valor_entero = valor_izquierdo.valor_entero % valor_derecho.valor_entero;
            }   
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {

            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho = derecho->getTipo(e);
            if(tipoIzquierdo.esEntero() && tipoDerecho.esEntero())
            {
                return Type(TIPOENTERO);
            }
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (%): No es permitida la operación " + tipoIzquierdo.getNombre() + " % " + tipoDerecho.getNombre());                
            return Type(TIPOERROR);            

        }
};

class Menos: public Expresion
{
    public:
        Expresion *expr ;     
               
        Menos(int l, int c, Expresion *e)
        {            
            expr = e;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {
            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }    

            simbolo valor = expr->getValor(e);                        
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = tmp;
            resultado.valor_entero = valor.valor_entero * -1;
            resultado.valor_real = valor.valor_real * -1;                                 
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            Type tipo =expr->getTipo(e);
            if(tipo.esReal())
            {
                return Type(TIPOREAL);
            }
            else
            if(tipo.esEntero())
            {
                return Type(TIPOENTERO);
            }    
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (-): No es permitida la operación  - " + tipo.getNombre()); 
            return Type(TIPOERROR);        

        }
};


class Relacional: public Expresion
{
    public:
        Expresion *izquierdo;
        Expresion *derecho;  
        string operacion;
        Relacional(int l, int c, Expresion * i, string op,  Expresion *d)
        {
            izquierdo = i;
            derecho = d;
            linea = l;
            columna = c;
            operacion = op;
        }

        simbolo getValor(Entorno *e)
        {
            simbolo resultado = simbolo();
            resultado.linea = linea;
            resultado.columna = columna;
            resultado.tipo = getTipo(e);                        
            if(resultado.tipo.esLogico())
            {
                Type tipo_izquierdo = izquierdo->getTipo(e);
                Type tipo_derecho = derecho->getTipo(e);

                if(operacion=="<")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real < (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero < (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }
                if(operacion==">")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real > (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero > (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }   
                if(operacion==">=")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real >= (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero >= (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }                               
                if(operacion=="<=")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real <= (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero <= (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }    
                if(operacion=="==")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real == (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero == (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }  
                if(operacion=="!=")
                {
                    resultado.valor_booleano = tipo_izquierdo.esReal() ? 
                        (izquierdo->getValor(e).valor_real != (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero )):
                        (izquierdo->getValor(e).valor_entero != (tipo_derecho.esReal()? derecho->getValor(e).valor_real: derecho->getValor(e).valor_entero ));
                }                                               
            }
            //printf("Resultado %d\n",resultado->tipo);
            return resultado;      
        } 

        Type getTipo(Entorno *e) override
        {
            Type tipoIzquierdo = izquierdo->getTipo(e);
            Type tipoDerecho = derecho->getTipo(e);
            if( tipoIzquierdo.esNumerico() && tipoDerecho.esNumerico())
            {
                return Type(TIPOLOGICO);
            }
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación ("+operacion+"): No es permitida la operación " + tipoIzquierdo.getNombre() + " "+operacion+" " + tipoDerecho.getNombre());            
            return Type(TIPOERROR);
        }       
};


class Negacion: public Expresion
{
    public:
        Expresion *expr ;     
               
        Negacion(int l, int c, Expresion *e)
        {            
            expr = e;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {
            Type tmp = getTipo(e);   
            if(tmp.esError())
            {
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = tmp;
                return resultado;
            }  

            simbolo valor = expr->getValor(e);
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = tmp;
            resultado.valor_booleano =  ! valor.valor_booleano;
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            Type tipo = expr->getTipo(e);
            if(tipo.esLogico())
            {
                return Type(TIPOLOGICO);
            }   
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (!): No es permitida la operación  ! " + tipo.getNombre()); 
            return Type(TIPOERROR);        

        }
};


class Logica: public Expresion
{
    public:
        Expresion *izquierda ;     
        Expresion *derecha;
        string operacion;
               
        Logica(int l, int c, Expresion *i, string op, Expresion *d)
        {            
            izquierda = i;
            derecha = d;
            linea = l;
            columna = c;
            operacion = op;
        }

        simbolo getValor(Entorno *e) override
        {            
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(!resultado.tipo.esError())
            {
                if(operacion == "||")
                {
                    resultado.valor_booleano =  izquierda->getValor(e).valor_booleano || derecha->getValor(e).valor_booleano ;
                }            
                if(operacion == "&&")
                {
                    resultado.valor_booleano =  izquierda->getValor(e).valor_booleano && derecha->getValor(e).valor_booleano ;
                } 
            }                       
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            Type tipoIzquierdo = izquierda->getTipo(e);
            Type tipoDerecho = derecha->getTipo(e);
            if(tipoIzquierdo.esLogico()  && tipoDerecho.esLogico())
            {
                return Type(TIPOLOGICO);
            }   
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación ("+operacion+"): No es permitida la operación  "+ tipoIzquierdo.getNombre() + " " + operacion + " "+ tipoDerecho.getNombre()); 
            return Type(TIPOERROR);        

        }
};


class Variable: public Expresion
{
    public:
        string id;
        Variable(int l, int c, string i)
        {
            id = i;
            linea = l;
            columna = c;
        }

        simbolo getValor(Entorno *e) override
        {            
            simbolo *tmp = e->tabla.obtenerSimbolo(id, linea);
            if(tmp==NULL)
            {                
                simbolo resultado = simbolo(); 
                resultado.linea = linea; 
                resultado.columna = columna; 
                resultado.tipo = Type(TIPOERROR);
                return resultado;                
            }            
            return *tmp;
        }

        Type getTipo(Entorno *e) override
        {
            simbolo *tmp = e->tabla.obtenerSimbolo(id, linea);
            if(tmp==NULL)
            {                
                return Type(TIPOERROR);
            }
            return tmp->tipo;
        }        
};

/*Instrucciones------------------------------------------------------------------------------>*/
class Escribir: public Instruccion
{    
    public:
        Expresion *expr;
        Escribir(int l, int c, Expresion *e)
        {
            linea = l;
            columna = c;
            expr= e;
        }

        void ejecutar(Entorno *e)override
        {
            //cout<<<expr->getTipo(e).getNombre()<<"\t";
            Type type = expr->getTipo(e);
            switch (type.tipo)
            {
                case TIPOLOGICO:
                    //cout<<< expr->getValor(e).getCadenaLogico()<<endl;
                    mostrar_en_pantalla(expr->getValor(e).getCadenaLogico(),e);
                    break;                
                case TIPOENTERO:
                    //cout<<< expr->getValor(e).valor_entero<<endl;
                    mostrar_en_pantalla(to_string(expr->getValor(e).valor_entero),e);
                    break;        
                case TIPOREAL:
                    //cout<<< expr->getValor(e).valor_real<<endl;
                    mostrar_en_pantalla(to_string(expr->getValor(e).valor_real),e);
                    break;        
                case TIPOCADENA:
                    //cout<<< expr->getValor(e).valor_cadena<<endl;
                    mostrar_en_pantalla(expr->getValor(e).valor_cadena,e);
                    break;        
                case TIPOPOSICION:
                    //cout<<< "y:" <<expr->getValor(e).valor_posicion[0] <<", x:"<< expr->getValor(e).valor_posicion[1]<<endl;
                    mostrar_en_pantalla("y:"+to_string(expr->getValor(e).valor_posicion[0])+", x:" + to_string(expr->getValor(e).valor_posicion[1]),e);
                    break;    
                case TIPOERROR:
                    //cout <<"<---------------->";
                    break;                                                                      
            }            
        }
        void mostrar_en_pantalla(string mens, Entorno *e)
        {
        	mens = "\tentornoMostrarMensaje(\""+mens+"\");\n";
        	e->escribir_fichero(mens);
        }
};


class Declaracion: public Instruccion
{
    public:
        listaId *lista;
        Type tipo;        
        Declaracion(int l, int c, Type t, listaId *ids)
        {
            linea = l;
            columna = c;
            tipo = t;
            lista = ids;
        }

        void ejecutar(Entorno *e)override
        {     
            for (auto id : lista->data)
            {                
            	/*Aquí recorremos el vector que es la lista de identificadores y
            	 * creamos un número símbolo para cada uno de ellos.*/
                simbolo *tmp = new simbolo();
                tmp->linea = linea;
                tmp->constante = false;
                tmp->columna = columna;
                tmp->tipo = tipo;
                tmp->id = id;
                e->tabla.insertarSimbolo(tmp);
            }            
        }       
};

class Constante: public Instruccion
{
    public:
        string id; 
        Expresion *expr;      
        Constante(int l, int c, string i, Expresion *e)
        {   
            id = i;
            linea = l;
            columna = c;
            expr = e;            
        }

        void ejecutar(Entorno *e)override
        {                 
            simbolo tmp = expr->getValor(e);                                  
            if(!tmp.tipo.esError())
            {                
                tmp.constante = true;
                tmp.id = id;
                if(e->tabla.insertarSimbolo(&tmp))
                {
                	/*Mensaje de error (Innecesario), ya ha sido implementado en la clase tabla.*/
                }
            }
        }       
};

class Asignacion: public Instruccion
{
    public:
        string id; 
        Expresion *expr;      
        Asignacion(int l, int c, string i, Expresion *e)
        {   
            id = i;
            linea = l;
            columna = c;
            expr = e;            
        }

        void ejecutar(Entorno *e)override
        {                 
            simbolo tmp = expr->getValor(e);                                  
            if(!tmp.tipo.esError())
            {                
                tmp.id = id;
                e->tabla.actualizarSimbolo(&tmp);
            }
        }       
};

class Bloque: public Instruccion
{
    public:
        vector<Instruccion*> instrucciones;
        Bloque(int l, int c)
        {
            linea = l;
            columna = c;
        }

        void addInstruccion(Instruccion *inst)
        {
        	if(inst!=NULL)
        	{
        		instrucciones.push_back(inst);
        	}
        }

        void ejecutar(Entorno *e)override
        {

            //cout<<<"Hay "<< instrucciones.size() <<" instrucciones"<<endl;
            for (unsigned i=0; i<instrucciones.size(); i++)
            {
                instrucciones.at(i)->ejecutar(e);
            }                
        } 
};


class Si: public Instruccion
{
    public:
        Bloque* instrucciones;
        Expresion* condicion;
        Si* SinoSi;
        Si(int l, int c, Expresion *e, Bloque *ins, Si *elseNodo)
        {
            linea = l;
            columna = c;
            instrucciones = ins;
            condicion = e;
            SinoSi = elseNodo;
        }
        
        void ejecutar(Entorno *e)override
        {                 
            simbolo valor = condicion->getValor(e);
            if(valor.tipo.esLogico())
            {                
                if(valor.valor_booleano)
                {
                    instrucciones->ejecutar(e);
                }                
                else
                {
                    if(SinoSi!=NULL)
                    {
                        SinoSi->ejecutar(e);
                    }                    
                }
            }
            else
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en sentencia IF, la condición debe devolver un valor lógico.");                 
            }
        } 
};


class Mientras: public Instruccion
{
    public:
        Bloque* instrucciones;
        Expresion* condicion;        
        Mientras(int l, int c, Expresion *e, Bloque *ins)
        {
            linea = l;
            columna = c;
            instrucciones = ins;
            condicion = e;            
        }
        
        void ejecutar(Entorno *e)override
        {                 
            simbolo valor = condicion->getValor(e);
            if(valor.tipo.esLogico())
            {         
                bool condicionflag = valor.valor_booleano;       
                while(condicion)
                {
                    instrucciones->ejecutar(e);                    
                    condicionflag = condicion->getValor(e).valor_booleano;
                }            
            }
            else
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en sentencia Mientras, la condición debe devolver un valor lógico.");                 
            }
        } 
};


class Repetir: public Instruccion
{
    public:
        Bloque* instrucciones;
        Expresion* condicion;        
        Repetir(int l, int c, Expresion *e, Bloque *ins)
        {
            linea = l;
            columna = c;
            instrucciones = ins;
            condicion = e;            
        }
        
        void ejecutar(Entorno *e)override
        {                 
            simbolo valor = condicion->getValor(e);
            if(valor.tipo.esNumerico())
            {         
                int inicio = 0;
                int limite = 0;
                if(valor.tipo.esEntero()){limite = valor.valor_entero;}
                if(valor.tipo.esReal()){limite = valor.valor_real;}
                for(inicio = 0; inicio < limite; inicio++ )
                {
                    instrucciones->ejecutar(e);                     
                }            
            }
            else
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en sentencia Repetir, la condición debe devolver un valor númerico entero.");                 
            }
        } 
};


class Obstaculo_movimiento: public Instruccion
{
    public:
        string movimiento;
        Expresion *expr;
        bool esEjemplo = false;
        Obstaculo_movimiento(int l, int c, string mov, Expresion *e)
        {
            linea = l;
            columna = c;
            movimiento = mov;
            expr = e;
            esEjemplo = false;
        }
        Obstaculo_movimiento(int l, int c, string mov, Expresion *e, bool exm)
        {
            linea = l;
            columna = c;
            movimiento = mov;
            expr = e;
            esEjemplo = true;
        }
        void ejecutar(Entorno *e)override
        {
            simbolo valor = expr->getValor(e);
            if(valor.tipo.esNumerico())
            {
                int valorMovimiento = 0;
                if(valor.tipo.esEntero()){ valorMovimiento = valor.valor_entero;}
                else{ valorMovimiento = valor.valor_real;}

                simbolo *posicion_obstaculo = e->tabla.obtenerSimboloLocal("POSICION_RELATIVA_OBSTACULO", linea);
                int y = posicion_obstaculo->valor_posicion[0];
                int x = posicion_obstaculo->valor_posicion[1];
                int maximo = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", linea)->valor_entero;
                //cout<<< "Aplicando movimiento "<< movimiento <<endl;
                if(movimiento=="este")
                {
                    if(( x + valorMovimiento) < maximo)
                    {
                        posicion_obstaculo->valor_posicion[1] = x + valorMovimiento;
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion "+movimiento + ": El valor debe ser menor al tamaño máximo del tablero." + to_string(x+valorMovimiento) +" > " + to_string(maximo-1));
                    }
                }
                else
                if(movimiento=="oeste")
                {
                    if(( x + valorMovimiento) >= 0)
                    {
                        posicion_obstaculo->valor_posicion[1] = x - valorMovimiento;
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion "+movimiento + ": El valor debe ser mayor a 0 (< "+ to_string(y-valorMovimiento)+")");
                    }
                }
                else
                if(movimiento=="norte")
                {
                    if(( y - valorMovimiento) >= 0)
                    {
                        posicion_obstaculo->valor_posicion[0] = y - valorMovimiento;
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion "+movimiento + ": El valor debe ser mayor a 0 (< "+ to_string(y-valorMovimiento)+")");
                    }
                }
                else
                if(movimiento=="sur")
                {
                    if(( y + valorMovimiento) < maximo)
                    {
                        posicion_obstaculo->valor_posicion[0] = y + valorMovimiento;
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion "+movimiento + ": El valor debe ser menor al tamaño máximo del tablero. "  + to_string(y+valorMovimiento) +" > " + to_string(maximo-1));
                    }
                }				
            }
            else
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "El valor asociado a la instruccion '"+movimiento+"' debe ser númerico. Tipo enviado: " +valor.tipo.getNombre());
            }            
        }
};

class PonerObstaculo: public Instruccion
{
    public:
        Expresion *expr;
        PonerObstaculo(int l, int c, Expresion *e)
        {
            linea = l;
            columna = c;
            expr  = e;
        }

        void ejecutar(Entorno *e)override
        {
            //printf("Poniendo objeto. \n");
            if(expr==NULL)
            /*En este caso significa que hay que dibujar en la posición relativa.*/
            {
                simbolo *posicion_obstaculo = e->tabla.obtenerSimboloLocal("POSICION_RELATIVA_OBSTACULO", linea);
                if(posicion_obstaculo!=NULL)
                {
                    int y_posicion = posicion_obstaculo->valor_posicion[0];
                    int x_posicion = posicion_obstaculo->valor_posicion[1];
                    int limite = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", 0)->valor_entero;
                    // Comprobamos que los limites se cumplan
                    if(y_posicion >= 0 && y_posicion < limite)
                    {
                        if(x_posicion >= 0 && x_posicion < limite)
                        {
                        	//int estado_casilla = e->tablero[y_posicion * limite + x_posicion];
                        	int *ptr = e->tablero;
                        	int estado_casilla = *&ptr[y_posicion * limite + x_posicion];
                        	//cout<<<"Tipo casilla "<<estado_casilla<<endl;
                        	if(estado_casilla==CASILLA_VACIA)
                        	{
                                string mensaje = "\tentornoPonerObstaculo(" + to_string(y_posicion) + ", " +to_string(x_posicion) + ");\n";
                                e->escribir_fichero(mensaje);
                                e->tablero[y_posicion * limite + x_posicion] = CASILLA_OBSTACULO;
                        	}
                        	else
                        	{
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion Obstaculo: La posición ("+ to_string(y_posicion)+" , "+to_string(x_posicion ) +") se encuentra ocupada.");
                        	}
                        }
                        else
                        {
                            Error::registrarErrorSemantico(linea, columna," " ,
                            "Error: Instruccion Obstaculo: El valor de columna es incorrecto. Coordena ("+ to_string(y_posicion)+", "+to_string(x_posicion) +") está fuera del tablero" );
                        }
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion Obstaculo: El valor de fila es incorrecto. Coordena ("+ to_string(y_posicion)+", "+to_string(x_posicion) +") está fuera del tablero" );
                    }
                }
            }
            else
            {
                simbolo valor = expr->getValor(e);
                if(valor.tipo.esPosicion())
                {
                    /*Aquí escribimos el código para posicionar un obstaculo en esta posicion*/                	
                    int y_posicion = valor.valor_posicion[0];
                    int x_posicion = valor.valor_posicion[1];
                    int limite = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", 0)->valor_entero;
                    // Comprobamos que los limites se cumplan
                    if(y_posicion >= 0 && y_posicion < limite)
                    {
                        if(x_posicion >= 0 && x_posicion < limite)
                        {
                        	int estado_casilla = e->tablero[y_posicion * limite + x_posicion];
                        	//cout<<<"Tipo casilla "<<estado_casilla<<endl;
                        	if(estado_casilla==CASILLA_VACIA)
                        	{
                                simbolo *posicion_obstaculo = e->tabla.obtenerSimboloLocal("POSICION_RELATIVA_OBSTACULO", linea);
								posicion_obstaculo->valor_posicion[0]=y_posicion;
								posicion_obstaculo->valor_posicion[1]=x_posicion;
								string mensaje = "\tentornoPonerObstaculo(" + to_string(y_posicion) + ", " +to_string(x_posicion) + ");\n";
								e->escribir_fichero(mensaje);
								e->tablero[y_posicion * limite + x_posicion] = CASILLA_OBSTACULO;
                        	}
                        	else
                        	{
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion Obstaculo: La posición ("+ to_string(y_posicion)+" , "+to_string(x_posicion ) +") se encuentra ocupada.");
                        	}
                        }
                        else
                        {
                            Error::registrarErrorSemantico(linea, columna," " ,
                            "Error: Instruccion Obstaculo: El valor de columna es incorrecto. Coordena ("+ to_string(y_posicion)+", "+to_string(x_posicion) +") está fuera del tablero" );
                        }
                    }
                    else
                    {
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error: Instruccion Obstaculo: El valor de fila es incorrecto. Coordena ("+ to_string(y_posicion)+", "+to_string(x_posicion) +") está fuera del tablero"  );
                    }

                }
                else                
                {
                    /* Reportar error semantico*/
                    Error::registrarErrorSemantico(linea, columna," " ,
                    "Error: Instruccion Obstaculo: Valor esperado es un valor de tipo posición y se ha intentado establecer un valor de tipo "+ valor.tipo.getNombre());
                }
            }
        }

};

class Establecer_dimension: public Instruccion
{
    public:
        Expresion *expr;
        Establecer_dimension(int l, int c, Expresion *e)
        {
            linea = l; columna = c; 
            expr = e;
        }

        void ejecutar(Entorno *e) override
        {
            simbolo valor = expr->getValor(e);
            if(!valor.tipo.esNumerico())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "El valor asociado a la instruccion Dimensión debe ser númerico.");
                return;                
            }
            else
            {
            	int valorTmp = 0;
            	if(valor.tipo.esEntero())
            	{
            		valorTmp =valor.valor_entero;
            	}
            	else
            	{
            		valorTmp = valor.valor_real;
            	}
                if(valorTmp>=4 && valorTmp<=10)
                {
                	simbolo *simbolo_dimension = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", linea);
                	simbolo_dimension->valor_entero = valorTmp;

                    e->tablero = new int[valorTmp*valorTmp];
                    for(int i =0; i < valorTmp*valorTmp; i++)
                    {
                    	e->tablero[i] = CASILLA_VACIA;
                    }
                }
                else
                {
                    Error::registrarErrorSemantico(linea, columna," " ,
                    "Error al tratar de establecer el valor" + to_string(valorTmp) + " al tamaño del tablero. Tiene que ser un valor  4<=valor<=10");
                    return;
                }
            }
        }
};

class Establecer_entrada: public Instruccion
{
    public:
        Expresion *expr;
        Establecer_entrada(int l, int c, Expresion *e)
        {
            linea = l; columna = c; 
            expr = e;
        }

        void ejecutar(Entorno *e) override
        {
        	simbolo coordena = expr->getValor(e);
        	if(!coordena.tipo.esPosicion())
        	{
                Error::registrarErrorSemantico(linea, columna," " ,
                "El valor asociado a la instruccion Entrada debe ser posición.");
                return;
        	}
        	else
        	{
        		simbolo *simbolo_dimension = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", linea);
        		/*Verificamos que esté en el borde del tablero*/
        		int tamano_tablero = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", 0)->valor_entero;                
        		int y_coordenada = coordena.valor_posicion[0];
        		int x_coordenada = coordena.valor_posicion[1];

        		if( y_coordenada == 0 || y_coordenada == (tamano_tablero -1) )
        		{
                    /*Que esté dentro del tablero*/
        			if( x_coordenada >= 0 && x_coordenada <= (tamano_tablero-1) )
        			{
                		simbolo_dimension->valor_posicion[0] =coordena.valor_posicion[0];
                		simbolo_dimension->valor_posicion[1] =coordena.valor_posicion[1];
                		e->tablero[y_coordenada * tamano_tablero + x_coordenada] = CASILLA_ENTRADA;
        			}
        			else
        			{
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Entrada. Debe estar al borde del tablero.");
        			}
        		}
        		else
        		{
                    /*Comprobamos que y esté dentro del tablero*/
        			if( y_coordenada >= 0 && y_coordenada <= (tamano_tablero-1))
        			{
                        /*Corrobamos que la entrada esté dentro del tablero*/
            			if( x_coordenada == 0 || x_coordenada == (tamano_tablero -1) )
            			{
                    		simbolo_dimension->valor_posicion[0] =coordena.valor_posicion[0];
                    		simbolo_dimension->valor_posicion[1] =coordena.valor_posicion[1];
                    		e->tablero[y_coordenada * tamano_tablero + x_coordenada] = CASILLA_ENTRADA;
            			}
            			else
            			{
                            Error::registrarErrorSemantico(linea, columna," " ,
                            "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Entrada. Debe estar al borde del tablero.");
            			}
        			}
        			else
        			{
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Entrada. Debe estar al borde del tablero.");
        			}
        		}
        	}
        }
};


class Establecer_salida: public Instruccion
{
    public:
        Expresion *expr;
        Establecer_salida(int l, int c, Expresion *e)
        {
            linea = l; columna = c; 
            expr = e;
        }

        void ejecutar(Entorno *e) override
        {
        	simbolo coordena = expr->getValor(e);
        	if(!coordena.tipo.esPosicion())
        	{
                Error::registrarErrorSemantico(linea, columna," " ,
                "El valor asociado a la instruccion Salida debe ser posición.");
                return;
        	}
        	else
        	{
        		simbolo *simbolo_dimension = e->tabla.obtenerSimboloLocal("SALIDA_TABLERO_INIT", linea);
        		/*Verificamos que esté en el borde del tablero*/
        		int tamano_tablero = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", 0)->valor_entero;
        		int y_coordenada = coordena.valor_posicion[0];
        		int x_coordenada = coordena.valor_posicion[1];

        		if( y_coordenada == 0 || y_coordenada == (tamano_tablero -1) )
        		{
                    /*Verificamos que esté dentro del tablero*/
        			if( x_coordenada >= 0 && x_coordenada <= (tamano_tablero-1) )
        			{
                		simbolo_dimension->valor_posicion[0] =coordena.valor_posicion[0];
                		simbolo_dimension->valor_posicion[1] =coordena.valor_posicion[1];
                		e->tablero[y_coordenada * tamano_tablero + x_coordenada] = CASILLA_SALIDA;

        			}
        			else
        			{
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Salida. Debe estar al borde del tablero.");
        			}
        		}
        		else
        		{
                    /*Corroboramos que esté dentro del tablero*/
        			if( y_coordenada >= 0 && y_coordenada <= (tamano_tablero -1) )
        			{
            			if( x_coordenada == 0 || x_coordenada == (tamano_tablero -1) )
            			{
                    		simbolo_dimension->valor_posicion[0] =coordena.valor_posicion[0];
                    		simbolo_dimension->valor_posicion[1] =coordena.valor_posicion[1];
                    		e->tablero[y_coordenada * tamano_tablero + x_coordenada] = CASILLA_SALIDA;
            			}
            			else
            			{
                            Error::registrarErrorSemantico(linea, columna," " ,
                            "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Salida. Debe estar al borde del tablero.");
            			}
        			}
        			else
        			{
                        Error::registrarErrorSemantico(linea, columna," " ,
                        "Error con coordenada ("+ to_string(y_coordenada)+ ","+ to_string(x_coordenada)+") para establecer Salida. Debe estar al borde del tablero.");
        			}
        		}

        	}
        }
};

class Establecer_pausa: public Instruccion
{
    public:
        Expresion *expr;
        Establecer_pausa(int l, int c, Expresion *e)
        {
            linea = l; columna = c; 
            expr = e;
        }

        void ejecutar(Entorno *e) override
        {
        	simbolo valor = expr->getValor(e);
        	if(!valor.tipo.esNumerico())
        	{
                Error::registrarErrorSemantico(linea, columna," " ,
                "El valor asociado a la instruccion Salida debe ser posición.");
                return;
        	}
        	else
        	{
            	double valorTmp = 0;
            	if(valor.tipo.esEntero())
            	{
            		valorTmp =valor.valor_entero;
            	}
            	else
            	{
            		valorTmp = valor.valor_real;
            	}

        		simbolo *simbolo_pausa = e->tabla.obtenerSimboloLocal("PAUSA_TABLERO", linea);
        		simbolo_pausa->valor_real =valorTmp;

        	}
        }
};

class Ejemplo:public Instruccion
{
    public:
        string id;
        Bloque *instrucciones;
        Ejemplo(int l, int c, string i, Bloque * inst)
        {
            linea = l;
            columna = c;
            id = i;
            instrucciones = inst;
        }

        void ejecutar(Entorno *e) override
        {
            /*Aquí escribimos el código para generar los movimientos*/
        	/*Primero reestablecemos el tamaño, la pausa la entrada y la salida*/
        	e->escribir_fichero("\n\t/*Comenzando ejemplo*/\n");

            //entornoPonerEntrada(5,6,0.400);
            int y_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[0];
            int x_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[1];
            float pausa = e->tabla.obtenerSimboloLocal("PAUSA_TABLERO", 0)->valor_real;
            string cadena = "\tentornoPonerEntrada("+to_string(y_entrada)+","+to_string(x_entrada)+","+to_string(pausa)+");\n";
            e->escribir_fichero(cadena);

            //entornoPonerSalida(0,2);
            int y_salida = e->tabla.obtenerSimboloLocal("SALIDA_TABLERO_INIT", 0)->valor_posicion[0];
            int x_salida = e->tabla.obtenerSimboloLocal("SALIDA_TABLERO_INIT", 0)->valor_posicion[1];
            cadena = "\tentornoPonerSalida("+to_string(y_salida)+","+to_string(x_salida)+");\n";

            e->escribir_fichero(cadena);

            /*Ponemos al jugador en la entrada*/
            e->tabla.obtenerSimboloLocal("JUGADOR_POSICION_ACTUAL",0)->valor_posicion[0] = y_entrada;
            e->tabla.obtenerSimboloLocal("JUGADOR_POSICION_ACTUAL",0)->valor_posicion[1] = x_entrada;
            e->estado_juego = NO_GANADO;
            e->puede_continuar = true;
        	instrucciones->ejecutar(e);
        }

};

class Movimiento_jugador: public Instruccion
{
    public:
        string movimiento;
        Expresion *expr;
        Movimiento_jugador(int l, int c, string mov, Expresion *e)
        {
            linea = l;
            columna = c;
            movimiento = mov;
            expr = e;
        }
        Movimiento_jugador(int l, int c, string mov, Expresion *e, bool exm)
        {
            linea = l;
            columna = c;
            movimiento = mov;
            expr = e;
        }
        void ejecutar(Entorno *e)override
        {
        	//cout<<<"Moviendo al jugador"<<endl;
            simbolo valor = expr->getValor(e);
            if(valor.tipo.esNumerico())
            {
            	/*Continuamos solo si el jugador no ha perdido o no ha llegado a la meta.*/
            	//if(e->estado_juego == NO_GANADO)
                if (e->puede_continuar)
            	{
					int valorMovimiento = 0;
					if(valor.tipo.esEntero()){ valorMovimiento = valor.valor_entero;}
					else{ valorMovimiento = valor.valor_real;}

					simbolo *posicion_obstaculo = e->tabla.obtenerSimboloLocal("JUGADOR_POSICION_ACTUAL", linea);
					int y = posicion_obstaculo->valor_posicion[0];
					int x = posicion_obstaculo->valor_posicion[1];
					int maximo = e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT", linea)->valor_entero;
					float pausa = e->tabla.obtenerSimboloLocal("PAUSA_TABLERO", linea)->valor_real;
					//cout<<< "Aplicando movimiento "<< movimiento <<endl;
					if(movimiento=="este")
					{
                        for(int i = 1; i <= valorMovimiento ; i++)
                        {
                            if(( x + valorMovimiento) < maximo)
                            {
                                if(e->tablero[y*maximo + (x+i)] == CASILLA_VACIA || e->tablero[y*maximo + (x+i)] == CASILLA_ENTRADA)
                                {                                    
                                        posicion_obstaculo->valor_posicion[1] = x + i;
                                        /*Ahora escribimos la cadena*/
                                        string cadena = "\tentornoPonerFigura(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])
                                                +","+to_string(pausa)+");\n";
                                        e->escribir_fichero(cadena);                                    
                                }
                                else
                                if(e->tablero[y*maximo + (x+i)] == CASILLA_SALIDA)
                                {                                     
                                        posicion_obstaculo->valor_posicion[1] = x + i;
                                        /*Ahora escribimos la cadena*/
                                        string cadena = "\tentornoPonerFiguraSalida(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                        cadena = cadena + "\tentornoMostrarMensaje (\"  ¡ Lo conseguiste !    \");\n";
                                        e->escribir_fichero(cadena);
                                        e->estado_juego= GANADO;     
                                        e->puede_continuar= false; 
                                        break;                              
                                }
                                else
                                {
                                    posicion_obstaculo->valor_posicion[1] = x + i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerChoque(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena  = cadena +"\tentornoMostrarMensaje (\"  ¡ Mala suerte !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= PERDIDO;
                                    e->puede_continuar= false;
                                    break;
                                }
                            }
                            else
                            {
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion "+movimiento + " " + to_string(valorMovimiento)+  ": El valor queda fuera del trablero. ");
                                break;
                            }
                        }
					}
					else
					if(movimiento=="oeste")
					{
                        for(int i = 1; i <= valorMovimiento; i++)
                        {                        
                            if(( x - valorMovimiento) >= 0)
                            {
                                if(e->tablero[y*maximo + (x-i)] == CASILLA_VACIA || e->tablero[y*maximo + (x-i)] == CASILLA_ENTRADA)
                                {
                                    posicion_obstaculo->valor_posicion[1] = x - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFigura(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])
                                            +","+to_string(pausa)+");\n";
                                    e->escribir_fichero(cadena);
                                }
                                else
                                if(e->tablero[y*maximo + (x-i)] == CASILLA_SALIDA)
                                {
                                    posicion_obstaculo->valor_posicion[1] = x - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFiguraSalida(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena = cadena + "\tentornoMostrarMensaje (\"  ¡ Lo conseguiste !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= GANADO;
                                    e->puede_continuar= false;
                                    break;
                                }
                                else
                                {
                                    posicion_obstaculo->valor_posicion[1] = x - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerChoque(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena  = cadena +"\tentornoMostrarMensaje (\"  ¡ Mala suerte !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= PERDIDO;
                                    e->puede_continuar= false;
                                    break;
                                }
                            }
                            else
                            {
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion "+movimiento + " " + to_string(valorMovimiento)+  ": El valor queda fuera del trablero. ");
                                break;
                            }
                        }
					}
					else
					if(movimiento=="norte")
					{
                        for(int i = 1; i <= valorMovimiento; i++)
                        {
                            if(( y - valorMovimiento) >= 0)
                            {
                                if(e->tablero[(y - i)*maximo + x] == CASILLA_VACIA || e->tablero[(y - i)*maximo + x] == CASILLA_ENTRADA)
                                {
                                    posicion_obstaculo->valor_posicion[0] = y - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFigura(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])
                                            +","+to_string(pausa)+");\n";
                                    e->escribir_fichero(cadena);
                                }
                                else
                                if(e->tablero[(y - i)*maximo + x] ==  CASILLA_SALIDA)
                                {
                                    posicion_obstaculo->valor_posicion[0] = y - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFiguraSalida(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena = cadena + "\tentornoMostrarMensaje (\"  ¡ Lo conseguiste !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= GANADO;
                                    e->puede_continuar= false;
                                    break;
                                }
                                else
                                {

                                    posicion_obstaculo->valor_posicion[0] = y - i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerChoque(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena  = cadena +"\tentornoMostrarMensaje (\"  ¡ Mala suerte !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= PERDIDO;
                                    e->puede_continuar= false;
                                    break;
                                }


                            }
                            else
                            {
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion "+movimiento + " " + to_string(valorMovimiento)+  ": El valor queda fuera del trablero. ");
                                break;
                            }
                        }
					}
					else
					if(movimiento=="sur")
					{
                        for(int i = 1; i<= valorMovimiento ; i++)
                        {                        
                            if(( y + valorMovimiento) < maximo)
                            {

                                if(e->tablero[(y + i)*maximo + x] == CASILLA_VACIA || e->tablero[(y + i)*maximo + x] == CASILLA_ENTRADA)
                                {
                                    posicion_obstaculo->valor_posicion[0] = y + i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFigura(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])
                                            +","+to_string(pausa)+");\n";
                                    e->escribir_fichero(cadena);
                                }
                                else
                                if(e->tablero[(y + i)*maximo + x] ==  CASILLA_SALIDA)
                                {
                                    posicion_obstaculo->valor_posicion[0] = y + i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerFiguraSalida(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena = cadena + "\tentornoMostrarMensaje (\"  ¡ Lo conseguiste !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= GANADO;
                                    e->puede_continuar= false;
                                    break;
                                }
                                else
                                {

                                    posicion_obstaculo->valor_posicion[0] = y + i;
                                    /*Ahora escribimos la cadena*/
                                    string cadena = "\tentornoPonerChoque(" + to_string(posicion_obstaculo->valor_posicion[0]) + ", "+ to_string(posicion_obstaculo->valor_posicion[1])+");\n";
                                    cadena  = cadena +"\tentornoMostrarMensaje (\"  ¡ Mala suerte !    \");\n";
                                    e->escribir_fichero(cadena);
                                    e->estado_juego= PERDIDO;
                                    e->puede_continuar= false;
                                    break;
                                }
                            }
                            else
                            {
                                Error::registrarErrorSemantico(linea, columna," " ,
                                "Error: Instruccion "+movimiento + " " + to_string(valorMovimiento)+  ": El valor queda fuera del trablero. ");
                                break;
                            }
                        }
					}
				}
				else
				{
					/*Error::registrarErrorSemantico(linea, columna," " ,
					"El valor asociado a la instruccion '"+movimiento+"' debe ser númerico. Tipo enviado : " + valor.tipo.getNombre());*/
				}
            }
        }
};



class Programa:public Instruccion
{
    public:
        Bloque * definiciones = NULL;
        Bloque * configuraciones= NULL;
        Bloque * obstaculos= NULL;
        Bloque * ejemplos= NULL;
        Programa(int l, int c)
        {
            linea = l;
            columna = c;
        }

        void setDefiniciones(Bloque *def)
        {
            definiciones = def;
        }

        void setConfiguraciones(Bloque *conf)
        {
            configuraciones  = conf;
        }

        void setObstaculos(Bloque *obs)
        {
            obstaculos = obs;
        }

        void setEjemplos(Bloque *ejemp)
        {
            ejemplos = ejemp;
        }

        void ejecutar(Entorno *e) override
        {
        	e->abrirFichero();
        	string encabezado = "#include <iostream>\n#include <allegro5/allegro.h>\n#include <stdio.h>\n#include \"entorno.h\"\n\nusing namespace std;\n\nint main(int argc, char** argv)\n{\n";

            if(definiciones!=NULL)
            {
                definiciones->ejecutar(e);
            }
            if(configuraciones!=NULL)
            {
                configuraciones->ejecutar(e);
                //entornoPonerEntrada(5,6,0.400);
                int y_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[0];
                int x_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[1];
                float pausa = e->tabla.obtenerSimboloLocal("PAUSA_TABLERO", 0)->valor_real;
                string cadena = "\tentornoPonerEntrada("+to_string(y_entrada)+","+to_string(x_entrada)+","+to_string(pausa)+");\n";
                e->escribir_fichero(cadena);

                //entornoPonerSalida(0,2);
                int y_salida = e->tabla.obtenerSimboloLocal("SALIDA_TABLERO_INIT", 0)->valor_posicion[0];
                int x_salida = e->tabla.obtenerSimboloLocal("SALIDA_TABLERO_INIT", 0)->valor_posicion[1];
                cadena = "\tentornoPonerSalida("+to_string(y_salida)+","+to_string(x_salida)+");\n";

                e->escribir_fichero(cadena);
            }   
            if(obstaculos!=NULL)
            {
                obstaculos->ejecutar(e);
            }   
            if(ejemplos!=NULL)
            {
            	/*Creamos un nuevo símbolo que nos ayuda a saber la posición relativa del objeto*/
                int y_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[0];
                int x_entrada = e->tabla.obtenerSimboloLocal("ENTRADA_TABLERO_INIT", 0)->valor_posicion[1];
                simbolo *posicion_jugador = new simbolo("JUGADOR_POSICION_ACTUAL",y_entrada,x_entrada, 0,0);
                posicion_jugador->simbolo_juego = true;
                e->tabla.insertarSimbolo(posicion_jugador);

                /*Ahora ejecutamos los ejemplos*/
                ejemplos->ejecutar(e);
                e->escribir_fichero("\tentornoMostrarMensajeFin (\"  ¡ Fin !    \");");
            }
            /*De ultimo agregamos el tamaño del tablero*/
            
            string cadena_actual;
            string output;
            ifstream file;
            file.open(e->path_fichero);
            file >> cadena_actual;
            file.close();

            encabezado = encabezado+"/*----------------Estableciendo tamaño del tablero-----------*/\n\n\tentornoIniciar(" +to_string(e->tabla.obtenerSimboloLocal("TAMANO_TABLERO_INIT",0)->valor_entero)+");";
            encabezado = encabezado + "\n"+cadena_actual;

            ofstream fichero;
            fichero.open (e->path_fichero);
            fichero << encabezado;
            fichero.close();


            string cierre = "\n\n\tentornoTerminar();\n\tentornoPausa(3);\n\treturn 0;\n}";
            e->escribir_fichero(cierre);
            e->cerrarFichero();

        }

};






#endif
