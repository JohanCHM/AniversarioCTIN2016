// Motores.h
/****************************************************************************************
*	Nombre de archivo: Motores.h
*   Proyecto: Carbeto
*	Clase para controlar los motores
*
****************************************************************************************/

#ifndef _MOTORES_h
#define _MOTORES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Motores
{
public:
	// ****** CONSTRUCTOR

	/** Constructor with the pins for the motors
	* \param pwmA pin pwm del motor A
	* \param motA1 pin 1 del motor A
	* \param motA2 pin 2 del motor A
	* \param pwmB pin pwm del motor B
	* \param motB1 pin 1 del motor A
	* \param motB2 pin 2 del motor A
	*/
	Motores(int pwmA, int motA1, int motA2, int pwmB, int motB1, int motB2);

	/** Inicializa los motores */
	void begin();

	// ****** ACCESSORS & MUTATORS
	/** Configurar el pwm del motor A
	* \param pwmA Valor de calibracion para el motor A hacia delante
	* \param pwmI Valor de calibracion para el motor A hacia izquierda
	* \param pwmD Valor de calibracion para el motor A hacia la derecha
	*/
	void configurarA(uint8_t pwmA, uint8_t pwmI, uint8_t pwmD);

	/** Configurar el pwm de avance del motor A
	* \param pwm Valor de pwm de Avance
	*/
	void configurarAavance(uint8_t pwm);

	/** Regresa el valor del pwm del motor A hacia adelante
	* \return pwmA Valor de calibracion para el motor A hacia adelante
	*/
	uint8_t retroPWMaAdelante();

	/** Regresa el valor del pwm del motor A hacia la izquierda
	* \return pwmA Valor de calibracion para el motor A hacia la izquierda
	*/
	uint8_t retroPWMaIzquierda();

	/** Regresa el valor del pwm del motor A hacia la derecha
	* \return pwmA Valor de calibracion para el motor A hacia la derecha
	*/
	uint8_t retroPWMaDerecha();

	/** Configurar el pwm del motor B
	* \param pwmA Valor de calibracion para el motor B hacia delante
	* \param pwmI Valor de calibracion para el motor B hacia izquierda
	* \param pwmD Valor de calibracion para el motor B hacia la derecha
	*/
	void configurarB(uint8_t pwmA, uint8_t pwmI, uint8_t pwmD);

	/** Configurar el pwm de avance del motor B
	* \param pwm Valor de pwm de Avance
	*/
	void configurarBavance(uint8_t pwm);

	/** Regresa el valor del pwm del motor B hacia adelante
	* \return PWMbA Valor de calibracion para el motor B hacia adelante
	*/
	uint8_t retroPWMbAdelante();

	/** Regresa el valor del pwm del motor B hacia izquierda
	* \return PWMbI Valor de calibracion para el motor B hacia izquierda
	*/
	uint8_t retroPWMbIzquierda();

	/** Regresa el valor del pwm del motor B hacia derecha
	* \return PWMbD Valor de calibracion para el motor B hacia derecha
	*/
	uint8_t retroPWMbDerecha();

	/** Cambia el tiempo de aceleracion
	* \param tiempo Timepo que tarda en acelerar
	*/
	void setTacelera(uint32_t tiempo);

	/** Regresa el tiempo que tarda en acelerar
	* \return tiempo Timepo que tarda en acelerar
	*/
	uint32_t getTacelera();

	/** Cambia el tiempo de desaceleracion
	* \param tiempo Timepo que tarda en desacelerar
	*/
	void setTdesacelera(uint32_t tiempo);

	/** Regresa el tiempo que tarda en desacelerar
	* \return tiempo Timepo que tarda en desacelerar
	*/
	uint32_t getTdesacelera();

	
	// ****** FUNCTIONS
	/** Acelera hasta el motor hasta el PWM establecido
	* \return time El tiempo que tarda en acelerar el motor
	*/
	uint32_t aceleraAdelante();

	/** Acelera el motor en giro izq hasta hasta el PWM establecido
	* \return time El tiempo que tarda en acelerar el motor
	*/
	uint32_t aceleraIzquierda();

	/** Acelera el motor en giro der hasta hasta el PWM establecido
	* \return time El tiempo que tarda en acelerar el motor
	*/
	uint32_t aceleraDerecha();

	/** Desacelera hasta el motor hasta el PWM establecido
	* \return time El tiempo que tarda en acelerar el motor
	*/
	uint32_t desaceleraAdelante();

	/**	Mueve la llanta A
	*/
	void mueveA();

	/**	Mueve la llanta B
	*/
	void mueveB();

	/** Mueve los dos motores hacia adelante */
	void avanza();

	/** Mueve los dos motores hacia adelante */
	void retrocede();

	/** Detiene los motores */
	void detiene();

	/** Gira a la Izquierda */
	void girarIzq();

	/** gira a la Derecha*/
	void girarDer();

private:
	// ***** DATA MEMBERS
	int pinPWM_A;	//!< PWM del motor A
	int pinAIN1;	//!< pin 1 del motor A
	int pinAIN2;	//!< pin 2 del motor A
	int pinPWM_B;	//!< PWM del motor B
	int pinBIN1;	//!< pin 1 del motor B
	int pinBIN2;	//!< pin 2 del motor B
	uint8_t pwm_motorAadel;	//!< PWM de calibración para el motor A adel
	uint8_t pwm_motorAizq;	//!< PWM de calibración para el motor A izq
	uint8_t pwm_motorAder;	//!< PWM de calibración para el motor A der
	uint8_t pwm_motorBadel;	//!< PWM de calibración para el motor B adel
	uint8_t pwm_motorBizq;	//!< PWM de calibración para el motor B izq
	uint8_t pwm_motorBder;	//!< PWM de calibración para el motor B der
	uint32_t t_acelera;		//!< Tiempo de aceleracion
	uint32_t t_desacelera;	//!< Tiempo de desaceleracion

}; //End of class motores

#endif

