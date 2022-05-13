/*
 * pit.c
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#include "pit.h"

void pit_init_ch0(uint32_t ldval) {
	//Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	//Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis

	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[0].LDVAL = ldval; // every 500ms   V = 0.00625 * 24000000–1    T(seconds) || 0.003125 * 24000000–1 || 0.00125 * 24000000–1 || 0.008333 * 24000000–1
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;	//No chaining of timers
	PIT->CHANNEL[0].TCTRL |= PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;	//enable int

	NVIC_SetPriority(PIT_IRQn, 2);
	NVIC_ClearPendingIRQ(PIT_IRQn); //Clear any pending IRQ from PIT
	NVIC_EnableIRQ(PIT_IRQn); //Enable the PIT interrupt in the NVIC

	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;	//enable timer
}

void pit_init_ch1(uint32_t ldval) {
	//Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	//Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis

	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[1].LDVAL = ldval; // 0x4FFFFF every 500ms   V = 0.5 * 10485760–1    T(seconds) 10485760(Bus Clock)
	PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK;	//No chaining of timers
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; //enable int
	PIT->CHANNEL[1].TCTRL |= PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void clear_flag_ch0() {
	/* Clear interrupt flag.*/
	PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
}

void clear_flag_ch1() {
	/* Clear interrupt flag.*/
	PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
}

char get_flag_ch0() {
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		clear_flag_ch0();
		return SI;
	} else {
		return NO;
	}
}

char get_flag_ch1() {
	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		clear_flag_ch1();
		return SI;
	} else {
		return NO;
	}
}
