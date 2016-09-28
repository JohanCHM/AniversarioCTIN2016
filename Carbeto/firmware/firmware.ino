/*  Nombre de archivo: main.ino 
*   Proyecto: Carbeto
*   Programa principal para el carrito de aldea
*/

//	************************* LIBRARIES ***********************************
#include "pitches.h"	// Cabecera con la definici�n de las notas musicales
#include "Motores.h"	// Clase para el control de los motores
#include "colors.h"		// Cabecera con la definici�n de los colores
#include "pines.h"		// Cabecera con la definici�n de los pines

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>


//	************************* VARIABLES ***********************************
//Movimientos
uint32_t tiempo;	//!< variable de almacenamiento de milisegundos
uint32_t tiempoAvanzar = 1000;	//!< tiempo para avanzar una casilla.
uint32_t tiempoGirarDer = 1000;	//!< tiempo para dar la vuelta a la Derecha
uint32_t tiempoGirarIzq = 1000;	//!< tiempo para dar la vuelta a la Izquierda
uint32_t tiempoPausado = 500;	//!< tiempo de pausa entre cada movimiento
uint8_t pwmTemporal;			//!< variable para estar guardando y manipulando pwm's
const uint8_t dalePWM = 5;	//!< valor de incremento para PWM's

Motores llantas = Motores(PIN_PWM_A, PIN_AIN1, PIN_AIN2, PIN_PWM_B, PIN_BIN1, PIN_BIN2);	//!< Objeto para accionar motores

//BLUETOOTH
SoftwareSerial BT1(PIN_RX, PIN_TX); //!< RX, TX recorder que se cruzan
char instruc[31];  //!< TRAMA DE COMUNICACION


// MAQUINA DE ESTADOS
enum _State {
	STA_ESPERA,
	STA_PROCESANDO,
	STA_SALVAR,
	STA_DECIDIR,
	STA_ADELANTE,
	STA_ADELANTEREP,
	STA_IZQUIERDA,
	STA_IZQUIERDAREP,
	STA_DERECHA,
	STA_DERECHAREP,
	STA_ERROR,
	STA_LEVANTADO,
	STA_CONFIGURAR,
	STA_CALIBADELTIEMPO,
	STA_CALIBADELMOVIMIENTO,
	STA_CALIBIZQTIEMPO,
	STA_CALIBIZQMOVIMIENTO,
	STA_CALIBDERTIEMPO,
	STA_CALIBDERMOVIMIENTO,
	STA_CONTROLREMOTO
};

_State State;//!< Estado actual en el que se encuentra el robot
int banderaPrimer = LOW;	//!< bandera para la primer corrida
int contadorInstrucciones = -1;	//!< variable para ir se�alando en que parte del arreglo se encuentra

// LEDs
Adafruit_NeoPixel luces = Adafruit_NeoPixel(LED_COUNT, PIN_RGB, NEO_GRB + NEO_KHZ400); //!< objeto para controlar los LEDs
uint8_t ledTope = 0;	// LED tope

//	************************* CONFIGURATION ***************************************
void setup()
{
	// MOTORES
	llantas.begin();
	llantas.detiene();

	//LUCES
	luces.begin();  // Inicia los LEDs
	ledOff(); //Apaga los LEDs
	
	//COMUNICACION SERIAL
	Serial.begin(9600);
	Serial.println("Comunicacion PC Lista");

	//BLUETOOTH
	BT1.begin(9600);

	//SONIDO
	//pinMode(PIN_BUZZ, OUTPUT);
	
	//TOPES
	pinMode(SEN_CHOQUE, INPUT_PULLUP);
	pinMode(SEN_PISO, INPUT_PULLUP);

	// ******* ACCIONES INICIALES ************
	State = STA_CONTROLREMOTO;
	cambioDeEstado(); //Inicializa las variables de cambio de estado

	// ******* MENSAJES ************
	Serial.println("Carbeto 14 Listo");
	BT1.println("Carbeto 14 Listo");
	
}

//	************************* PROGRAM ***************************************
void loop()
{
	switch (State)
	{
	
	//************Estados de movimiento
	//Estado ControlRemoto
	case STA_CONTROLREMOTO:
		//Primera entrada al estado
		if (banderaPrimer == LOW)
		{
			llantas.detiene();
			Serial.println("Estado ESPERA");
			BT1.println("Estado ESPERA");
			ledBlanco();
			ledOff();

			vaciarTrama();	//Limpia cualquier info en el serial del Bluetooth

			//Configuracion inicial de llantas

			llantas.configurarA(170, 170, 170);	// Guarda el primer PWM A
			llantas.configurarB(170, 170, 170);	// Guarda el segundo PWM B

			banderaPrimer = HIGH;
		}
		
		ledGroovy();

		if (BT1.available())
		{
			if (BT1.peek() == 'a')	//Avanzar
			{
				llantas.avanza();
				//Monitoreo
				Serial.println("a");
				BT1.println("a");

				//vaciar
				vaciarTrama();	//Vacia la trama anterior
				delay(5);			// Tiempo de asentamiento

			}	//End of primer caracter = a
			else if (BT1.peek() == 's')	//Detener
			{
				llantas.detiene();
				//Monitoreo
				Serial.println("s");
				BT1.println("s");


			}	//End of primer caracter =s
			else if (BT1.peek() == 'w')	//aumentar PWM izq
			{
				pwmTemporal = llantas.retroPWMaAdelante();
				llantas.configurarAavance(pwmTemporal + dalePWM);
				llantas.avanza();
				//Monitoreo
				Serial.println(pwmTemporal);
				BT1.println(pwmTemporal);
				
			}	//End of primer caracter = w
			else if (BT1.peek() == 'q')	//reducir PWM izq
			{
				pwmTemporal = llantas.retroPWMaAdelante();
				llantas.configurarAavance(pwmTemporal - dalePWM);
				llantas.avanza();
				//Monitoreo
				Serial.println(pwmTemporal);
				BT1.println(pwmTemporal);

			}
			else if (BT1.peek() == 'p')	//aumentar PWM der
			{
				pwmTemporal = llantas.retroPWMbAdelante();
				llantas.configurarBavance(pwmTemporal + dalePWM);
				llantas.avanza();
				//Monitoreo
				Serial.println(pwmTemporal);
				BT1.println(pwmTemporal);

			}
			else if (BT1.peek() == 'o')	//reducir PWM der
			{
				pwmTemporal = llantas.retroPWMbAdelante();
				llantas.configurarBavance(pwmTemporal - dalePWM);
				llantas.avanza();
				//Monitoreo
				Serial.println(pwmTemporal);
				BT1.println(pwmTemporal);

			}

			//vaciar
			vaciarTrama();	//Vacia la trama anterior
			delay(5);			// Tiempo de asentamiento


			while (BT1.read() >= 0);	// Vaciado de buffer de BLuetooth por medio de while vacío
		}

		if (Serial.available())
		{
			String S = GetLine();
			BT1.print(S);
			Serial.println("---> " + S);
		}

		break;	//End STA_CONTROLREMOTO
		
		//-----------------------------------------------------------------------------------------------------------------------------------------------------

	//Estado Espera
	case STA_ESPERA:
		//Primera entrada al estado
		if (banderaPrimer == LOW)
		{
			llantas.detiene();
			//Serial.println("Estado ESPERA");
			BT1.println("Estado ESPERA");
			ledBlanco();
			ledOff();

			vaciarTrama();	//Limpia cualquier info en el serial del Bluetooth

			banderaPrimer = HIGH;
		}

		ledGroovy();

		if (BT1.available())
		{
			if (BT1.peek() == 'g')	//Se tomo foto
			{
				//Cambio al Estado Salvar
				State = STA_PROCESANDO;
				cambioDeEstado();

			}	//End of primer caracter = g
		else if (BT1.peek() == 's')	//Ordenes
			{
				llantas.configurarA(BT1.parseInt(), BT1.parseInt(), BT1.parseInt());	// Guarda el primer PWM A
				llantas.configurarB(BT1.parseInt(), BT1.parseInt(), BT1.parseInt());	// Guarda el segundo PWM B
				tiempoAvanzar = BT1.parseInt();		// Guarda el tiempo Avanzar
				tiempoGirarIzq = BT1.parseInt();	// Guarda el tiempo de Girar Izquierda
				tiempoGirarDer = BT1.parseInt();	// Guarda el tiempo de Girar a la Derecha
				tiempoPausado = BT1.parseInt();		// Guarda el tiempo pausado


				vaciarTrama();	//Vacia la trama anterior
				BT1.readBytesUntil('F', instruc, 30);	//Guarda el resto de la trama
				delay(5);			// Tiempo de asentamiento

				//Repite los PWMs
				/*Serial.print("PWM A adelante: ");
				Serial.println(llantas.retroPWMaAdelante());
				Serial.print("PWM B adelante: ");
				Serial.println(llantas.retroPWMbAdelante());
				Serial.print("PWM A izquierda: ");
				Serial.println(llantas.retroPWMaIzquierda());
				Serial.print("PWM B izquierda: ");
				Serial.println(llantas.retroPWMbIzquierda());
				Serial.print("PWM A derecha: ");
				Serial.println(llantas.retroPWMaDerecha());
				Serial.print("PWM B derecha: ");
				Serial.println(llantas.retroPWMbDerecha());
				Serial.print("Tiempo Avanzar: ");
				Serial.println(tiempoAvanzar);
				Serial.print("Tiempo Girar Der: ");
				Serial.println(tiempoGirarDer);
				Serial.print("Tiempo Girara Izq: ");
				Serial.println(tiempoGirarIzq);*/


				//Cambio al Estado Salvar
				State = STA_SALVAR;
				cambioDeEstado();
				
			}	//End of primer caracter =s
		else if (BT1.peek() == 'c')	//Calibracion
			{
				// tiempo adelante
				BT1.println(" �se paso (p) o le falta (f) o ta bueno(b)? ");
				// si se paso-> aumenta el contador avance y repite "tiempo adelante"
				// si le falta-> aumenta el contador avance y repite "tiempo adelante"
				// si esta g�eno -> pasa a calibrar movimiento adelante
								
				// movimiento adelante
				// �se fua a la izq o se fue a la derecha o ta g�eno?
				// si se fue a la izq-> aumenta el contador de izquierda y repite "movimiento adelante"
				// si se fue a la der-> aumenta el contador de derecha y repite "movimiento adelante"
				// si esta g�eno -> pasa a calibrar 

				// tiempo izquierda
				// �se paso o le falta o ta g�eno?
				// si se paso-> aumenta el contador avance y repite "tiempo izquierda"
				// si le falta-> aumenta el contador avance y repite "tiempo izquierda"
				// si esta g�eno -> pasa a calibrar movimiento izquierdo

				// moviemiento izquierdo
				// �se fua pa'rriba o se fue pa'bajo o ta g�eno?
				// si se fue a la izq-> aumenta el contador de izquierda y repite "tiempo izquierdo"
				// si se fue a la der-> aumenta el contador de derecha y repite "tiempo izquierdo"
				// si esta g�eno -> pasa a calibrar tiempo derecha

				// tiempo derecha
				// �se paso o le falta o ta g�eno?
				// si se paso-> aumenta el contador avance
				// si le falta-> aumenta el contador avance
				// si esta g�eno -> pasa a calibrar movimiento

				// movimiento derecho
				// �se fua pa'rriba o se fue pa'bajo o ta g�eno?
				// si se fue a la izq-> aumenta el contador de izquierda y repite "tiempo izquierdo"
				// si se fue a la der-> aumenta el contador de derecha y repite "tiempo izquierdo"
				// si esta g�eno -> pasa a calibrar tiempo derecha
				
				// Imprime multiplicador y parametro de calibracion
			}

			while (BT1.read() >= 0);	// Vaciado de buffer de BLuetooth
		}

		if (Serial.available())
		{
			String S = GetLine();
			BT1.print(S);
			Serial.println("---> " + S);
		}

		
		break;

	// Estado Procesar
	case STA_PROCESANDO:
		//Primera entrada al estado
		if (banderaPrimer == LOW)
		{
			llantas.detiene();
			//Serial.println("Estado PROCESANDO");
			BT1.println("Estado PROCESANDO");
			ledBlanco();
			ledOff();

			vaciarTrama();	//Limpia cualquier info en el serial del Bluetooth

			ledTope = 0;			

			banderaPrimer = HIGH;
		}

		if (BT1.available())
		{
			if (BT1.peek() == 's')	//Ordenes
			{

				llantas.configurarA(BT1.parseInt(), BT1.parseInt(), BT1.parseInt());	// Guarda el primer PWM A
				llantas.configurarB(BT1.parseInt(), BT1.parseInt(), BT1.parseInt());	// Guarda el segundo PWM B
				tiempoAvanzar = BT1.parseInt();		// Guarda el tiempo Avanzar
				tiempoGirarIzq = BT1.parseInt();	// Guarda el tiempo de Girar Izquierda
				tiempoGirarDer = BT1.parseInt();	// Guarda el tiempo de Girar a la Derecha
				tiempoPausado = BT1.parseInt();		// Guarda el tiempo pausado


				vaciarTrama();	//Vacia la trama anterior
				BT1.readBytesUntil('F', instruc, 30);	//Guarda el resto de la trama
				delay(5);			// Tiempo de asentamiento

				//Repite los PWMs
				/*Serial.print("PWM A adelante: ");
				Serial.println(llantas.retroPWMaAdelante());
				Serial.print("PWM B adelante: ");
				Serial.println(llantas.retroPWMbAdelante());
				Serial.print("PWM A izquierda: ");
				Serial.println(llantas.retroPWMaIzquierda());
				Serial.print("PWM B izquierda: ");
				Serial.println(llantas.retroPWMbIzquierda());
				Serial.print("PWM A derecha: ");
				Serial.println(llantas.retroPWMaDerecha());
				Serial.print("PWM B derecha: ");
				Serial.println(llantas.retroPWMbDerecha());
				Serial.print("Tiempo Avanzar: ");
				Serial.println(tiempoAvanzar);
				Serial.print("Tiempo Girar Der: ");
				Serial.println(tiempoGirarDer);
				Serial.print("Tiempo Girara Izq: ");
				Serial.println(tiempoGirarIzq);*/


				//Cambio al Estado Salvar
				State = STA_SALVAR;
				cambioDeEstado();

			}	//End of primer caracter =s					

			while (BT1.read() >= 0);	// Vaciado de buffer de BLuetooth
		}


		if (Serial.available())
		{
			String S = GetLine();
			BT1.print(S);
			Serial.println("---> " + S);
		}
  
		if (millis() - tiempo >= 500) //tiempo de loading
		{
			// LEDs de carga de coche
			ledCarga(ledTope);
			
			//reinicia el tiempo
			tiempo = millis();
			
			//aumenta el ledTope
			ledTope++;

			if (ledTope > LED_COUNT)
			{
				ledTope = 0;
			}
		}

		
		break;


	//Estado Salvar
	case STA_SALVAR:
		//Primera entrada al estado
		if (banderaPrimer == LOW){
			//Serial.println("Estado Salvar");
			BT1.println("Estado Salvar");			
			banderaPrimer = HIGH;
		}


		//Cambio al Estado Decidir
		State = STA_DECIDIR;
		contadorInstrucciones = -1;	//Reinicia contador de instrucciones
		cambioDeEstado();
		
		break;

	//Estado Decidir
	case STA_DECIDIR:
		if (banderaPrimer == LOW){
			Serial.println("Estado Decidir");
			BT1.println("Estado Decidir");
			contadorInstrucciones++;	//Avanza un espacio en el lector
			
			banderaPrimer = HIGH;
		}

		//Cambio al Estado Adelante
		if (instruc[contadorInstrucciones] == 'a')
		{
			State = STA_ADELANTE;
			cambioDeEstado();
		}
		//Cambio al Estado Adelante Repetir
		else if (instruc[contadorInstrucciones] == 'A')
		{
			State = STA_ADELANTEREP;
			cambioDeEstado();
		}
		//Cambio al Estado Izquierda
		else if (instruc[contadorInstrucciones] == 'i')
		{
			State = STA_IZQUIERDA;
			cambioDeEstado();
		}
		//Cambio al Estado Izquierda Repetir
		else if (instruc[contadorInstrucciones] == 'I')
		{
			State = STA_IZQUIERDAREP;
			cambioDeEstado();
		}
		//Cambio al Estado Derecha
		else if (instruc[contadorInstrucciones] == 'd')
		{
			State = STA_DERECHA;
			cambioDeEstado();
		}
		//Cambio al Estado Derecha Repetir
		else if (instruc[contadorInstrucciones] == 'D')
		{
			State = STA_DERECHAREP;
			cambioDeEstado();
		}
		//Cambio al Estado Espera
		else if (instruc[contadorInstrucciones] == 'F')
		{
			State = STA_ESPERA;
			cambioDeEstado();
		}
		//Cambia al Estado Error
		else
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		break;

	//Estado Adelante
	case STA_ADELANTE:
		if (banderaPrimer == LOW){
			beep();

			//Acelera hasta la vel objetivo
			llantas.aceleraAdelante();	//Acelera poco a poco
			llantas.avanza();	// Acciona las llantas hacia adelante
					
			Serial.println("Estado Adelante");
			BT1.println("Estado Adelante");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledAdelante();  //Cambia los LEDs al verde hacia adelante

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoAvanzar)
		{
			//Desacelera hasta frenar el coche
			llantas.desaceleraAdelante();
			delay(tiempoPausado);

			//Cambia de nuevo al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	//Estado AdelanteRepetir
	case STA_ADELANTEREP:
		if (banderaPrimer == LOW){
			beep();
			
			//Acelera hasta la vel objetivo
			llantas.aceleraAdelante();	//Acelera poco a poco
			llantas.avanza();	// Acciona las llantas hacia adelante

			Serial.println("Estado Adelante Repetir");
			BT1.println("Estado Adelante Repetir");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledRepAdelante();  //Cambia los LEDs al verde hacia adelante centro repetir

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoAvanzar)
		{
			//Desacelera hasta frenar el coche
			llantas.desaceleraAdelante();
			delay(tiempoPausado);

			//Cambia de nuevo al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	//Estado Izquierda Repetir
	case STA_IZQUIERDA:
		if (banderaPrimer == LOW){
			beep();
			llantas.aceleraIzquierda();//Acelera poco a poco
			llantas.girarIzq();	// Acciona las llantas para girar a la Izq
			Serial.println("Estado Izquierda");
			BT1.println("Estado Izquierda");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledIzq();  //Cambia los LEDs al amarillo hacia izquierda

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoGirarIzq)
		{
			// Detiene el coche
			llantas.detiene();
			delay(tiempoPausado);

			// Cambia al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	//Estado Izquierda Repetir
	case STA_IZQUIERDAREP:
		if (banderaPrimer == LOW){
			beep();			
			llantas.aceleraIzquierda();//Acelera poco a poco a la izquierda
			llantas.girarIzq();	// Acciona las llantas para girar a la Izq
			Serial.println("Estado Izquierda Repetir");
			BT1.println("Estado Izquierda Repetir");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledRepIzq();  //Cambia los LEDs al amarillo hacia izquierda con centro de repetir

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoGirarIzq)
		{
			// Detiene el coche
			llantas.detiene();
			delay(tiempoPausado);

			// Cambia al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	//Estado Derecha
	case STA_DERECHA:
		if (banderaPrimer == LOW){
			beep();
			llantas.aceleraDerecha();	//Acelera poco a poco a la Derecha
			llantas.girarDer();	// Acciona las llantas para girar a la Der
			Serial.println("Estado Derecha");
			BT1.println("Estado Derecha");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledDer();  //Cambia los LEDs al naranja hacia derecha

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoGirarDer)
		{
			// Detiene el coche
			llantas.detiene();
			delay(tiempoPausado);

			// Cambia al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	// Estado Derecha Repetir
	case STA_DERECHAREP:
		if (banderaPrimer == LOW){
			beep();
			llantas.aceleraDerecha();	//Acelera poco a poco a la Derecha 
		llantas.girarDer();	// Acciona las llantas para girar a la Der
			Serial.println("Estado Derecha repetir");
			BT1.println("Estado Derecha repetir");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledRepDer();  //Cambia los LEDs al naranja hacia derecha centro repetir

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Decidir
		if (millis() - tiempo >= tiempoGirarDer)
		{
			// Detiene el coche
			llantas.detiene();
			delay(tiempoPausado);

			// Cambia al estado decidir
			State = STA_DECIDIR;
			cambioDeEstado();
		}
		
		//Cambio al Estado Error
		else if (digitalRead(SEN_CHOQUE) == ACTIVADO)
		{
			State = STA_ERROR;
			cambioDeEstado();
		}
		//Cambio al Estado Levantado
		else if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}
		
		break;

	//Estado Error
	case STA_ERROR:
		if (banderaPrimer == LOW)
		{
			longBeep();
			llantas.detiene();	//!< Detiene el auto
			Serial.println("Estado Error");
			BT1.println("Estado Error");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledError();  //Cambia los LEDs al rojo de error

			banderaPrimer = HIGH;
		}

		
		//Cambio al Estado Levantado
		if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}

		break;

	//Estado Levantado
	case STA_LEVANTADO:
		if (banderaPrimer == LOW)
		{
			llantas.detiene();	//!< Detiene el auto
			Serial.println("Estado Levantado");
			BT1.println("Estado Levantado");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			//ledOff();	//Apaga los leds
			ledAzul();

			banderaPrimer = HIGH;
		}

		delay(500); //Tiempo para asegurar que se levanto bien

		//Cambio al Estado Espera
		if (digitalRead(SEN_PISO) != ACTIVADO)
		{
			State = STA_ESPERA;
			cambioDeEstado();
		}

		break;

	//************Estados de calibracion
	case STA_CALIBADELTIEMPO:
		if (banderaPrimer == LOW)
		{
			llantas.detiene();	//!< Detiene el auto
			Serial.println("Estado Calibracion");
			BT1.println("Estado Calibracion");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledOff();	//Apaga los leds
			
			banderaPrimer = HIGH;
		}



		break;


	default:
		if (banderaPrimer == LOW)
		{
			llantas.detiene();	//!< Detiene el auto
			Serial.println("Estado Default");
			BT1.println("Estado Default");
			ledBlanco();	//Color para asegurar que el siguiente se despliegue correctamente
			ledError();  //Cambia los LEDs al rojo de error

			banderaPrimer = HIGH;
		}

		//Cambio al Estado Levantado
		if (digitalRead(SEN_PISO) == ACTIVADO)
		{
			State = STA_LEVANTADO;
			cambioDeEstado();
		}

		break;
	}

	
}

// ********************FUNCTIONS**********************************************
// LEDS*************

/* Encendido de Loading*/
void ledCarga(uint16_t ledInicial)
{
	// Apagar
	ledOff();

	//encender LEDs hasya el m�ximo
	for (uint16_t i = 0; i < ledInicial; i++) {
		luces.setPixelColor(i, luces.Color(0, 255, 0));
		luces.show();
	}

} //End of ledPulso

/** Encendido de LEDs en blanco */
void ledBlanco()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, 0xFFFFFF);
	}
	luces.show();
	delay(5);
} //End of ledBlanco




/** Encendido de LEDs color adelante */
void ledAdelante()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, FRENTE);
	}
	luces.show();
	delay(5);
} //End of ledAdeladnte

/** Encendido de LEDs color adelante y centro repite*/
void ledRepAdelante()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, FRENTE);
	}
	luces.setPixelColor(2, REPITE);  //Pone el LED central en morado
	luces.show();
	delay(5);
} // End of ledRepAdelante

/** Encendido de LEDs color izquierda */
void ledIzq()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, IZQ);
	}
	luces.show();
	delay(5);
} //End of ledIzq

/** Encendido de LEDs color izquierda y centro repite*/
void ledRepIzq()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, IZQ);
	}
	luces.setPixelColor(2, REPITE);  //Pone el LED central en morado
	luces.show();
	delay(5);
} // End of ledRepIzq

/** Encendido de LEDs color derecha */
void ledDer()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, DER);
	}
	luces.show();
	delay(10);
} //End of ledDer

/** Encendido de LEDs color derecha y centro repite*/
void ledRepDer()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, DER);
	}
	luces.setPixelColor(2, REPITE);  //Pone el LED central en morado
	luces.show();
	delay(5);
} //End of ledRepDer

/** Encendido de LEDs color error */
void ledError()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, CHIN);
	}
	luces.show();
	delay(5);
} //End of ledError

/** Encendido de LEDs color azul */
void ledAzul()
{
	for (int i = 0; i < LED_COUNT; i++) {
		luces.setPixelColor(i, AZUL);
	}
	luces.show();
	delay(5);
} //End of ledError

/** Muestra un arcoiris */
void ledGroovy()
{
	static int col_count; //exp
	rainbow(col_count);
	delay(10);
	
	if (millis() - tiempo >= BARRIDO) {
		col_count++;
		if (col_count >= LED_COUNT * 10){
			col_count = 0;
		}
		tiempo = millis();

	}

} //End of ledGroovy

/** Apagado de LEDs */
void ledOff()
{
	for (int i = 0; i<LED_COUNT; i++)
	{
		luces.setPixelColor(i, 0);
	}
	luces.show();
	delay(5);
} //End of ledOff

/** Muestra un arcoiris en todos los LEDs
* \param startPosition posiscion inicial del arcoiris
*/
void rainbow(byte startPosition)
{
	// Need to scale our rainbow. We want a variety of colors, even if there
	// are just 10 or so pixels.
	int rainbowScale = 192 / LED_COUNT;

	// Next we setup each pixel with the right color
	for (int i = 0; i < LED_COUNT; i++)
	{
		// There are 192 total colors we can get out of the rainbowOrder function.
		// It'll return a color between red->orange->green->...->violet for 0-191.
		luces.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
	}
	// Finally, actually turn the LEDs on:
	luces.show();	
}

/* Input a value 0 to 191 to get a color value.
* The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
*  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
* \return color value
*/
uint32_t rainbowOrder(byte position)
{
	// 6 total zones of color change:
	if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
	{
		return luces.Color(0xFF, position * 8, 0);
	}
	else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
	{
		position -= 31;
		return luces.Color(0xFF - position * 8, 0xFF, 0);
	}
	else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
	{
		position -= 63;
		return luces.Color(0, 0xFF, position * 8);
	}
	else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
	{
		position -= 95;
		return luces.Color(0, 0xFF - position * 8, 0xFF);
	}
	else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
	{
		position -= 127;
		return luces.Color(position * 8, 0, 0xFF);
	}
	else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
	{
		position -= 159;
		return luces.Color(0xFF, 0x00, 0xFF - position * 8);
	}
}

// ******* MANEJO DE FLUJOS ************

String GetLine()
{
	String S = "";
	if (Serial.available())
	{
		char c = Serial.read();;
		while (c != 'r')            //Hasta que el caracter sea intro
		{
			S = S + c;
			delay(25);
			c = Serial.read();

		}
		return(S);
	}
}

void vaciarTrama()
{
	for (int i = 0; i < 31; i++)
	{
		instruc[i] = 'F';
	}
}

int longitudTrama()
{	
	int contador = 0;
	int i = 0;
	do
	{
		if (instruc[i] != 'F')
		{
			contador++;
			i++;
		}
		else
		{
			i = -1;
		}
	} while (i>=0);

	return contador;
}

// ******* ESTADOS ************

/** Acciones al realizar un cambio de estado
* \param Estado Estado al que se desea cambiar
*/
void cambioDeEstado()
{
	banderaPrimer = LOW;	//Reinicia la bandera de primera entrada al estado

	tiempo = millis();	//Reinicia el tiempo en el nuevo estado
}

// ******* SONIDOS ************

void beep()
{
	analogWrite(PIN_BUZZ, 50);
	delay(25);                 // Espera
	analogWrite(PIN_BUZZ, 0);            // Apaga
	delay(25);                 // Espera
}

void longBeep()
{
	analogWrite(PIN_BUZZ, 100);
	delay(250);                 // Espera
	analogWrite(PIN_BUZZ, 0);            // Apaga
	delay(25);                 // Espera
}
