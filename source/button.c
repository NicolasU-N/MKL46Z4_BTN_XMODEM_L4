/*
 * button.c
 *
 *  Created on: 12/05/2022
 *      Author: Nicolas
 */
#include "button.h"

extern Tm_Control c_tiempo;

void initBtns() {
	/* Initialize and enable BTN */
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // Enable clock  to PORT C

	/* Initialize and enable SW */
	PORTC->PCR[3] |= PORT_PCR_MUX(1);
	PORTC->PCR[12] |= PORT_PCR_MUX(1);

	/* Enable internal pullup resistor */
	PORTC->PCR[3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC->PCR[12] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	PTC->PDDR &= ~((1u << 3) | (1u << 12));

	Tm_Inicie_periodo(&c_tiempo, N_PER_READ_BTN, 4); // Iniciar periodo de lectura pulsadores
}

char read_button_plus() {
	static char count = 0;	// Contador de número de estados iguales
	static char button_state = 1;	// Realiza un seguimiento del estado actual
	char current_state = (((PTC->PDIR) >> BTN_PLUS) & 0x01U);	// Comprobar si el botón está alto o bajo

	if (current_state != button_state) {
		count++; // El estado del botón está a punto de cambiarse, aumentar el contador
		if (count >= 4) {
			button_state = current_state; // El botón no ha rebotado por cuatro veces, cambia de estado
			// Si se presionó el botón (no se soltó)
			if (current_state != 0) {
				count = 0;
				return SI;
			}
		}

	} else {
		count = 0; // Reiniciar contador
	}
	return NO;
}

char read_button_minus() {
	static char count = 0;	// Contador de número de estados iguales
	static char button_state = 1;	// Realiza un seguimiento del estado actual
	char current_state = (((PTC->PDIR) >> BTN_MINUS) & 0x01U); // Comprobar si el botón está alto o bajo

	if (current_state != button_state) {
		count++; // El estado del botón está a punto de cambiarse, aumentar el contador
		if (count >= 4) {
			button_state = current_state; // El botón no ha rebotado por cuatro veces, cambia de estado
			// Si se presionó el botón (no se soltó)
			if (current_state != 0) {
				count = 0;
				return SI;
			}
		}
	} else {
		count = 0; // Reiniciar contador
	}
	return NO;
}

