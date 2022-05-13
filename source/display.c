/*
 * display.c
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#include "display.h"

char digit = 0;

// codes of symbols
// segments: g-f-e-d-c-b-a
const uint32_t scode[] = { (1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (0 << SEG_G_PIN), // 0b0111111, //0     //0
(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN) | (0 << SEG_G_PIN), // 0b0000110, //1     //1
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (0 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (0 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1011011, //2     //2
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1001111, //3     //3
(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1100110, //4     //4
(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1101101, //5     //5
(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1111101, //6     //6
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN) | (0 << SEG_G_PIN), // 0b0000111, //7     //7
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1111111, //8     //8
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1101111, //9     //9
// 0b00000000, //10    //space
(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (0 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1110111, //10    //A
(0 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1111100, //11    //B
(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (0 << SEG_G_PIN), // 0b0111001, //12    //C
(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (0 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1011110, //13    //D
(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN) | (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1111001, //14    //E
(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN) | (0 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN) | (1 << SEG_G_PIN), // 0b1110001, //15    //F
(0 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN) | (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN) | (1 << SEG_G_PIN), }; // 16	 //-

void display_init() {
	/* Initialize and enable LED */
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // Enable clock  to PORT A
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; // Enable clock  to PORT D

	PORTA->PCR[SEG_A_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_B_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_C_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_D_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_E_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_F_PIN] |= PORT_PCR_MUX(1); // PORT A MUX
	PORTA->PCR[SEG_G_PIN] |= PORT_PCR_MUX(1); // PORT A MUX

	PORTD->PCR[COM_1_PIN] |= PORT_PCR_MUX(1); // PORT D MUX
	PORTD->PCR[COM_2_PIN] |= PORT_PCR_MUX(1); // PORT D MUX
	PORTD->PCR[COM_3_PIN] |= PORT_PCR_MUX(1); // PORT D MUX

	PTA->PDDR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN) | (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN) | (1u << SEG_G_PIN); // Outputs
	PTD->PDDR |= (1u << COM_1_PIN) | (1u << COM_2_PIN) | (1u << COM_3_PIN); // Outputs

	PTA->PCOR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN) | (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN) | (1u << SEG_G_PIN); // OFF leds
	PTD->PCOR |= (1u << COM_1_PIN) | (1u << COM_2_PIN) | (1u << COM_3_PIN); // OFF leds
}

void display_scan(uint8_t character) {
	set_display_off();

	switch (digit) {
	case 0:
		PTD->PSOR |= (1u << COM_1_PIN);
		PTA->PSOR |= scode[((character / 10) / 10) % 10]; //((character / 10) / 10) % 10
		break;
	case 1:
		PTD->PSOR |= (1u << COM_2_PIN);
		PTA->PSOR |= scode[(character / 10) % 10]; //(character / 10) % 10
		break;
	case 2:
		PTD->PSOR |= (1u << COM_3_PIN);
		PTA->PSOR |= scode[character % 10]; // character % 10
		break;
	}

	if (++digit > 2)
		digit = 0;
}

void set_display_stby() {
	set_display_off(); // clear display

	PTA->PSOR |= scode[16];						// "-"
	PTD->PSOR |= (1u << COM_1_PIN) | (1u << COM_2_PIN) | (1u << COM_3_PIN); // ON leds COM
}

void set_display_off() {
	PTA->PCOR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN) | (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN) | (1u << SEG_G_PIN);
	PTD->PCOR |= (1u << COM_1_PIN) | (1u << COM_2_PIN) | (1u << COM_3_PIN); // OFF leds COM
}
