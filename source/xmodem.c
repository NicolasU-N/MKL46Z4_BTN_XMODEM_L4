/*
 * xmodem.c
 *
 *  Created on: 6/04/2022
 *      Author: Nicolas
 */
#include "xmodem.h"

extern Tm_Control c_tiempo;
extern buffer_struct *pBufferRx;
extern buffer_struct *pBufferDisplay;
extern buffer_struct *pBufferXmodem;

//----------------- VALIDAR ERRORES ----------------------
char flag_eot = SI;
char flagvalidate = SI;

uint8_t chrx;
uint8_t chxmd;
uint8_t lastpakectNo = 0;		// idx del ultimo paquete

uint8_t pakectNo = 0;			// idx del paquete
uint8_t lastpakectNoComp = 0;	// idx del paquete complemento a 1

char num_intentos_ACK = 0;
uint8_t sum_checksum;			// sumatoria modulo del paqueete
//--------------------------------------------------------

char state_xmodem = RECIBIR;

void xmodem_init() {
	Tm_Inicie_periodo(&c_tiempo, N_PER_MUX, 1); //8.333 ms
	pBufferXmodem = buffer_init(BUFF_SIZE_XMD); // 132
}

void procesar_xmoden() {
	switch (state_xmodem) {
	case RECIBIR:

		//-------------------------------------------------- SACAR DATOS BUFFER RX
		if (!buffer_is_empty(pBufferRx)) { // si hay datos en el buffer
			buffer_get_data(pBufferRx, &chrx);
			buffer_add(pBufferXmodem, chrx);

			//myprintf_uart1(" %d\r\n", chrx);
			Tm_Inicie_timeout(&c_tiempo, N_TO_PKT_INC, 1200); // 10seg	|| 2 SEG->240 360
			Tm_Inicie_timeout(&c_tiempo, N_TO_ERROR_SH, 240); // 1seg	|| 2 SEG->240 360
			//--------------------------------------------------
		} else {

			if (Tm_Hubo_timeout(&c_tiempo, N_TO_PKT_INC)) {

				Tm_Termine_timeout(&c_tiempo, N_TO_PKT_INC);

				if (flag_eot) {
					uart_send_byte(NAK);
					myprintf_uart1("NAKini\r\n");
				} else {		// No ha finalizado la transmision

					Tm_Inicie_timeout(&c_tiempo, N_TO_PKT_INC, 1200); //2 SEG->240 360

					buffer_reset(pBufferXmodem);	//reset buffer
					uart_send_byte(NAK);

					if (num_intentos_ACK < 3)
						num_intentos_ACK++;

					if (num_intentos_ACK == 2)
						set_blink_2hz();

					myprintf_uart1("NAKto %d\r\n", num_intentos_ACK);
				}
			}

			if (Tm_Hubo_timeout(&c_tiempo, N_TO_ERROR_SH) && flag_eot == NO && buffer_is_empty(pBufferDisplay)) {
				Tm_Termine_timeout(&c_tiempo, N_TO_ERROR_SH);

				buffer_peek(pBufferXmodem, &chxmd, 0);

				myprintf_uart1("1HZ %d\r\n", chxmd);
				set_blink_1hz();

				//--------------------------------------- VALIDAR INICIO DE PAQUETE
				if (chxmd == 4 || chxmd == 24) { // validar si el primer caracter es EOT O CAN
					myprintf_uart1("EOT %d\r\n", chxmd);
					buffer_reset(pBufferXmodem);			//reset buffer
					Tm_Termine_timeout(&c_tiempo, N_TO_PKT_INC);

					chxmd = 0;
					state_xmodem = RECIBIR;
					set_stby_mode();
					flag_eot = SI;
					flagvalidate = SI;
					sum_checksum = pakectNo = lastpakectNoComp = lastpakectNo = num_intentos_ACK = 0;
					uart_send_byte(ACK);
				}
				//---------------------------------------
			}

			if (buffer_is_full(pBufferXmodem)) {
				//DEBUG BUFFER
				/*
				 uart_send_byte(CAN);
				 uart_send_byte(CAN);
				 myprintf_uart1("\r\n");
				 while (buffer_get_data(pBufferXmodem, &chxmd)) //&& pBufferXmodem->count >= 1
				 {
				 myprintf_uart1(" %d\r\n", chxmd);
				 }
				 */
				state_xmodem = VALIDAR;
				flagvalidate = SI;
				flag_eot = NO; //bandera para validar que se termino la transmision
				sum_checksum = num_intentos_ACK = 0; //reset contador intentos NAK //pakectNo = lastpakectNo = lastpakectNoComp
			}

		}

		break;
	case VALIDAR:
		//--------------------------------------- VALIDAR SECUENCIA
		buffer_peek(pBufferXmodem, &pakectNo, 1);
		buffer_peek(pBufferXmodem, &lastpakectNoComp, 2);

		//myprintf_uart1("\r\nVAL PKTN %d - ", pakectNo);
		//myprintf_uart1("VAL LPKTN %d\r\n", lastpakectNoComp);

		if ((pakectNo == lastpakectNo + 1) && (lastpakectNoComp == 255 - pakectNo)) { // en secuencia y complemento valido
			myprintf_uart1("OK PKT\r\n");
			lastpakectNo = pakectNo; // Actualizar pakectNo
			flag_eot = NO;
			flagvalidate = SI;

		} else if (pakectNo == lastpakectNo) { //Validar duplicado
			myprintf_uart1("\r\nDUP PCK=%d COMP=%d\r\n", pakectNo, lastpakectNoComp);
			lastpakectNo = pakectNo; // Actualizar pakectNo
			flag_eot = NO;
			flagvalidate = NO;
			buffer_reset(pBufferXmodem);			//reset buffer
			state_xmodem = RECIBIR;			//CAMBIAR ESTADO
			uart_send_byte(ACK); //ENVIAR ACK

		} else { //fuera de secuencia
			//myprintf_uart1("\r\nFS PCK=%d COMP=%d\r\n", pakectNo, lastpakectNoComp);
			flag_eot = NO;
			flagvalidate = NO;
			sum_checksum = pakectNo = 0;

			buffer_reset(pBufferXmodem); //reset buffer			;
			state_xmodem = RECIBIR;
			uart_send_byte(NAK); // Enviar NAK y resetear banderas

		}
		//---------------------------------------
		//--------------------------------------- VALIDAR CHECKSUM
		//Leer los datos del buffer rx, sumar cada dato y calcular el %256 en sum_checksum
		if (flagvalidate) {
			chxmd = 0;
			for (int i = 3; i < BUFF_SIZE_XMD - 1; i++) {
				buffer_peek(pBufferXmodem, &chxmd, i);
				sum_checksum += chxmd; // sumatoria
			}

			buffer_peek(pBufferXmodem, &chxmd, BUFF_SIZE_XMD - 1); //obtener checksum en chxmd

			// Si coincide el checksum
			if (chxmd == sum_checksum % 256) {
				//myprintf_uart1("SCHEK=%d CHS=%d\r\n", sum_checksum, sum_checksum % 256);
				flag_eot = NO;
				flagvalidate = SI;
				state_xmodem = MOSTRAR;
				buffer_get_data(pBufferXmodem, &chxmd);
				buffer_get_data(pBufferXmodem, &chxmd);
				buffer_get_data(pBufferXmodem, &chxmd); // Omitir header del paquete
				set_normal_mode();

			} else {
				myprintf_uart1("\r\nERR CSP=%d CSC=%d\r\n", chxmd, sum_checksum % 256);
				flag_eot = NO;
				flagvalidate = NO;
				buffer_reset(pBufferXmodem);	//reset buffer
				sum_checksum = chxmd = 0;
				state_xmodem = RECIBIR;
				uart_send_byte(NAK);	// Enviar NAK, resetear banderas y cambiar a estado recibir
			}
		}

		break;
	case MOSTRAR:
		//copiar buffer de rx en display
		if (buffer_available(pBufferDisplay)) { // hay espacio disponible en el buffer del display
			// copiar al buffer del display
			if (buffer_get_data(pBufferXmodem, &chxmd) && buffer_get_count(pBufferXmodem) >= 1) {
				//myprintf_uart1("CRX=%d\r\n", chxmd);
				buffer_add(pBufferDisplay, chxmd);
			} else {
				//myprintf_uart1("BFRXFREE\r\n");
				buffer_reset(pBufferXmodem);	//reset buffer
				sum_checksum = num_intentos_ACK = 0;
				state_xmodem = RECIBIR;
				flag_eot = NO;
				flagvalidate = SI;
				uart_send_byte(ACK);
			}
		}
		break;
	}
}
