#include <iostream>
#include <allegro5/allegro.h>
#include <stdio.h>
#include "entorno.h"

using namespace std;

int main(int argc, char** argv)
{
/*----------------Estableciendo tamaño del tablero-----------*/

	entornoIniciar(10);
	entornoPonerEntrada(2,0,0.250000);
	entornoPonerSalida(2,9);
	entornoPonerObstaculo(1, 0);
	entornoPonerObstaculo(2, 1);
	entornoPonerObstaculo(3, 2);
	entornoPonerObstaculo(4, 3);
	entornoPonerObstaculo(5, 4);
	entornoPonerObstaculo(6, 5);
	entornoPonerObstaculo(7, 6);
	entornoPonerObstaculo(8, 7);

	/*Comenzando ejemplo*/
	entornoPonerEntrada(2,0,0.250000);
	entornoPonerSalida(2,9);
	entornoMostrarMensaje("caso1");
	entornoPonerFigura(1, 1,0.250000);
	entornoPonerFigura(1, 1,0.250000);
	entornoPonerFigura(2, 2,0.250000);
	entornoPonerFigura(2, 2,0.250000);
	entornoPonerFigura(1, 1,0.250000);
	entornoMostrarMensaje("Fin");

	/*Comenzando ejemplo*/
	entornoPonerEntrada(2,0,0.250000);
	entornoPonerSalida(2,9);
	entornoPonerFigura(1, 1,0.250000);
	entornoPonerFigura(2, 2,0.250000);
	entornoPonerFigura(2, 2,0.250000);
	entornoMostrarMensaje("Hola mundo");


entornoTerminar();
entornoPausa(3);
	return 0;
}