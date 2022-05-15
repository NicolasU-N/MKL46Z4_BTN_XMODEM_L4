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
#include "button.h"

//---------------BUFFER RX-----------------
buffer_struct *pBufferRx;
//--------------------------------------

//---------------DISPLAY------------------
buffer_struct *pBufferDisplay;

extern Tm_Contador N_SHOW;
//--------------------------------------

//---------------XMODEM-----------------
buffer_struct *pBufferXmodem;
//--------------------------------------

//---------------TIMER------------------
Tm_Control c_tiempo;
static Tm_Periodo periodos[NUM_PER];
static Tm_Timeout timeouts[NUM_TO];
//-------------------------------------

void PIT_IRQHandler(void) {
	Tm_Procese(&c_tiempo);
	PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; //Clear interrupt flag.
}

/*
 * @brief   Application entry point.
 */
int main(void) {
	clock_config();

	/* ============ UART0 ================ */
	uart0_init(SystemCoreClock, 2400); //115200

	pit_init_ch0(0x30D37); //0x30D37 -> 8.333ms || 0x124F7 -> 3.125ms || 0x752F-> 1.25ms || 0xB71AFF -> 500ms

	Tm_Inicio(&c_tiempo, periodos, NUM_PER, timeouts, NUM_TO); //, pwms, NUM_PWMS , &atender_timer

	xmodem_init();

	show_data_init();

	initBtns();

	//uart_send_byte(NAK);
	//myprintf_uart1("NAKini\r\n");

	while (1) {

		if (Tm_Hubo_periodo(&c_tiempo, N_PER_MUX)) { // 120hz
			Tm_Baje_periodo(&c_tiempo, N_PER_MUX);
			mux_display();
		}

		if (Tm_Hubo_periodo(&c_tiempo, N_PER_SHOW_D)) { // sacar datos de buffer
			Tm_Baje_periodo(&c_tiempo, N_PER_SHOW_D);
			show_data();
		}
		//-----------------------------------------------------------------------------

		//Read Btn
		if (Tm_Hubo_periodo(&c_tiempo, N_PER_READ_BTN)) { // sacar datos de buffer
			Tm_Baje_periodo(&c_tiempo, N_PER_READ_BTN);
			if (read_button_minus()) {
				if (N_SHOW != 1)
					N_SHOW--;
				myprintf_uart1("- %d\r\n", N_SHOW); //debug
				Tm_Inicie_periodo(&c_tiempo, N_PER_SHOW_D, N_SHOW);
			}

			if (read_button_plus()) {
				if (N_SHOW <= 4)
					N_SHOW++;
				myprintf_uart1("+ %d\r\n", N_SHOW); //debug
				Tm_Inicie_periodo(&c_tiempo, N_PER_SHOW_D, N_SHOW);
			}
		}

		//-----------------------------------------------------------------------------
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
