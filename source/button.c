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
	// Counter for number of equal states
	static char count = 0;

	// Keeps track of current (debounced) state
	static char button_state = 1;

	// Check if button is high or low for the moment
	char current_state = (((PTC->PDIR) >> BTN_PLUS) & 0x01U);

	if (current_state != button_state) {

		// Button state is about to be changed, increase counter
		count++;
		if (count >= 4) {
			// The button have not bounced for four checks, change state
			button_state = current_state;
			// If the button was pressed (not released), tell main so
			if (current_state != 0) {
				count = 0;
				return SI;
			}
		}

	} else {
		// Reset counter
		count = 0;
	}
	return NO;
}


char read_button_minus() {
	// Counter for number of equal states
	static char count = 0;

	// Keeps track of current (debounced) state
	static char button_state = 1;

	// Check if button is high or low for the moment
	char current_state = (((PTC->PDIR) >> BTN_MINUS) & 0x01U);

	if (current_state != button_state) {

		// Button state is about to be changed, increase counter
		count++;
		if (count >= 4) {
			// The button have not bounced for four checks, change state
			button_state = current_state;
			// If the button was pressed (not released), tell main so
			if (current_state != 0) {
				count = 0;
				return SI;
			}
		}

	} else {
		// Reset counter
		count = 0;
	}
	return NO;
}

