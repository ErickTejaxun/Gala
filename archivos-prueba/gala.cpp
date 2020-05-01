#include <iostream>
#include <allegro5/allegro.h>
#include <stdio.h>
#include "entorno.h"

using namespace std;

int main(int argc, char** argv)
{
	// CONFIGURACIÓN DEL ENTORNO
	entornoIniciar(7);
	entornoPonerEntrada(5,6,0.400);
	entornoPonerSalida(0,2);

	entornoMostrarMensaje("OBSTACULOS");
	entornoPonerObstaculo(0,6);
	entornoPonerObstaculo(1,6);
	entornoPonerObstaculo(2,6);
	entornoPonerObstaculo(3,6);
	entornoPonerObstaculo(3,5);
	entornoPonerObstaculo(3,4);
	entornoPonerObstaculo(3,3);
	entornoPonerObstaculo(4,3);
	entornoPonerObstaculo(6,4);
	entornoPonerObstaculo(6,3);
	entornoPonerObstaculo(6,2);
	entornoPonerObstaculo(6,1);
	entornoPonerObstaculo(6,0);
	entornoPonerObstaculo(5,0);
	entornoPonerObstaculo(4,0);
	entornoPonerObstaculo(3,0);
	entornoPonerObstaculo(2,0);
	entornoPonerObstaculo(1,0);
	entornoPonerObstaculo(1,1);
	entornoPonerObstaculo(1,2);
	entornoPonerObstaculo(1,3);
	 
/* ------- RECORRIDO: EJEMPLO_1 ------- */

	entornoPonerEntrada(5,6,0.400);
	entornoPonerSalida(0,2);

	entornoMostrarMensaje("Soy Ejemplo 1");
	entornoPonerFigura(5,5,0.40);
	entornoPonerFigura(5,4,0.40);
	entornoPonerFigura(5,3,0.40);
	entornoPonerFigura(5,2,0.40);
	entornoPonerFigura(4,2,0.40);
	entornoPonerFigura(3,2,0.40);
	entornoPonerFigura(2,2,0.40);
	entornoPonerFigura(2,3,0.40);
	entornoPonerFigura(2,4,0.40);
	entornoPonerFigura(1,4,0.40);
	entornoPonerFigura(0,4,0.40);
	entornoPonerFigura(0,3,0.40);
	entornoPonerFigura(0,2,0.40);
	entornoPonerFiguraSalida(0,2);
	entornoMostrarMensaje ("  ¡ Lo conseguiste !    ");
	 
/* ------- RECORRIDO: EJEMPLO_2 ------- */

	entornoPonerEntrada(5,6,0.400);
	entornoPonerSalida(0,2);

	entornoMostrarMensaje("Soy Ejemplo 2");
	entornoPonerFigura(4,6,0.40);
	entornoPonerFigura(4,5,0.40);
	entornoPonerFigura(4,4,0.40);
	entornoPonerFigura(5,4,0.40);
	entornoPonerFigura(5,3,0.40);
	entornoPonerFigura(5,2,0.40);
	entornoPonerFigura(5,1,0.40);
	entornoPonerFigura(4,1,0.40);
	entornoPonerFigura(3,1,0.40);
	entornoPonerFigura(2,1,0.40);
	entornoPonerFigura(2,2,0.40);
	entornoPonerFigura(2,3,0.40);
	entornoPonerFigura(2,4,0.40);
	entornoPonerFigura(2,5,0.40);
	entornoPonerFigura(1,5,0.40);
	entornoPonerFigura(0,5,0.40);
	entornoPonerFigura(0,4,0.40);
	entornoPonerFigura(0,3,0.40);
	entornoPonerFigura(0,2,0.40);
	entornoPonerFiguraSalida(0,2);
	entornoMostrarMensaje ("  ¡ Lo conseguiste !    ");
	 
/* ------- RECORRIDO: EJEMPLO_3 ------- */

	entornoPonerEntrada(5,6,0.400);
	entornoPonerSalida(0,2);

	entornoMostrarMensaje("Soy Ejemplo 3");
	entornoPonerFigura(5,5,0.40);
	entornoPonerFigura(5,4,0.40);
	entornoPonerFigura(5,3,0.40);
	entornoPonerFigura(5,2,0.40);
	entornoPonerFigura(5,1,0.40);
	entornoPonerFigura(5,0,0.40);
	entornoPonerChoque(5,0);
	entornoMostrarMensaje ("  ¡ Mala suerte !    ");
	entornoMostrarMensajeFin ("  ¡ Fin !    ");
	entornoTerminar();

	entornoPausa(1);

	return 0;
}
