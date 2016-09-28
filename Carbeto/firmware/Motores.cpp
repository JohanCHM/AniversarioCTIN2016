/****************************************************************************************
*	Nombre de archivo: Motores.h
*   Proyecto: Carbeto
*	Definici�n de las funciones de la clase Motores
*
****************************************************************************************/

#include "Motores.h"

/** Constructor with the pins for the motors
* \param pwmA pin pwm del motor A
* \param motA1 pin 1 del motor A
* \param motA2 pin 2 del motor A
* \param pwmB pin pwm del motor B
* \param motB1 pin 1 del motor A
* \param motB2 pin 2 del motor A
*/
Motores::Motores(int pwmA, int motA1, int motA2, int pwmB, int motB1, int motB2)
{
	//Define los pines de salida para el motor A
	pinPWM_A = pwmA;
	pinAIN1 = motA1;
	pinAIN2 = motA2;

	//Define los pines de salida para el motor A
	pinPWM_B = pwmB;
	pinBIN1 = motB1;
	pinBIN2 = motB2;

}


/** Inicializa los motores */
void Motores::begin()
{
	//Inicializacion de pines
	pinMode(pinPWM_A, OUTPUT);
	pinMode(pinAIN1, OUTPUT);
	pinMode(pinAIN2, OUTPUT);

	pinMode(pinPWM_B, OUTPUT);
	pinMode(pinBIN1, OUTPUT);
	pinMode(pinBIN2, OUTPUT);

	configurarA(255, 255, 255);	//Inicializa a full PWM a
	configurarB(255, 255, 255);	//Inicializa a full PWM b

	setTacelera(250);		//Inicializa el tiempo de aceleracion
	setTdesacelera(250);	//Inicializa el tiempo de desaceleracion
}

/** Configurar el pwm del motor A
* \param pwmA Valor de calibracion para el motor A hacia delante
* \param pwmI Valor de calibracion para el motor A hacia izquierda
* \param pwmD Valor de calibracion para el motor A hacia la derecha
*/
void Motores::configurarA(uint8_t pwmA, uint8_t pwmI, uint8_t pwmD)
{
	pwm_motorAadel = pwmA;	//!< PWM de calibraci�n para el motor A adel
	pwm_motorAizq = pwmI;	//!< PWM de calibraci�n para el motor A izq
	pwm_motorAder = pwmD;	//!< PWM de calibraci�n para el motor A der	
}

/** Configurar el pwm de avance del motor A
* \param pwmB Valor de pwm de Avance
*/
void Motores::configurarAavance(uint8_t pwm)
{
	pwm_motorAadel = pwm;
}

/** Regresa el valor del pwm del motor A hacia adelante
* \return pwmA Valor de calibracion para el motor A hacia adelante
*/
uint8_t Motores::retroPWMaAdelante()
{
	return pwm_motorAadel;
}

/** Regresa el valor del pwm del motor A hacia la izquierda
* \return pwmA Valor de calibracion para el motor A hacia la izquierda
*/
uint8_t Motores::retroPWMaIzquierda()
{
	return pwm_motorAizq;
}

/** Regresa el valor del pwm del motor A hacia la derecha
* \return pwmA Valor de calibracion para el motor A hacia la derecha
*/
uint8_t Motores::retroPWMaDerecha()
{
	return pwm_motorAder;
}

/** Configurar el pwm del motor B
* \param pwmB Valor de calibracion para el motor B
*/
void Motores::configurarB(uint8_t pwmA, uint8_t pwmI, uint8_t pwmD)
{
	pwm_motorBadel = pwmA;
	pwm_motorBizq = pwmI;
	pwm_motorBder = pwmD;
}

/** Configurar el pwm de avance del motor B
* \param pwmB Valor de pwm de Avance
*/
void Motores::configurarBavance(uint8_t pwm)
{
	pwm_motorBadel = pwm;
}


/** Regresa el valor del pwm del motor B hacia adelante
* \return PWMbA Valor de calibracion para el motor B hacia adelante
*/
uint8_t Motores::retroPWMbAdelante()
{
	return pwm_motorBadel;
}

/** Regresa el valor del pwm del motor B hacia izquierda
* \return PWMbI Valor de calibracion para el motor B hacia izquierda
*/
uint8_t Motores::retroPWMbIzquierda()
{
	return pwm_motorBizq;
}

/** Regresa el valor del pwm del motor B hacia derecha
* \return PWMbD Valor de calibracion para el motor B hacia derecha
*/
uint8_t Motores::retroPWMbDerecha()
{
	return pwm_motorBder;
}

/** Cambia el tiempo de aceleracion
* \param tiempo Timepo que tarda en acelerar
*/
void Motores::setTacelera(uint32_t tiempo)
{
	t_acelera = tiempo;
}

/** Regresa el tiempo que tarda en acelerar
* \return tiempo Timepo que tarda en acelerar
*/
uint32_t Motores::getTacelera()
{
	return t_acelera;
}

/** Cambia el tiempo de desaceleracion
* \param tiempo Timepo que tarda en desacelerar
*/
void Motores::setTdesacelera(uint32_t tiempo)
{
	t_desacelera = tiempo;
}


/** Regresa el tiempo que tarda en desacelerar
* \return tiempo Timepo que tarda en desacelerar
*/
uint32_t Motores::getTdesacelera()
{
	return t_desacelera;
}

/** Acelera hasta el motor hasta el PWM establecido
* \return time El tiempo que tarda en acelerar el motor
*/
uint32_t Motores::aceleraAdelante()
{
	//pwm progresivos para acelerar los motores
	uint8_t pwmActual_motorA = 0;	
	uint8_t pwmActual_motorB = 0;
	

	//Acciona el motor A hacia adelante con el PWM definido para A
	analogWrite(pinPWM_A, pwmActual_motorA);
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);


	//Acciona el motor B hacia adelante con el PWM definido para B
	analogWrite(pinPWM_B, pwmActual_motorB);
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);

	//aceleración en rampa paralela de los motores de acuerdo a su pwm de configuracion
	for (uint16_t contador = 0; contador < getTacelera(); contador++)
	{

		// Incremento proporcional de motor A
		pwmActual_motorA = map(contador, 0, getTacelera(), 0, pwm_motorAadel);
		analogWrite(pinPWM_A, pwmActual_motorA);

		// Incremento proporcional de motor B
		pwmActual_motorB = map(contador, 0, getTacelera(), 0, pwm_motorBadel);
		analogWrite(pinPWM_B, pwmActual_motorB);

		delay(1);
		
	}
	
}		//End acelerarAdelante

/** Acelera hasta el motor hasta el PWM establecido
* \return time El tiempo que tarda en acelerar el motor
*/
uint32_t Motores::aceleraDerecha()
{
	//pwm progresivos para acelerar los motores
	uint8_t pwmActual_motorA = 0;
	uint8_t pwmActual_motorB = 0;


	//Acciona el motor A hacia adelante con el PWM definido para A
	analogWrite(pinPWM_A, pwmActual_motorA);
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);


	//Acciona el motor B hacia adelante con el PWM definido para B
	analogWrite(pinPWM_B, pwmActual_motorB);
	digitalWrite(pinBIN1, HIGH);
	digitalWrite(pinBIN2, LOW);

	//aceleración en rampa paralela de los motores de acuerdo a su pwm de configuracion
	for (uint16_t contador = 0; contador < getTacelera(); contador++)
	{

		// Incremento proporcional de motor A
		pwmActual_motorA = map(contador, 0, getTacelera(), 0, pwm_motorAder);
		analogWrite(pinPWM_A, pwmActual_motorA);

		// Incremento proporcional de motor B
		pwmActual_motorB = map(contador, 0, getTacelera(), 0, pwm_motorBder );
		analogWrite(pinPWM_B, pwmActual_motorB);

		delay(1);

	}

}		//End acelerarDerecha

/** Acelera hasta el motor hasta el PWM establecido
* \return time El tiempo que tarda en acelerar el motor
*/
uint32_t Motores::aceleraIzquierda()
{
	//pwm progresivos para acelerar los motores
	uint8_t pwmActual_motorA = 0;
	uint8_t pwmActual_motorB = 0;


	//Acciona el motor A hacia adelante con el PWM definido para A
	analogWrite(pinPWM_A, pwmActual_motorA);
	digitalWrite(pinAIN1, HIGH);
	digitalWrite(pinAIN2, LOW);


	//Acciona el motor B hacia adelante con el PWM definido para B
	analogWrite(pinPWM_B, pwmActual_motorB);
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);

	//aceleración en rampa paralela de los motores de acuerdo a su pwm de configuracion
	for (uint16_t contador = 0; contador < getTacelera(); contador++)
	{

		// Incremento proporcional de motor A
		pwmActual_motorA = map(contador, 0, getTacelera(), 0, pwm_motorAizq);
		analogWrite(pinPWM_A, pwmActual_motorA);

		// Incremento proporcional de motor B
		pwmActual_motorB = map(contador, 0, getTacelera(), 0, pwm_motorBizq);
		analogWrite(pinPWM_B, pwmActual_motorB);

		delay(1);

	}

}		//End acelerarIzquierda

/** Desacelera hasta el motor hasta el PWM establecido
* \return time El tiempo que tarda en acelerar el motor
*/
uint32_t Motores::desaceleraAdelante()
{
	uint8_t pwmActual_motorA = 0;
	uint8_t pwmActual_motorB = 0;
	
	//Acciona el motor A hacia adelante con el PWM definido para A
	analogWrite(pinPWM_A, pwm_motorAadel);
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);


	//Acciona el motor B hacia adelante con el PWM definido para B
	analogWrite(pinPWM_B, pwm_motorAadel);
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);

	//aceleración en rampa paralela de los motores de acuerdo a su pwm de configuracion
	for (uint16_t contador = getTdesacelera(); contador > 0; contador--)
	{

		// Incremento proporcional de motor A
		pwmActual_motorA = map(contador, 0, getTdesacelera(), 0, pwm_motorAadel);
		analogWrite(pinPWM_A, pwmActual_motorA);

		// Incremento proporcional de motor B
		pwmActual_motorB = map(contador, 0, getTdesacelera(), 0, pwm_motorBadel);
		analogWrite(pinPWM_B, pwmActual_motorB);

		delay(1);

	}

}

/**	Mueve la llanta A
*/
void Motores::mueveA()
{
	analogWrite(pinPWM_A, pwm_motorAadel);
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);
}

/**	Mueve la llanta B
*/
void Motores::mueveB()
{
	analogWrite(pinPWM_B, pwm_motorBadel);
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);

}

/** Mueve los dos motores hacia adelante */
void Motores::avanza()
{
	//Acciona el motor A hacia adelante con el PWM definido para A
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);
	analogWrite(pinPWM_A, retroPWMaAdelante());

	//Acciona el motor B hacia adelante con el PWM definido para B
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);
	analogWrite(pinPWM_B, retroPWMbAdelante());
}

/** Mueve los dos motores hacia adelante */
void Motores::retrocede()
{
	//Acciona el motor A hacia atr�s con el PWM definido para A
	digitalWrite(pinAIN1, HIGH);
	digitalWrite(pinAIN2, LOW);
	analogWrite(pinPWM_A, retroPWMaAdelante());

	//Acciona el motor B hacia atr�s con el PWM definido para B
	digitalWrite(pinBIN1, HIGH);
	digitalWrite(pinBIN2, LOW);
	analogWrite(pinPWM_B, retroPWMbAdelante());
}

/** Detiene los motores */
void Motores::detiene()
{
	//Acciona el motor A hacia atr�s con el PWM definido para A
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, LOW);
	analogWrite(pinPWM_A, retroPWMaAdelante());

	//Acciona el motor B hacia atr�s con el PWM definido para B
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, LOW);
	analogWrite(pinPWM_B, retroPWMbAdelante());
}

/** Gira a la Izquierda */
void Motores::girarIzq()
{
	//Acciona el motor A hacia adelante con el PWM definido por el mapeo MUCHO
	digitalWrite(pinAIN1, HIGH);
	digitalWrite(pinAIN2, LOW);
	analogWrite(pinPWM_A, retroPWMaIzquierda());

	//Acciona el motor B hacia atr�s con el PWM definido por el mapeo POCO
	digitalWrite(pinBIN1, LOW);
	digitalWrite(pinBIN2, HIGH);
	analogWrite(pinPWM_B, retroPWMbIzquierda());
}

/** gira a la Derecha*/
void Motores::girarDer()
{
	//Acciona el motor A hacia adelante con el PWM definido por el mapeo POCO
	digitalWrite(pinAIN1, LOW);
	digitalWrite(pinAIN2, HIGH);
	analogWrite(pinPWM_A, retroPWMaDerecha());

	//Acciona el motor B hacia atr�s con el PWM definido por el mapeo MUCHO
	digitalWrite(pinBIN1, HIGH);
	digitalWrite(pinBIN2, LOW);
	analogWrite(pinPWM_B, retroPWMbDerecha());
}



