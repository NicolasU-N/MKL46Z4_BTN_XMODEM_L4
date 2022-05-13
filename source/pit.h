/*
 * pit.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef PIT_H_
#define PIT_H_

#include "MKL46Z4.h"
#include "Varios.h"

void pit_init_ch0(uint32_t ldval);

void pit_init_ch1(uint32_t ldval);

char get_flag_ch0(); //char

char get_flag_ch1(); //char

void clear_flag_ch0();

void clear_flag_ch1();

#endif /* PIT_H_ */
