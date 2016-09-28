/****************************************************************************************
*	Nombre de archivo: pines.h
*   Proyecto: Carbeto
*	Definición de pines para Arduino Nano y UNO
*
****************************************************************************************/
#ifndef _PINES_h
#define _PINES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//***** SOFTWARE SERIAL
#define PIN_RX		4
#define PIN_TX		2

//***** ENTRADAS
#define SEN_PISO    16  //ADC2
#define SEN_CHOQUE  17  //ADC3

#define ACTIVADO 1
#define DESACTIVADO 0


//***** SALIDAS
#define PIN_RGB     3
#define PIN_BUZZ    5 //

#define PIN_PWM_A   6 //!< PWM de control para el motor A
#define PIN_AIN1    8
#define PIN_AIN2    7

#define PIN_PWM_B   11  //!< PWM de control para el motor B
#define PIN_BIN1    9
#define PIN_BIN2    10

//***** UBICACIONES DE MEMORIA
#define DIR_TIEMPO_AVANZAR	0
#define DIR_TIEMPO_GIRAR	10

#define DIR_PWM_MOTORA_ADELENTE		100
#define DIR_PWM_MOTORA_IZQUIERDA	110
#define DIR_PWM_MOTORA_DERECHA		120
#define DIR_PWM_MOTORB_ADELANTE		130
#define DIR_PWM_MOTORB_IZQUIERDA	140
#define DIR_PWM_MOTORB_DERECHA		150

#endif

