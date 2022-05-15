/*
 * show_display.c
 *
 *  Created on: 5/04/2022
 *      Author: Nicolas
 */
#include "showDisplay.h"

extern Tm_Control c_tiempo;
extern buffer_struct *pBufferDisplay;

char state_display;
uint8_t character;

Tm_Contador N_SHOW = 1;

void show_data_init() {
	// Initialize display
	display_init();
	state_display = STBY_MODE;
	pBufferDisplay = buffer_init(BUFF_SIZE_DIS); // 32 64 200
	Tm_Inicie_periodo(&c_tiempo, N_PER_MUX, 1); //8.333 ms = 120hz
	Tm_Inicie_periodo(&c_tiempo, N_PER_SHOW_D, N_SHOW); // sacar datos del buffer (hz,ldval) 120hz->1, 50hz->2 ,40hz->3, 30hz->4, 24hz->5
}

void show_data() {
	if (!buffer_is_empty(pBufferDisplay)) { // si hay datos en el buffer
		buffer_get_data(pBufferDisplay, &character);
		//myprintf_uart1(" %d\r\n", character); //CO=%d
	}
}

void mux_display() {
	switch (state_display) {
	case NORMAL_MODE:
		display_scan(character);
		break;
	case STBY_MODE:
		set_display_stby();
		break;
	case OFF_MODE:
		set_display_off();
		break;
	}
}

void set_blink_1hz() {
	Tm_Termine_periodo(&c_tiempo, N_PER_2HZ);
	Tm_Inicie_periodo(&c_tiempo, N_PER_1HZ, 60); // 500msec 1HZ 50%

}

void set_blink_2hz() {
	Tm_Termine_periodo(&c_tiempo, N_PER_1HZ);
	Tm_Inicie_periodo(&c_tiempo, N_PER_2HZ, 30); // 250msec 2HZ 50%
}

void set_normal_mode() {
	state_display = NORMAL_MODE;
	//Terminar periodos
	Tm_Termine_periodo(&c_tiempo, N_PER_1HZ);
	Tm_Termine_periodo(&c_tiempo, N_PER_2HZ);
}

void set_stby_mode() {
	state_display = STBY_MODE;
	//Terminar periodos
	Tm_Termine_periodo(&c_tiempo, N_PER_1HZ);
	Tm_Termine_periodo(&c_tiempo, N_PER_2HZ);
}

void toggle_state_display() {
	if (state_display != OFF_MODE) {
		state_display = OFF_MODE;
	} else {
		state_display = NORMAL_MODE;
	}
}

