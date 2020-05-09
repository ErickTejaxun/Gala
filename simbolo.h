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

using namespace std;
class Expresion;
class Entorno;

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
};

class Type
{        
    public:
        int tipo;
        Type(int t)
        {
            switch (t)
            {
                case 0:
                    tipo = TIPOENTERO;
                    break;
                case 1:
                    tipo = TIPOREAL;
                    break;
                case 2:
                    tipo = TIPOLOGICO;
                    break;
                case 3:
                    tipo = TIPOCADENA;
                    break;
                case 4:
                    tipo = TIPOPOSICION;
                    break;        
                default:
                    tipo =TIPOERROR;
                    break;
            }            
        }  
        Type(){tipo=0;}
        int getTipo(){return tipo;}
        void setTipo(int t){tipo = t;}

        bool esEntero(){ return tipo == TIPOENTERO;};        
        bool esReal(){ return tipo ==TIPOREAL;};        
        bool esLogico(){ return tipo == TIPOLOGICO;};        
        bool esCadena(){ return tipo ==TIPOCADENA;}        
        bool esPosicion(){return tipo==TIPOPOSICION;}                           
        string getNombre()
        {
            if(esEntero()){return "TIPOENTERO";}
            if(esReal()){return "TIPOREAL";}
            if(esLogico()){return "TIPOLOGICO";}
            if(esCadena()){return "TIPOCADENA";}
            if(esPosicion()){return "TIPOPOSICION";}
        }


};


class simbolo 
{
    public:       
        Type tipo = Type(0);
        int valor_entero=0;
        float valor_real=0;   
        bool valor_booleano=0;
        Expresion *valor_posicion[2];
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
        simbolo(string i, Expresion *y, Expresion *x, int l, int c): id(i), linea(l), columna(c){ tipo =Type(TIPOPOSICION); valor_posicion[0] = y; valor_posicion[1] = x;}
        string getCadenaLogico(){return valor_booleano? "verdadero": "falso";}   
        Type getTipo(Entorno *e){ return tipo;}
};

class TablaSimbolos
{
    public:
        map<string, simbolo> tabla;  
        //string tipos[4] = {"TIPOENTERO","TIPOREAL","lógico","TIPOERROR"};    
        string getCadenaData()
        {
            string TIPOCADENA = "Nombre\t\tTipo\t\tValor";            
            map<string, simbolo>::iterator item;
            for(item = tabla.begin(); item != tabla.end(); item++)
            {
                switch (item->second.tipo.getTipo())
                {
                    case 0:
                        TIPOCADENA = TIPOCADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +to_string(item->second.valor_entero);
                        break;                
                    case 1:
                        TIPOCADENA = TIPOCADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +to_string(item->second.valor_real);
                        break;                
                    case 2:
                        TIPOCADENA = TIPOCADENA + "\n"+item->second.id +"\t\t\t"+item->second.tipo.getNombre()+"\t\t\t"
                        +item->second.getCadenaLogico();
                        break;                                                        
                    default:
                        break;
                }                
            }
            return TIPOCADENA;
        }

        simbolo * obtenerSimbolo(string id, int linea)
        {                        
            auto it = tabla.find(id);            
            if (it != tabla.end())
            {
                simbolo *tmp = & it->second;     
                return tmp;            
            }
            else
            {
                simbolo *tmp = new simbolo();
                tmp->tipo.setTipo(TIPOERROR);
                cout<<"TIPOERROR semántico en línea "<<linea <<": Símbolo "<< id<<" no encontrado "<<endl;
                return tmp;
            }                
        }        

        int insertarSimbolo(simbolo *s)
        {                           
            simbolo simb = simbolo();
            simb.id = s->id; simb.linea = s->linea; simb.columna = s->columna; simb.tipo.setTipo(s->tipo.getTipo());
            switch (s->tipo.getTipo())
            {
                case 0:      
                    simb.valor_entero = s->valor_entero;                                  
                    break;
                case 1:                    
                    simb.valor_real = s->valor_real;                    
                    break;       
                case 2:                    
                    simb.valor_booleano = s->valor_booleano;
                    break;                                                       
            }     
            /*Vemos si existe el símbolo*/
            auto it = tabla.find(s->id);            
            if (it != tabla.end())
            {
                /*Si existe verificamos el tipo y actualizamos el valor*/
                if(s->tipo.getTipo() == it->second.tipo.getTipo())
                {
                    switch (s->tipo.getTipo())
                    {
                        case 0:      
                            it->second.valor_entero = s->valor_entero;                                  
                            break;
                        case 1:                    
                            it->second.valor_real = s->valor_real;                    
                            break;       
                        case 2:                    
                            it->second.valor_booleano = s->valor_booleano;
                            break;                                                       
                    } 
                }
                else
                {
                    cout<<"TIPOERROR semántico en línea "<<s->linea<<": La variable "<<s->id<<" es de tipo "<<it->second.tipo.getNombre()<<", TIPOERROR tratando de asignar un valor de tipo "<<s->tipo.getNombre()<<endl;
                }
            }
            else
            {
                /*Si no existe lo insertamos.*/
                tabla.insert( pair<string,simbolo>(s->id,simb));
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

/*Expresiones*/
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
        /*TIPOPOSICION*/
        Literal(int l, int c, Expresion *y, Expresion *x)
        {
            valor = simbolo("",y,x,l,c);
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
            simbolo valor_izquierdo = izquierdo->getValor(e);
            simbolo valor_derecho = derecho->getValor(e);
            
            if(getTipo(e).esCadena()) // Resultado TIPOCADENA
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
            resultado.tipo = getTipo(e);
            if(resultado.getTipo(e).esReal()) // TIPOREAL
            {
                
                resultado.valor_real = (valor_izquierdo.getTipo(e).esReal() ?  valor_izquierdo.valor_real: valor_izquierdo.valor_entero) +
                                       (valor_derecho.getTipo(e).esReal()? valor_derecho.valor_real: valor_derecho.valor_entero);                                    
            }            
            if(resultado.getTipo(e).esEntero())
            {                                
                resultado.valor_entero = valor_izquierdo.valor_entero + valor_derecho.valor_entero;                                
            }                                        
            return resultado;


        }

        Type getTipo(Entorno *e) override
        {

            if(izquierdo->getTipo(e).esCadena() || derecho->getTipo(e).esCadena())
            {
                return Type(TIPOCADENA);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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
                return Type(TIPOERROR);
            }
            if(izquierdo->getTipo(e).esLogico() || derecho->getTipo(e).esLogico())
            {                           
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

            if(expr->getTipo(e).esEntero())
            {
                return Type(TIPOENTERO);
            }    

            return Type(TIPOREAL);        

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
            cout<<expr->getTipo(e).getNombre()<<"\t";
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
                    cout<< expr->getValor(e).valor_posicion[0] << expr->getValor(e).valor_posicion[1] <<endl;            
                    break;    
                default:
                    cout <<"TIPOERROR "<< type.getNombre() << endl;
                    break;                                                                      
            }
        }       

};





#endif