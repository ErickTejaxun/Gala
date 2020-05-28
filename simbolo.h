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



using namespace std;
class Expresion;
class Entorno;
class Error;

extern vector<Error> listaErrores;


enum TYPES { TIPOENTERO, TIPOREAL, TIPOLOGICO, TIPOCADENA, TIPOPOSICION, TIPOERROR};
class Error
{
    public:
        int linea;
        int columna;
        string descripcion;
        string tipo;
        string id;
        Error(int l, int c, string t, string i, string desc):linea(l), columna(c), tipo(t), descripcion(desc), id(i){}        
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
        bool esNumerico(){ return esEntero()|| esReal();}                      
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
        bool constante = false;
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
        //string tipos[4] = {"TIPOENTERO","TIPOREAL","lógico","TIPOERROR"};    
        string getCadenaData()
        {
            string DATACADENA = "Nombre\t\tTipo\t\tValor";            
            map<string, simbolo>::iterator item;
            for(item = tabla.begin(); item != tabla.end(); item++)
            {
                switch (item->second.tipo.getTipo())
                {
                    case TIPOENTERO:
                        DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +to_string(item->second.valor_entero);
                        break;                
                    case TIPOREAL:
                        DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +to_string(item->second.valor_real);
                        break;                
                    case TIPOLOGICO:
                        DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +item->second.getCadenaLogico();
                        break;   
                    case TIPOCADENA:
                        DATACADENA = DATACADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +item->second.valor_cadena;
                        break;                       
                    default:
                        break;
                }                
            }
            return DATACADENA;
        }

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

        int insertarSimbolo(simbolo *s)
        {                           
            simbolo simb = simbolo();
            simb.id = s->id; 
            simb.linea = s->linea; 
            simb.columna = s->columna; 
            simb.tipo= Type(s->tipo.tipo);
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
                    "La "+s->rol() +" "+s->id+" ya ha sido declarada");
                return 0;
            }                        
            tabla.insert( pair<string,simbolo>(s->id,simb));
            return 1;
        }


        int actualizarSimbolo(simbolo *s)
        {
            simbolo *tmp = obtenerSimbolo(s->id, s->linea);
            if(tmp!=NULL)
            {
                if(!tmp->constante)
                {
                    Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                        "El valor del símbolo " +s->id+ " es constante ");
                }
                if(s->tipo.tipo == tmp->tipo.tipo)
                {
                    s->linea = tmp->linea;
                    tmp = s;
                }           
                else
                {
                    Error::registrarErrorSemantico(s->linea, s->columna, s->id,                         
                        "La variable " +s->id+ " es de tipo " +tmp->tipo.getNombre()+", Error tratando de asignar un valor de tipo " +s->tipo.getNombre());                    
                } 
            }
        }
};

/*
clase para manejar los entornos*/
class Entorno
{
    public:
        TablaSimbolos tabla;
        Entorno *padre; /*Puntero hacia el entorno padre.*/
        Entorno(Entorno * p)
        {
            padre = p;
            tabla = TablaSimbolos();
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
            if(!izquierdo->getTipo(e).esError() && !derecho->getTipo(e).esError())         
            {
                if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
                {
                    return Type(TIPOCADENA);
                }
                if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
                {              
                    Error::registrarErrorSemantico(linea, columna," " ,
                    "Error en operación (+): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " + " + derecho->getTipo(e).getNombre());                             
                    return Type(TIPOERROR); // TIPOERROR;
                }
                if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
                {
                    return Type(TIPOREAL);
                }
                if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
                {
                    return Type(TIPOENTERO);
                }                
            }
            
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (+): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " + " + derecho->getTipo(e).getNombre());                                        
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
            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
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

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (-): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " - " + derecho->getTipo(e).getNombre());                
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {   
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (-): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " - " + derecho->getTipo(e).getNombre());                                        
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
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

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (*): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " * " + derecho->getTipo(e).getNombre());                 
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                   
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (*): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " * " + derecho->getTipo(e).getNombre());                         
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
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

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (/): No es permitida la operación " +  izquierdo->getTipo(e).getNombre() + " / " + derecho->getTipo(e).getNombre());                
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {            
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (/): No es permitida la operación " +  izquierdo->getTipo(e).getNombre() + " / " +  derecho->getTipo(e).getNombre());                               
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
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

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (^): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " ^ " + derecho->getTipo(e).getNombre());                
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {       
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (^): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " ^ " + derecho->getTipo(e).getNombre());                                    
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
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
            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esEntero()) // TIPOREAL
            {
                
                resultado.valor_real = valor_izquierdo.valor_entero % valor_derecho.valor_entero; 
            }   
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (%): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " % " + derecho->getTipo(e).getNombre());                
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                     
                Error::registrarErrorSemantico(linea, columna," " ,
                "Error en operación (%): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " % " + derecho->getTipo(e).getNombre());                      
                return Type(TIPOERROR); // TIPOERROR;
            }
            if(izquierdo->getTipo(e).esReal() || derecho->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            if(izquierdo->getTipo(e).esEntero() ||derecho->getTipo(e).esEntero())
            {
                return Type(TIPOREAL);
            }

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
            simbolo valor = expr->getValor(e);            
            

            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            resultado.valor_entero = valor.valor_entero * -1;
            resultado.valor_real = valor.valor_real * -1;                                 
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            if(expr->getTipo(e).esReal())
            {
                return Type(TIPOREAL);
            }
            else
            if(expr->getTipo(e).esEntero())
            {
                return Type(TIPOENTERO);
            }    
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (-): No es permitida la operación  - " + expr->getTipo(e).getNombre()); 
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
            if( izquierdo->getTipo(e).esNumerico() && derecho->getTipo(e).esNumerico())
            {
                return Type(TIPOLOGICO);
            }
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación ("+operacion+"): No es permitida la operación " + izquierdo->getTipo(e).getNombre() + " "+operacion+" " + derecho->getTipo(e).getNombre());            
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
            simbolo valor = expr->getValor(e);
            simbolo resultado = simbolo(); 
            resultado.linea = linea; 
            resultado.columna = columna; 
            resultado.tipo = getTipo(e);
            resultado.valor_booleano =  ! valor.valor_booleano;
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            if(expr->getTipo(e).esLogico())
            {
                return Type(TIPOLOGICO);
            }   
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación (!): No es permitida la operación  ! " + expr->getTipo(e).getNombre()); 
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
            if(operacion == "||")
            {
                resultado.valor_booleano =  izquierda->getValor(e).valor_booleano || derecha->getValor(e).valor_booleano ;
            }            
            if(operacion == "&&")
            {
                resultado.valor_booleano =  izquierda->getValor(e).valor_booleano && derecha->getValor(e).valor_booleano ;
            }                        
            return resultado;
        }

        Type getTipo(Entorno *e) override
        {
            if(izquierda->getTipo(e).esLogico()  && derecha->getTipo(e).esLogico())
            {
                return Type(TIPOLOGICO);
            }   
            Error::registrarErrorSemantico(linea, columna," " ,
            "Error en operación ("+operacion+"): No es permitida la operación  "+ izquierda->getTipo(e).getNombre() + " " + operacion + " "+ derecha->getTipo(e).getNombre()); 
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
            //cout<<expr->getTipo(e).getNombre()<<"\t";
            Type type = expr->getTipo(e);
            switch (type.tipo)
            {
                case TIPOLOGICO:
                    cout<< expr->getValor(e).getCadenaLogico()<<endl;
                    break;                
                case TIPOENTERO:
                    cout<< expr->getValor(e).valor_entero<<endl;
                    break;        
                case TIPOREAL:
                    cout<< expr->getValor(e).valor_real<<endl;    
                    break;        
                case TIPOCADENA:
                    cout<< expr->getValor(e).valor_cadena<<endl;        
                    break;        
                case TIPOPOSICION:
                    cout<< "y:" <<expr->getValor(e).valor_posicion[0] <<", x:"<< expr->getValor(e).valor_posicion[1]<<endl;
                    break;    
                case TIPOERROR:
                    //cout <<"<---------------->";
                    break;                                                                      
            }            
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
                simbolo *tmp = new simbolo();
                tmp->linea = linea;
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
                    if(tmp.tipo.esCadena())
                    {
                        cout<<tmp.valor_cadena<<endl;
                    }
                }
            }
        }       
};






#endif