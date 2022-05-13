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

//----------------- VALIDAR ERRORES ----------------------
char flag_eot = SI;
char flagvalidate = SI;

uint8_t chrx;
uint8_t lastpakectNo = 0;		// idx del ultimo paquete

uint8_t pakectNo = 0;			// idx del paquete
uint8_t lastpakectNoComp = 0;	// idx del paquete complemento a 1

char num_intentos_ACK = 0;
uint8_t sum_checksum;			// sumatoria modulo del paqueete
//--------------------------------------------------------

char state_xmodem = RECIBIR;

void xmodem_init() {
	Tm_Inicie_periodo(&c_tiempo, N_PER_MUX, 1); //8.333 ms
	Tm_Inicie_periodo(&c_tiempo, N_PER_SEND_ACK, 120); // 1seg
}

void procesar_xmoden() {
	switch (state_xmodem) {
	case RECIBIR:

		if (buffer_is_empty(pBufferRx)) {
			if (Tm_Hubo_periodo(&c_tiempo, N_PER_SEND_ACK)) { // 1seg envia NAK o ACK
				Tm_Baje_periodo(&c_tiempo, N_PER_SEND_ACK);
				if (flag_eot) {	//Se termino la transmision
					uart_send_byte(NAK);
				} else {		// No ha finalizado la transmision

					switch (num_intentos_ACK) {
					case 1:
						set_blink_1hz();
						break;
					case 20:
						set_blink_2hz();
						break;
					}

					if (!flagvalidate) {
						myprintf_uart1("NAK %d\r\n", num_intentos_ACK);
						buffer_reset(pBufferRx);		//reset buffer
						uart_send_byte(NAK);
					} else {

						//buffer_reset(pBufferRx);		//reset buffer
						if (Tm_Hubo_timeout(&c_tiempo, N_TO_PKT_INC)) {
							//Tm_Termine_timeout(&c_tiempo, N_TO_PKT_INC);
							buffer_reset(pBufferRx);		//reset buffer
							uart_send_byte(NAK);
							myprintf_uart1("NAK TO %d\r\n", num_intentos_ACK);
						} else {
							buffer_reset(pBufferRx);		//reset buffer
							uart_send_byte(ACK);
							myprintf_uart1("ACK %d\r\n", num_intentos_ACK);
						}

					}

					if (num_intentos_ACK < 20)
						num_intentos_ACK++;

				}
			}
		}

		if (buffer_is_full(pBufferRx)) {
			//DEBUG BUFFER
			/*
			 uart_send_byte(CAN);
			 uart_send_byte(CAN);

			 myprintf_uart1("\r\n");

			 while (buffer_get_data(pBufferRx, &chrx)) //&& pBufferRx->count >= 1
			 {
			 //uart_send_byte(&chtx, 1);
			 //PRINTF("%d\r\n", chtx);
			 myprintf_uart1(" %d\r\n", chrx);
			 }
			 */
			//------------------- degub display
			set_normal_mode();
			state_xmodem = VALIDAR;
			flagvalidate = SI;
			flag_eot = NO; //bandera para validar que se termino la transmision
			//reset contador intentos NAK
			sum_checksum = num_intentos_ACK = 0;			//pakectNo = lastpakectNo = lastpakectNoComp

		} else {
			//Si no esta lleno, valide que el primer caracter sea un EOT sino es un paquete incompleto

			if (buffer_is_empty(pBufferDisplay)) {
				buffer_peek(pBufferRx, &chrx, 0);
				//PRINTF("\r\nVALIDAR %d\r\n", chrx);
				//myprintf("\r\nVALIDAR %d\r\n", chrx);
				//myprintf_uart1("\r\n %d\r\n", chrx);

				if (chrx == 4 || chrx == 24) { // validar si el primer caracter es EOT O CAN
					myprintf_uart1("EOT %d\r\n", chrx);
					Tm_Termine_timeout(&c_tiempo, N_TO_PKT_INC);
					buffer_reset(pBufferRx);			//reset buffer
					chrx = 0;
					state_xmodem = RECIBIR;
					set_stby_mode();
					flag_eot = SI;
					flagvalidate = SI;
					sum_checksum = pakectNo = lastpakectNoComp = lastpakectNo = num_intentos_ACK = 0;
					uart_send_byte(ACK);
				}
			}

			if (Tm_Hubo_timeout(&c_tiempo, N_TO_PKT_INC) && !flag_eot) {
				Tm_Termine_timeout(&c_tiempo, N_TO_PKT_INC);
				myprintf_uart1("BF INCL\r\n");
				buffer_reset(pBufferRx);			//reset buffer
				sum_checksum = num_intentos_ACK = 0;
				flag_eot = NO;
				flagvalidate = NO;
				state_xmodem = RECIBIR;
				chrx = 0;
			}

		}

		break;
	case VALIDAR:
		//--------------------------------------- VALIDAR INICIO DE PAQUETE
		/*
		 buffer_peek(pBufferRx, &chrx, 0);
		 //PRINTF("\r\nVALIDAR %d\r\n", chrx);
		 //myprintf("\r\nVALIDAR %d\r\n", chrx);
		 myprintf_uart1("\r\nVALIDAR %d\r\n", chrx);

		 if (chrx == 4 || chrx == 24) { // validar si el primer caracter es EOT O CAN
		 myprintf_uart1("\r\nEOT%d\r\n", chrx);
		 uart_send_byte(ACK);
		 uart_send_byte(ACK);
		 uart_send_byte(ACK);
		 state_xmodem = RECIBIR;
		 state_display = STBY;
		 flag_eot = SI;
		 flagvalidate = SI;
		 sum_checksum = pakectNo = lastpakectNoComp = lastpakectNo =
		 num_intentos_ACK = 0;
		 break;
		 }
		 */

		//---------------------------------------
		//--------------------------------------- VALIDAR SECUENCIA
		buffer_peek(pBufferRx, &pakectNo, 1);
		buffer_peek(pBufferRx, &lastpakectNoComp, 2);

		/*
		 myprintf_uart1("\r\nVAL PKTN %d - ", pakectNo);
		 myprintf_uart1("VAL LPKTN %d\r\n", lastpakectNoComp);
		 */

		if ((pakectNo == lastpakectNo + 1) && (lastpakectNoComp == 255 - pakectNo)) { // en secuencia Y valida complemento
			//myprintf_uart1("\r\nOK PCKT\r\n");
			lastpakectNo = pakectNo; // Actualizar pakectNo
			flag_eot = NO;
			flagvalidate = SI;

			//myprintf("\r\nPAQUETE OK %d\r\n", chrx);
			//PRINTF("\r\nPAQUETE OK %d\r\n", chrx);

		} else if (pakectNo == lastpakectNo) { //Validar duplicado
			//myprintf_uart1("\r\nDUP PCK=%d COMP=%d\r\n", pakectNo, lastpakectNoComp);
			lastpakectNo = pakectNo; // Actualizar pakectNo
			flag_eot = NO;
			flagvalidate = SI; //ENVIAR ACK
			buffer_reset(pBufferRx);			//reset buffer
			//CAMBIAR ESTADO
			state_xmodem = RECIBIR;
			//break;

			//myprintf("\r\nPAQUETE REPETIDO %d\r\n", chrx);
		} else { //fuera de secuencia
			//myprintf_uart1("\r\nFS PCK=%d COMP=%d\r\n", pakectNo, lastpakectNoComp);
			//myprintf("\r\nERROR PCK=%d COMP=%d\r\n", pakectNo,
			//		lastpakectNoComp);
			flag_eot = NO;
			flagvalidate = NO;
			//num_intentos_ACK++;
			sum_checksum = pakectNo = 0;

			buffer_reset(pBufferRx);			//reset buffer
			//uart_send_byte(NAK); // Enviar NAK y resetear banderas
			state_xmodem = RECIBIR;
			//break;
		}
		//---------------------------------------
		//--------------------------------------- VALIDAR CHECKSUM
		//validar checksum
		//Leer los datos del buffer rx,copiarlos en buffer checksum y sumar cada dato y calcular el %256 en sum_checksum con while

		chrx = 0;
		for (int i = 3; i < BUFF_SIZE_RX - 1; i++) {
			buffer_peek(pBufferRx, &chrx, i);
			sum_checksum += chrx; // sumatoria
		}

		//obtener checksum en chrx
		buffer_peek(pBufferRx, &chrx, BUFF_SIZE_RX - 1);
		if (chrx == sum_checksum % 256) { // Si coincide el checksum

		//myprintf("SCHEK = %d CHS = %d\r\n", sum_checksum,sum_checksum % 256);
		//PRINTF("SCHEK = %d CHS = %d\r\n", sum_checksum, sum_checksum % 256);

			/*
			 myprintf_uart1("SCHEK=%d CHS=%d\r\n", sum_checksum,
			 sum_checksum % 256);
			 */

			flag_eot = NO;
			flagvalidate = SI;
			state_xmodem = MOSTRAR;
			buffer_get_data(pBufferRx, &chrx);
			buffer_get_data(pBufferRx, &chrx);
			buffer_get_data(pBufferRx, &chrx); // Omitir header del paquete
		} else {
			//myprintf_uart1("\r\nERR CSP=%d CSC=%d\r\n", chrx, sum_checksum % 256);
			flag_eot = NO;
			flagvalidate = NO;
			buffer_reset(pBufferRx);		//reset buffer
			sum_checksum = chrx = 0; //pakectNo
			// Enviar NAK, resetear banderas y cambiar a estado recibir
			state_xmodem = RECIBIR;
			//break;
		}

		break;
	case MOSTRAR:
		//copiar buffer de rx en display

		if (buffer_available(pBufferDisplay)) { // hay espacio disponible en el buffer del display
			// copiar al buffer del display
			if (buffer_get_data(pBufferRx, &chrx) && buffer_get_count(pBufferRx) >= 1) {
				//myprintf_uart1("CRX=%d-", chrx);
				buffer_add(pBufferDisplay, chrx);
			} else {
				//myprintf_uart1("BFRXFREE\r\n");
				buffer_reset(pBufferRx);			//reset buffer
				sum_checksum = num_intentos_ACK = 0; //pakectNo =
				state_xmodem = RECIBIR;
				flag_eot = NO;
				flagvalidate = SI;
			}
		}

		break;

	}

}
