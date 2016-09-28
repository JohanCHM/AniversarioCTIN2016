
/****************************************************************************************
*	Nombre de archivo: colors.h
*   Proyecto: Carbeto
*	Definición de colores para cada estado
*
****************************************************************************************/

#ifndef _COLORS_h
#define _COLORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define BARRIDO		100		//!< milisegundos para el barrido en groovy

/*
*  Colors in the Tags for "Aldea Digital!
*/

#define IZQ         0xA9D909
#define FRENTE      0x00641A
#define DER         0xFF8C00
#define REPITE      0xFF00FF
#define CHIN        0xFF0000
#define AZUL        0x0000FF

/*
*  Number of LEDs used to display the colors
*/
#define LED_COUNT   5

#endif

