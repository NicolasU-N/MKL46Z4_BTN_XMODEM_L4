/*
 * uart_ringBuffer.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef UART_RING_BUFFER_H_
#define UART_RING_BUFFER_H_

#include <buffer.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>

#include "Tiempo.h"
#include "KL46z.h"
#include "Varios.h"
#include "showDisplay.h"

//-------------------- DEBUG
#include "fsl_uart.h"
#include "pin_mux.h"

#define DEMO_UART UART1
#define DEMO_UART_CLKSRC SYS_CLK
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(BUS_CLK)
//-------------------- DEBUG

void clock_config();

void uart0_init(unsigned int uart0clk, unsigned int baud_rate);

void uart_send_byte(uint8_t data_input);

void uart_send_string(uint8_t *str_data);

char uart_receive_byte();

void UART0_IRQHandler();

void myprintf(char*, ...);

void myprintf_uart1(char*, ...);

void UART1_Write(uint8_t data);

#endif /* UART_RING_BUFFER_H_ */
