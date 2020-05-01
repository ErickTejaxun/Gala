/*
Estructuras para el manejo e implementación de la tabla de símbolos
Erick Tejaxún 
Teoría de Lenguajes UEx 2020
*/

#ifndef simbolo_H
# define simbolo_H
#include <string>
#include <stdio.h>
#include <map>
#include <iostream>

using namespace std;
class simbolo 
{
    public:       
        int tipo;
        int valor_entero;
        float valor_real;   
        bool valor_booleano;
        string id;
        int linea;
        int columna;
        /*Constructores*/
        simbolo() {} ;
        simbolo(string i, bool v, int l, int c): id(i), valor_booleano(v), linea(l), columna(c){tipo = 0;};
        simbolo(string i, int v, int l, int c): id(i), valor_entero(v), linea(l), columna(c){tipo = 1;};
        simbolo(string i, float v, int l, int c): id(i), valor_real(v), linea(l), columna(c){tipo = 2;};
        bool esEntero(){ return tipo == 0;};
        bool esReal(){ return tipo ==1;};
        bool esLogico(){ return tipo == 2;};
      
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


#endif