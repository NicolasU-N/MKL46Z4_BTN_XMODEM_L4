/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL46Z4_BTN_XMODEM.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "pit.h"
#include "showDisplay.h"
#include "uart_ringBuffer.h"
#include "xmodem.h"

//---------------BUFFER RX-----------------
buffer_struct *pBufferRx;
//--------------------------------------

//---------------DISPLAY------------------
buffer_struct *pBufferDisplay;
//--------------------------------------

//---------------XMODEM-----------------

//--------------------------------------

//---------------TIMER------------------
Tm_Control c_tiempo;
static Tm_Periodo periodos[NUM_PER];
static Tm_Timeout timeouts[NUM_TO];

static char atender_timer(char atienda);
//-------------------------------------

void PIT_IRQHandler(void) {
	/* Clear interrupt flag.*/
	if (atender_timer(NO)) {
		Tm_Procese(&c_tiempo);
	}
}

/*
 * @brief   Application entry point.
 */
int main(void) {
	clock_config();

	/* ============ UART0 ================ */
	uart0_init(SystemCoreClock, 2400); //115200

	pit_init_ch0(0x30D37); //0x30D37 -> 8.333ms || 0x124F7 -> 3.125ms || 0x752F-> 1.25ms || 0xB71AFF -> 500ms

	Tm_Inicio(&c_tiempo, periodos, NUM_PER, timeouts, NUM_TO, &atender_timer); //, pwms, NUM_PWMS

	xmodem_init();

	show_data_init();

	//myprintf("Hello World\r\n");
	uart_send_byte(NAK); //NACK 21

	while (1) {

		//if (atender_timer(NO)) {
		//	Tm_Procese(&c_tiempo);
		//}

		if (Tm_Hubo_periodo(&c_tiempo, N_PER_MUX)) { // 120hz multiplexacion y sacar datos de buffer de display
			Tm_Baje_periodo(&c_tiempo, N_PER_MUX);
			show_data();
		}

		//XMODEM
		procesar_xmoden();

		//BLINKS
		if (Tm_Hubo_periodo(&c_tiempo, N_PER_1HZ)) { // blink 1HZ
			Tm_Baje_periodo(&c_tiempo, N_PER_1HZ);
			toggle_state_display();
		}

		if (Tm_Hubo_periodo(&c_tiempo, N_PER_2HZ)) { // blink 2HZ
			Tm_Baje_periodo(&c_tiempo, N_PER_2HZ);
			toggle_state_display();
		}

	}
	return 0;
}

/* Rutina de verificación y atención del HW del timer */
static char atender_timer(char atienda) {
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		if (atienda)
			/* Clear interrupt flag.*/
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		return SI;
	} else {
		return NO;
	}
}
