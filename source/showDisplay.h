/*
 * showDisplay.h
 *
 *  Created on: 5/04/2022
 *      Author: Nicolas
 */

#ifndef SHOWDISPLAY_H_
#define SHOWDISPLAY_H_

#include "Varios.h"
#include "display.h"
#include "Tiempo.h"
#include "buffer.h"

#define NORMAL_MODE 0
#define STBY_MODE	1
#define OFF_MODE	2

void show_data_init();

void show_data();

void mux_display();

void set_blink_1hz();

void set_blink_2hz();

void set_normal_mode();

void set_stby_mode();

void toggle_state_display();

#endif /* SHOWDISPLAY_H_ */
