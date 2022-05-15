/* Definiciones generales */
#ifndef VARIOS_H_
#define VARIOS_H_

/* Valores booleanos */
#define SI	1
#define NO	0

/* TIMERS */
#define NUM_PER		5
#define NUM_TO		3
//#define NUM_PWMS	3

#define N_PER_MUX		0 // 120Hz
//#define N_PER_SEND_ACK	1
#define N_PER_1HZ		1 //1Hz
#define N_PER_2HZ		2 //2Hz
#define N_PER_SHOW_D	3 //2Hz
#define N_PER_READ_BTN	4

#define N_TO_PKT_INC	0
#define N_TO_ERROR		1
#define N_TO_ERROR_SH	2
// XON - XOFF
//#define XON	0x11
//#define XOFF	0x13

/* BUFFER SIZE */
#define BUFF_SIZE_RX	4
#define BUFF_SIZE_XMD	132
#define BUFF_SIZE_DIS	128

#endif

