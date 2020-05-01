/**

 INTRODUCCIÓN A LA PROGRAMACIÓN / FUNDAMENTOS DE PROGRAMACIÓN
 Curso 2019/2020

 Nombre: entorno.cpp
 Descripción: Implementación del TAD Entorno para el proyecto Merge
 Encargado del manejo de la interfaz del juego.
 Autor:	Profesores de las asignaturas
 Fecha:	25/11/2012
 Fecha última modificación: 30/10/2019 Allegro5

 */

#include "entorno.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro5.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

// -------------------------------------------------------------
// Definicion de constantes privadas
// -------------------------------------------------------------



const int COLOR_LINEAS = COLOR_BLANCO;
const float GROSOR_LINEAS = 2.0;


const int ANCHO_VENTANA = 600;
const int ALTO_VENTANA  = 650;

// Definicón de constantes para posicionar los números en el tablero
const int DISTANCIA_COLS       = 45;  // Distancia entre columnas
const int DISTANCIA_FILAS      = 45;  // Distancia entre filas
const int SEPARACION_FILAS_COL = 4;  // separacion entre las filas y columnas

int ORIGEN_X           = 80 ;  // Origen de las x
const int ORIGEN_Y     = 60 ;  // Origen de las y

// valor del radio para el redondeo de las esquinas del rectangulo
const int RADIO = 12;

// definicion de las constantes para posicionar los marcadores
const int FILA_MARCADOR = 110;
const int COL_MARCADOR  = 14;



ALLEGRO_DISPLAY     *display;
ALLEGRO_FONT        *font;
ALLEGRO_EVENT_QUEUE *colaEventos = NULL;
ALLEGRO_BITMAP      *bitmap = NULL;
ALLEGRO_BITMAP      *bitmap2 = NULL;


// -------------------------------------------------------------
// Definición de módulos PRIVADOS
// -------------------------------------------------------------

// define el color en función de los valores makecol - allegro library
ALLEGRO_COLOR makecolor2(int color) {

	ALLEGRO_COLOR alColor;

	switch (color) {
	case COLOR_BLANCO:
		alColor = al_map_rgb (255, 255, 255);
		break;
	case COLOR_ROJO:
		alColor = al_map_rgb (255,   69,  0);
		break;
	case COLOR_NEGRO:
		alColor = al_map_rgb (0,    0,   0);
		break;
	case COLOR_AZUL:
		alColor = al_map_rgb (70,   150, 255);
		break;
	case COLOR_GRIS:
		alColor = al_map_rgb ( 47, 79, 79);
		break;
	case COLOR_AMARILLO:
		alColor = al_map_rgb ( 255, 255, 000);
		break;
	case COLOR_ORO:
		alColor = al_map_rgb ( 234, 190, 63);
		break;
	case COLOR_VERDE:
		alColor = al_map_rgb ( 036,231,017);
		break;
	case COLOR_GRIS_CLARO:
		alColor = al_map_rgb ( 192,192,192);
		break;
	case COLOR_PLATA:
		alColor = al_map_rgb ( 138,149,151);
		break;
	default:
		alColor = al_map_rgb (255, 255, 255);
		break; //color blanco
	}

	return alColor;
}
void invertirFC(int &fila, int &columna) {
	int aux = columna;
	columna = fila;
	fila = aux;
}

// -------------------------------------------------------------
// Definición de la interfaz PÚBLICA
// -------------------------------------------------------------

void entornoIniciar(int tam) {

	// Iniciar el entorno

	if (al_init()){
	al_install_keyboard();

	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);
	al_set_new_display_flags (ALLEGRO_WINDOWED);


	display = al_create_display(ANCHO_VENTANA,ALTO_VENTANA);
	al_init_primitives_addon(); //

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();


	// carga las fuentes con tamaño 18
	font = al_load_ttf_font("DroidSerif-Regular.ttf", 18, 0);

    // borra la ventana con el color blanco
    al_clear_to_color(makecolor2(COLOR_BLANCO));
    al_set_window_title(display, "Uex TL: Laberinto");

    // se refresca la pantalla
    colaEventos = al_create_event_queue();
    al_register_event_source(colaEventos, al_get_keyboard_event_source());
    ORIGEN_X = (ANCHO_VENTANA -((tam)*DISTANCIA_COLS))/2;

    // carga las imágenes utilizadas para representar al jugador
    bitmap = al_load_bitmap("Smile.png");
    assert(bitmap != NULL);

    bitmap2 = al_load_bitmap("NotSmile.png");
    assert(bitmap2 != NULL);

    //inicia todas las celdas
    for (int i=0; i<tam;i++)
    		for (int j=0; j<tam;j++)
    			entornoPonerVacio(i,j);


    al_flip_display();

	}
}

void entornoTerminar(){

	al_destroy_display          (display);
	al_destroy_event_queue      (colaEventos);
	al_destroy_font				(font);
	al_shutdown_primitives_addon();
	al_shutdown_font_addon      ();
}





void entornoPonerVacio(int fila, int columna) {
	invertirFC(fila, columna);

	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_filled_rectangle(x1, y1, x2, y2,makecolor2(COLOR_GRIS_CLARO));


	al_flip_display();

}

void entornoPonerEntrada(int fila, int columna,  float pausa) {
	invertirFC(fila, columna);
	float base, altura, ptoMedioX, ptoMedioY;
	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_bitmap(bitmap,x1,y1,0);
	al_flip_display();

	al_rest(pausa);

	al_draw_filled_rectangle(x1, y1, x2, y2, makecolor2(COLOR_ROJO));

	base      = (x2 - x1);
	altura    = (y2 - y1);
	ptoMedioX = ((base / 2.0) + x1);
	ptoMedioY = ((y2 + y1) / 2.0) - (altura / 2.0) + SEPARACION_FILAS_COL;

	al_draw_text(font, makecolor2(COLOR_BLANCO), ptoMedioX, ptoMedioY,	ALLEGRO_ALIGN_CENTER, "E");

	al_flip_display();

}
void entornoPonerSalida(int fila, int columna) {
	invertirFC(fila, columna);
	float base, altura, ptoMedioX, ptoMedioY;
	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_filled_rectangle(x1, y1, x2, y2, makecolor2(COLOR_VERDE));

	base      = (x2 - x1);
	altura    = (y2 - y1);
	ptoMedioX = ((base / 2.0) + x1);
	ptoMedioY = ((y2 + y1) / 2.0) - (altura / 2.0) + SEPARACION_FILAS_COL;

	al_draw_text(font, makecolor2(COLOR_BLANCO), ptoMedioX, ptoMedioY,	ALLEGRO_ALIGN_CENTER, "S");

	al_flip_display();

}

void entornoPonerFiguraSalida(int fila, int columna) {
	invertirFC(fila, columna);

	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_filled_rectangle(x1, y1, x2, y2, makecolor2(COLOR_VERDE));

	al_draw_bitmap(bitmap,x1,y1,0);
	al_flip_display();

}
void entornoPonerObstaculo(int fila, int columna) {
	invertirFC(fila, columna);

	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_filled_rectangle(x1, y1, x2, y2, makecolor2(COLOR_NEGRO));


	al_draw_line (x1,y1,x2,y2,makecolor2(COLOR_BLANCO),3);
	al_draw_line (x1,y2,x2,y1,makecolor2(COLOR_BLANCO),3);
	al_flip_display();

}

void entornoPonerFigura(int fila, int columna, float pausa) {
	invertirFC(fila, columna);

	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;


   al_draw_bitmap(bitmap,x1,y1,0);
   al_flip_display();
   al_rest(pausa);
   al_draw_filled_rectangle(x1, y1, x2, y2,makecolor2(COLOR_GRIS_CLARO));

	al_flip_display();

}


void entornoPonerChoque(int fila, int columna) {
	invertirFC(fila, columna);

	float x1 = (ORIGEN_X + fila    * DISTANCIA_COLS                   ) + SEPARACION_FILAS_COL;
	float y1 = (ORIGEN_Y + columna * DISTANCIA_FILAS                  ) + SEPARACION_FILAS_COL;
	float x2 = (ORIGEN_X + fila    * DISTANCIA_COLS  + DISTANCIA_COLS )	- SEPARACION_FILAS_COL;
	float y2 = (ORIGEN_Y + columna * DISTANCIA_FILAS + DISTANCIA_FILAS) - SEPARACION_FILAS_COL;

	al_draw_filled_rectangle(x1, y1, x2, y2,makecolor2(COLOR_NEGRO));
   al_draw_bitmap(bitmap2,x1,y1,0);

	al_flip_display();

}

void entornoMostrarMensaje( string msg) {
	int col = 80;
	int fila = 25;
	int alto = 25;
	int ancho = 600;

	al_draw_filled_rectangle(col-2 , fila,  col + ancho, fila + alto,  makecolor2(COLOR_BLANCO));
	al_draw_text(font, makecolor2(COLOR_GRIS),col,fila, ALLEGRO_ALIGN_LEFT, msg.c_str());
	al_flip_display();

}


void entornoMostrarMensajeFin(string msg) {
	int i, j;
		for (i = 0, j = 0; i < 40 && j < 40; i += 4, j += 4) {

			al_draw_filled_rectangle(i + ORIGEN_X,     j + ORIGEN_Y   + 200,  ORIGEN_X   +350 - i, ORIGEN_Y+   320 -j,  makecolor2(COLOR_BLANCO));
			al_draw_filled_rectangle(i + ORIGEN_X + 2 ,j + ORIGEN_Y +2+ 200, (ORIGEN_X-2)+350 - i, ORIGEN_Y -2 +320 -j,  makecolor2(COLOR_GRIS));

			usleep(25000); //25 milisegundos
			al_flip_display();
		}

	al_draw_text(font, makecolor2(COLOR_BLANCO),ORIGEN_X+100,ORIGEN_Y+250, ALLEGRO_ALIGN_LEFT, msg.c_str());
	al_flip_display();
	usleep(500000); // medio segundo
}


void entornoPausa(float pausa) {
	al_rest(pausa);
}

