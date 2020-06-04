#include <iostream>
#include <allegro5/allegro.h>
#include <stdio.h>
#include "entorno.h"

using namespace std;

int main(int argc, char** argv)
{
/*----------------Estableciendo tamaño del tablero-----------*/

	entornoIniciar(7);
	entornoPonerEntrada(5,6,0.750000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("OBSTACULOS22");
	entornoPonerObstaculo(0, 6);
	entornoPonerObstaculo(1, 6);
	entornoPonerObstaculo(2, 6);
	entornoPonerObstaculo(3, 6);
	entornoPonerObstaculo(3, 5);
	entornoPonerObstaculo(3, 4);
	entornoPonerObstaculo(3, 3);
	entornoPonerObstaculo(4, 3);
	entornoPonerObstaculo(6, 4);
	entornoPonerObstaculo(6, 3);
	entornoPonerObstaculo(6, 2);
	entornoPonerObstaculo(6, 1);
	entornoPonerObstaculo(6, 0);
	entornoPonerObstaculo(5, 0);
	entornoPonerObstaculo(4, 0);
	entornoPonerObstaculo(3, 0);
	entornoPonerObstaculo(2, 0);
	entornoPonerObstaculo(1, 0);
	entornoPonerObstaculo(1, 1);
	entornoPonerObstaculo(1, 2);

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.750000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 1");
	entornoPonerFigura(5, 5,0.750000);
	entornoPonerFigura(5, 4,0.750000);
	entornoPonerFigura(5, 3,0.750000);
	entornoPonerFigura(5, 2,0.750000);
	entornoPonerFigura(4, 2,0.750000);
	entornoPonerFigura(3, 2,0.750000);
	entornoPonerFigura(2, 2,0.750000);
	entornoPonerChoque(1, 2);
	entornoMostrarMensaje ("  ¡ Mala suerte !    ");

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.750000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 1333");
	entornoPonerFigura(5, 5,0.750000);
	entornoPonerFigura(5, 4,0.750000);
	entornoPonerFigura(5, 3,0.750000);
	entornoPonerFigura(5, 2,0.750000);
	entornoPonerFigura(4, 2,0.750000);
	entornoPonerFigura(3, 2,0.750000);
	entornoPonerFigura(2, 2,0.750000);
	entornoPonerFigura(2, 3,0.750000);
	entornoPonerFigura(2, 4,0.750000);
	entornoPonerFigura(2, 5,0.750000);
	entornoPonerFigura(1, 5,0.750000);
	entornoPonerFigura(0, 5,0.750000);
	entornoPonerFigura(0, 4,0.750000);
	entornoPonerFigura(0, 3,0.750000);

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.750000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 2");
	entornoPonerFigura(4, 6,0.750000);
	entornoPonerFigura(4, 5,0.750000);
	entornoPonerFigura(4, 4,0.750000);
	entornoPonerFigura(5, 4,0.750000);
	entornoPonerFigura(5, 3,0.750000);
	entornoPonerFigura(5, 2,0.750000);
	entornoPonerFigura(5, 1,0.750000);
	entornoPonerFigura(4, 1,0.750000);
	entornoPonerFigura(3, 1,0.750000);
	entornoPonerFigura(2, 1,0.750000);
	entornoPonerFigura(2, 2,0.750000);
	entornoPonerFigura(2, 3,0.750000);
	entornoPonerFigura(2, 4,0.750000);
	entornoPonerFigura(2, 5,0.750000);
	entornoPonerFigura(1, 5,0.750000);
	entornoPonerFigura(0, 5,0.750000);
	entornoPonerFigura(0, 4,0.750000);
	entornoPonerFigura(0, 3,0.750000);
	entornoPonerFiguraSalida(0, 2);
	entornoMostrarMensaje ("  ¡ Lo conseguiste !    ");

	/*Comenzando ejemplo*/
	entornoPonerEntrada(5,6,0.750000);
	entornoPonerSalida(0,2);
	entornoMostrarMensaje("Soy Ejemplo 3");
	entornoPonerFigura(5, 5,0.750000);
	entornoPonerFigura(5, 4,0.750000);
	entornoPonerFigura(5, 3,0.750000);
	entornoPonerFigura(5, 2,0.750000);
	entornoPonerFigura(5, 1,0.750000);
	entornoPonerChoque(5, 0);
	entornoMostrarMensaje ("  ¡ Mala suerte !    ");
	entornoMostrarMensajeFin ("  ¡ Fin !    ");

	entornoTerminar();
	entornoPausa(3);
	return 0;
}