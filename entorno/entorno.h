/**

	TEORÍA DE LENGUAJES
	Curso 2019/2020

	Nombre: entorno.h
	Descripción: Especificación del TAD Entorno para el proyecto Laberinto
	     	 	 encargado del manejo de la interfaz del juego.
	     	 	 versión allegro 5.2
	Autor:	Profesores de las asignaturas
    Fecha:	01/04/2020
*/

#ifndef ENTORNO_H_
#define ENTORNO_H_

#include <iostream>
#include <string>

using namespace std;


// -------------------------------------------------------------
// Definicion de constantes públicas
// -------------------------------------------------------------
// Definición de colores que usa el entorno

const int COLOR_AZUL      = 1;
const int COLOR_BLANCO    = 2;
const int COLOR_ROJO      = 3;
const int COLOR_NEGRO     = 4;
const int COLOR_GRIS      = 5;
const int COLOR_AMARILLO  = 6;
const int COLOR_ORO       = 7;
const int COLOR_VERDE     = 8;
const int COLOR_GRIS_CLARO= 9;
const int COLOR_PLATA     = 10;

//Definición del tamaño máximo del laberinto
const int MAX_TAMANO = 10;

// ----------------------------------------------------------------
// Declaración de la interfaz pública
// ----------------------------------------------------------------


 /*
  *	PRE:  4 <= tam <= MAX_TAMANO
  *	DESCRIPCIÓN: Inicia la interfaz gráfica del programa, preparada para
  *		  un tablero de tam x tam casillas.
  */
void entornoIniciar (int tam);


 /*
  *	DESCRIPCIÓN: Libera los recursos usados por el entorno gráfico
  */
void entornoTerminar ();






/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Deja en blanco la posición (fila, columna) del laberinto
 */
void entornoPonerVacio  (int fila, int columna);



/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca en la posición (fila, columna) la entrada del laberinto,
 * colocando la imagen que representa al jugador, durante un tiempo definido por "pausa".
 * Cuando la imagen desaparece se ve la letra "E"
 */
void entornoPonerEntrada(int fila, int columna,  float pausa);


/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca en la posición (fila, columna) la salida del laberinto,
 * Escribe la letra "S"
 */
void entornoPonerSalida(int fila, int columna);



/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca un obstaculo en la posición (fila, columna) del laberinto,
 */
void entornoPonerObstaculo(int fila, int columna);


/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca en la posición (fila, columna) la imagen que representa al jugador,
 * durante un tiempo definido por "pausa".
 * La imagen aparece y desaparece para dar sensación de movimiento.
 */
void entornoPonerFigura(int fila, int columna, float pausa);


/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca en la posición (fila, columna) correspondiente a la salida del laberinto
 * la imagen que representa al jugador.
 * La imagen no desaparece ya que es el final del proceso
 */
void entornoPonerFiguraSalida(int fila, int columna);



/*
 * PRE: { 0 <= fila < MAX_TAMANO }
 *      { 0 <= columna < MAX_TAMANO }
 *
 * DESCRIPCIÓN: Coloca en la posición (fila, columna) correspondiente a un obstáculo
 * la imagen que representa al jugador después de haber chocado con el obstáculo
 * La imagen no desaparece ya que es el final del proceso
 */
void entornoPonerChoque(int fila, int columna);



/*
 * PRE: "msg" está correctamente inicializado
 *
 * DESCRIPCIÓN: Muestra el mensaje "msg" en la zona superior de la ventana
 * el mensaje permanece visible hasta que se escriba otro
 */
void entornoMostrarMensaje( string msg);

/*
 * PRE: "msg" está correctamente inicializado
 *
 * DESCRIPCIÓN: Muestra el mensaje "msg" en el centro de la ventana para indicar que ha finalizado el juego
 */
void entornoMostrarMensajeFin (string msg);


/*
 * PRE: {0 < pausa}
 *
 * DESCRIPCIÓN: para la ejecución durante "pausa" segundos
 */
void entornoPausa(float pausa);

#endif
