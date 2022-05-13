/* Tiempo.h    Declaraciones del modulo de tiempo */
#ifndef TIEMPO_H_
#define TIEMPO_H_

#include "Varios.h"
#include "uart_ringBuffer.h"
//#include "board.h"

/* Tipo de dato para los contadores */
typedef unsigned int Tm_Contador;

/* Tipo de dato para dimensiones de los arreglos dinámicos */
typedef unsigned char Tm_Num;

/* Estructura para definir e impplementar un período */
typedef struct Tm_Periodo Tm_Periodo;
#define TM_PER_F_ACTIVO    0x01		/* El período está activo */
#define TM_PER_F_FC        0x02		/* Hubo fin de conteo en el período */
struct Tm_Periodo {
	unsigned char banderas;
	Tm_Contador contador, /* Contador para calcular el período */
	periodo; /* Valor del período a generar */
};

//-------------------------------- PWM
typedef struct Tm_Pwm Tm_Pwm;
struct Tm_Pwm {
	Tm_Contador contador, /* Contador para calcular el período */
	periodo, /* Valor del período a generar */
	cmp; /* Valor a comparar para generar el duty cycle*/
	//GPIO_Type *puerto;
	Tm_Num num_puerto;
};

//-------------------------------- PWM

/* Tipo de dato para definir e implementar timeouts/retardos */
typedef Tm_Contador Tm_Timeout;

/* Tipo de dato de la rutina utilizada para verificar el estado de fin de conteo del timer (HW) y/o
 bajar la bandera de fin de conteo del timer (HW) */
typedef char Tm_Atender(char atiende);

/* Estructura de control del módulo */
typedef struct Tm_Control Tm_Control;
struct Tm_Control {
	/* Tabla de períodos */
	Tm_Periodo *pp;
	Tm_Num nper;

	/* Tabla de timeouts/retardos */
	Tm_Timeout *tp;
	Tm_Num nto;

	/* Rutina para manejar el timer (HW) */
	Tm_Atender *atender;
};

/* Rutina para iniciar el módulo (su estructura de datos) */
void Tm_Inicio(Tm_Control *tcp, Tm_Periodo *pp, Tm_Num nper, Tm_Timeout *tp, Tm_Num nto, Tm_Atender *atender); //Tm_Pwm *ppwm, Tm_Num npwm,

/* Rutina para procesar el módulo (dentro del loop de polling) */
void Tm_Procese(Tm_Control *tcp);

/* ===== RUTINAS DE INTERFAZ ====== */
/* Configurar un período para que empiece a funcionar */
char Tm_Inicie_periodo(Tm_Control *tcp, Tm_Num num_periodo, Tm_Contador periodo);

/* Desactivar un período para que deje de funcionar */
void Tm_Termine_periodo(Tm_Control *tcp, Tm_Num num_periodo);

/* Verificar si hubo fin de conteo en un periodo */
char Tm_Hubo_periodo(Tm_Control *tcp, Tm_Num num_periodo);

/* Bajar la bandera de conteo en un periodo */
void Tm_Baje_periodo(Tm_Control *tcp, Tm_Num num_periodo);

//----------------------------------------------
/* Configurar un timeout/retardo para que empiece a funcionar */
char Tm_Inicie_timeout(Tm_Control *tcp, Tm_Num num_timeout, Tm_Contador espera);

/* Desactivar un timeout/retardo para que deje de funcionar */
char Tm_Termine_timeout(Tm_Control *tcp, Tm_Num num_timeout);

/* Verificar si hubo timeout (o se completo el retardo) */
char Tm_Hubo_timeout(Tm_Control *tcp, Tm_Num num_timeout);

//----------------------------------------------

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif
