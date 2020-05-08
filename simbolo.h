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

using namespace std;
class Expresion;


class simbolo 
{
    public:       
        int tipo;
        int valor_entero;
        float valor_real;   
        bool valor_booleano;
        Expresion *valor_posicion[2];
        string valor_cadena;
        string id;
        int linea;
        int columna;
        /*Constructores*/
        simbolo() {} ;
        simbolo(string i, bool v, int l, int c): id(i), valor_booleano(v), linea(l), columna(c){tipo = 0;};
        simbolo(string i, int v, int l, int c): id(i), valor_entero(v), linea(l), columna(c){tipo = 1;};
        simbolo(string i, float v, int l, int c): id(i), valor_real(v), linea(l), columna(c){tipo = 2;};
        simbolo(string i, string v, int l, int c): id(i), valor_cadena(v), linea(l), columna(c){tipo = 3;};
        simbolo(string i, Expresion *y, Expresion *x, int l, int c): id(i), linea(l), columna(c){ tipo =4; valor_posicion[0] = y; valor_posicion[1] = x;}
        bool esEntero(){ return tipo == 0;};
        bool esReal(){ return tipo ==1;};
        bool esLogico(){ return tipo == 2;};
        bool esCadena(){ return tipo ==3;}
        bool esPosicion(){return tipo==4;}

};

class TablaSimbolos
{
    public:
        map<string, simbolo> tabla;  
        string tipos[4] = {"entero","real","lógico","error"};
        string logicos[2] = {"falso","verdadero"};

        string getCadenaData()
        {
            string cadena = "Nombre\t\tTipo\t\tValor";            
            map<string, simbolo>::iterator item;
            for(item = tabla.begin(); item != tabla.end(); item++)
            {
                switch (item->second.tipo)
                {
                    case 0:
                        cadena = cadena + "\n"+item->second.id +"\t\t\t"+tipos[item->second.tipo]+"\t\t\t"
                        +to_string(item->second.valor_entero);
                        break;                
                    case 1:
                        cadena = cadena + "\n"+item->second.id +"\t\t\t"+tipos[item->second.tipo]+"\t\t\t"
                        +to_string(item->second.valor_real);
                        break;                
                    case 2:
                        cadena = cadena + "\n"+item->second.id +"\t\t\t"+tipos[item->second.tipo]+"\t\t\t"
                        +logicos[item->second.valor_booleano];
                        break;                                                        
                    default:
                        break;
                }                
            }
            return cadena;
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
                tmp->tipo = 3; // Error                  
                cout<<"Error semántico en línea "<<linea <<": Símbolo "<< id<<" no encontrado "<<endl;
                return tmp;
            }                
        }        

        int insertarSimbolo(simbolo *s)
        {                           
            simbolo simb = simbolo();
            simb.id = s->id; simb.linea = s->linea; simb.columna = s->columna; simb.tipo = s->tipo;                    
            switch (s->tipo)
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
                if(s->tipo == it->second.tipo)
                {
                    switch (s->tipo)
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
                    cout<<"Error semántico en línea "<<s->linea<<": La variable "<<s->id<<" es de tipo "<<tipos[it->second.tipo]<<", error tratando de asignar un valor de tipo "<<tipos[s->tipo]<<endl;
                }
            }
            else
            {
                /*Si no existe lo insertamos.*/
                tabla.insert( pair<string,simbolo>(s->id,simb));
            }                    
            
        }
};

class Entorno
{
    public:
    TablaSimbolos tabla;
    Entorno *padre; /*Puntero hacia el entorno padre.*/
};


/*Clase que sirve como interfaz para las clases especificas*/


/*
class NodoAST
{
    public:
        int type_AST  = 0; //0 Intruccion, 1 = Expresion
        int getLinea(){} ;
        int getColumna(){};
        bool esInstruccion(){};
        bool esExpresion(){};
        simbolo getValor(){};  // Expresion
        virtual void ejecutar() = 0;     //Instruccion
        int getTipo(){};     
};
*/


class NodoAST
{
    public:
        virtual ~NodoAST() { };
        virtual int getLinea() = 0;
        virtual int getColumna()= 0;
        virtual bool esInstruccion()= 0;
        virtual bool esExpresion()= 0;
        virtual simbolo getValor()= 0;  
        virtual void ejecutar()= 0;    
        virtual int getTipo()= 0;     
};


class Expresion: public NodoAST
{
    public:   
        bool esInstruccion() override { return false;}   
        bool esExpresion() override { return true;}  
        void ejecutar() override {;} 
    
};

class Instruccion: public NodoAST
{
    public:
        bool esInstruccion() override { return true;}   
        bool esExpresion() override{ return false;}   
        simbolo getValor() override {;}    
};

/*Expresiones*/
class Literal: public Expresion
{
    public:      
        simbolo valor;
        bool esInstruccion() override { return false;}   
        bool esExpresion()override{ return true;}  

        /*Constructores*/
        /*Entero*/
        Literal(int l, int c, int val)
        {
            valor = simbolo("",val,l,c);
        }
        /*booleano*/
        Literal(int l, int c, bool val)
        {
            valor = simbolo("",val,l,c);
        }
        /*Real*/
        Literal(int l, int c, float val)
        {
            valor = simbolo("",val,l,c);
        }
        /*Cadena*/
        Literal(int l, int c, string val)
        {
            valor = simbolo("",val,l,c);
        }
        /*Posicion*/
        Literal(int l, int c, Expresion *y, Expresion *x)
        {
            valor = simbolo("",y,x,l,c);
        }

        simbolo getValor() override
        {
            return valor;
        }

        int getTipo() override
        {
            return valor.tipo;
        }

        int getLinea() override { return valor.linea;}
        int getColumna() override{ return valor.columna;}
};


/*Instrucciones*/
class Escribir: public Instruccion
{
    public:
        bool esInstruccion() override{ return true;}   
        bool esExpresion() override{ return false;} 
        NodoAST *expr;
        int linea;
        int columna;

        Escribir(int l, int c, NodoAST *e)
        {
            linea = l;
            columna = c;
            expr= e;
        }

        int getLinea() override{return linea;}
        int getColumna()override{return columna;}

        void ejecutar()override
        {
            switch (expr->getTipo())
            {
            case 0:
                cout<< expr->getValor().valor_booleano<<endl;            
                break;                
            case 1:
                cout<< expr->getValor().valor_entero<<endl;            
                break;        
            case 2:
                cout<< expr->getValor().valor_real<<endl;            
                break;        
            case 3:
                cout<< expr->getValor().valor_cadena<<endl;            
                break;        
            case 4:
                cout<< expr->getValor().valor_posicion[0] << expr->getValor().valor_posicion[1] <<endl;            
                break;                                                                          
            }
        }

        int getTipo() override
        {
            return 666;
        }        

};





#endif