#include <iostream>
#include <allegro5/allegro.h>
#include <stdio.h>
#include "entorno.h"

using namespace std;

int main(int argc, char** argv)
{
/*----------------Estableciendo tamaño del tablero-----------*/

	entornoIniciar(7);
	entornoPonerEntrada(5,6,0.500000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("OBSTACULOS22");
	entornoPonerObstaculo(0, 6);
	entornoPonerObstaculo(2, 0);
	entornoPonerObstaculo(3, 0);
	entornoPonerObstaculo(4, 0);

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.500000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 1");
	entornoPonerFigura(5, 2,0.500000);
	entornoPonerFigura(2, 2,0.500000);
	entornoPonerFigura(2, 4,0.500000);
	entornoPonerFigura(0, 4,0.500000);
	entornoPonerFiguraSalida(0, 2);

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.500000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 2");

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.500000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 3");


entornoTerminar();
entornoPausa(3);
	return 0;
}