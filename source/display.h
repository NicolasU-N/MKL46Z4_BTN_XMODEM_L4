/*
 * display.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stdint.h"
#include "MKL46Z4.h"

#define SEG_A_PIN       17U
#define SEG_B_PIN       16U
#define SEG_C_PIN       15U
#define SEG_D_PIN       14U
#define SEG_E_PIN       6U
#define SEG_F_PIN       7U
#define SEG_G_PIN       13U

#define COM_1_PIN		2U
#define COM_2_PIN		4U
#define COM_3_PIN		6U

//get k bits from p position
//#define GET_LSB(number, k, p) (((1 << k) - 1) & (number >> (p - 1)))

void display_init();

//void display_scan();
void display_scan(uint8_t character);

void set_display_stby();

void set_display_off();

#endif /* DISPLAY_H_ */
