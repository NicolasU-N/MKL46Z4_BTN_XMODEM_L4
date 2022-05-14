/*
 * button.h
 *
 *  Created on: 12/05/2022
 *      Author: Nicolas
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "MKL46Z4.h"
#include "Tiempo.h"
#include "Varios.h"

#define BTN_MINUS 3

#define BTN_PLUS 12

void initBtns();

char read_button_plus();

char read_button_minus();

#endif /* BUTTON_H_ */
