/*
 * uart_ringBuffer.c
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#include "uart_ringBuffer.h"

static uint16_t number;
static uint16_t divisor_check;
static uint16_t divisor;
static int8_t power;
static char send_count;

extern Tm_Control c_tiempo;
extern buffer_struct *pBufferRx;

void clock_config() {
	/*config clock Rx Tx*/
	SIM_SCGC5 |= (1U << SIM_SCGC5_SHIFT(PORTA));
	/*setting low clock 32kHz and FLL 1464 for clock 48MHz*/
	MCG_C4 = (MCG_C4 & 0xE0u) | (1u << MCG_C4_DMX32_SHIFT) | (1u << MCG_C4_DRS_SHIFT);
	SystemCoreClockUpdate();
	/*enable clock*/
	SIM_SCGC4 |= (1u << SIM_SCGC4_UART0_SHIFT);
}

void uart0_init(unsigned int uart0clk, unsigned int baud_rate) {
	/*setting buffer*/
	pBufferRx = buffer_init(BUFF_SIZE_RX); // 32 64 200

	//----------------------------------------------------------------------- DEBUG

	uart_config_t config;
	BOARD_InitPins();

	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200; //BOARD_DEBUG_UART_BAUDRATE
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

	//uint8_t *txbf = (uint8_t*) "HELLO\r\n";
	//UART_WriteBlocking(DEMO_UART, txbf, strlen((char*) txbf));
	myprintf_uart1("HELLO\r\n");

	//----------------------------------------------------------------------- DEBUG

	unsigned int calculated_baud = 0;
	unsigned int baud_diff = 0;
	unsigned int osr_val = 0;
	unsigned int sbr_val = 0;
	unsigned int temp = 0;
	unsigned char i = 0;

	/*config mux GPIO*/
	PORT_PCR(PORTA, RX_PIN) = ((PORT_PCR(PORTA, RX_PIN)) & (~PORT_PCR_MUX_MASK)) | (2U << PORT_PCR_MUX_SHIFT);
	PORT_PCR(PORTA, TX_PIN) = ((PORT_PCR(PORTA, TX_PIN)) & (~PORT_PCR_MUX_MASK)) | (2U << PORT_PCR_MUX_SHIFT);

	/*setting NVIC*/
	NVIC_ICPR |= (1U << ID_INT_UART0);
	NVIC_IPR(3) |= (1U << NVIC_IPR_PRI_N0_SHIFT);
	NVIC_ISER |= (1U << ID_INT_UART0);

	/*******************************************************************/
	/*This field configures the oversampling ratio for the receiver between 4x (00011) and 32x (11111).*/
	/*Calculate the first baud rate using the lowest OSR value possible.*/
	i = 4;
	sbr_val = (unsigned int) (uart0clk / (baud_rate * (i + 1)));
	calculated_baud = (uart0clk / (sbr_val * (i + 1)));

	if (calculated_baud > baud_rate) {
		baud_diff = calculated_baud - baud_rate;
	} else {
		baud_diff = baud_rate - calculated_baud;
	}

	osr_val = i;

	/*Select the best OSR value*/
	for (i = 5; i <= 32; i++) {
		sbr_val = (unsigned int) (uart0clk / (baud_rate * (i + 1)));
		calculated_baud = (uart0clk / (sbr_val * (i + 1)));

		if (calculated_baud > baud_rate) {
			temp = calculated_baud - baud_rate;
		} else {
			temp = baud_rate - calculated_baud;
		}

		if (temp <= baud_diff) {
			baud_diff = temp;
			osr_val = i;
		}
	}

	sbr_val = (unsigned int) (uart0clk / (baud_rate * (osr_val + 1)));

	/*Validate tolerance */
	if (baud_diff >= (baud_rate / 100) * 3) {
		while (1)
			;
	}

	/*disable receive and transmit*/
	UART0_C2 &= (~0x0Cu);

	/*config source clock MCGFLLCLK*/
	SIM_SOPT2 = (SIM_SOPT2 & (~(3u << SIM_SOPT2_UART0SRC_SHIFT))) | (1u << SIM_SOPT2_UART0SRC_SHIFT) | (0u << SIM_SOPT2_PLLFLLSEL_SHIFT);

	/*config pin Tx Rx*/
	SIM_SOPT5 &= ~(1u << SIM_SOPT5_UART0RXSRC_SHIFT | 3u << SIM_SOPT5_UART0TXSRC_SHIFT);

	// Setup OSR value
	UART0_C4 = (UART0_C4 & ~(0x1Fu | 1 << UART0_C4_M10_SHIFT)) | (osr_val & 0x1Fu);

	/* Save off the current value of the uartx_BDH except for the SBR field */
	UART0_BDH = (UART0_BDH & ~(0x1Fu)) | ((sbr_val & 0x1F00u) >> 8);
	UART0_BDH &= ~(1u << UART0_BDH_SBNS_SHIFT);

	UART0_BDL = (unsigned char) (sbr_val & 0xFFu);

	UART0_C1 = 0x00u;
	/*clear NF flag*/
	UART0_S1 |= (1 << 2u);

	/* Enable receiver interrupt receive*/
	UART0_C2 = 0x0Cu | 1 << UART0_C2_RIE_SHIFT;
	//------------------------------------------------------ Wo interrupt
	//UART0_C2 = 0x0Cu;
}

void uart_send_byte(uint8_t data_input) {
	UART0_D = data_input;
	while (!(UART0_S1 & (1 << UART0_S1_TDRE_SHIFT)))
		;
	//while(!(UART0_S1 & (1 << UART0_S1_TC_SHIFT))); //Waiting for transmission to get complete
}

void uart_send_string(uint8_t *str_data) {
	while (*str_data) {
		uart_send_byte(*str_data);
		str_data++;
	}
}

char uart_receive_byte() {
	//while (!(UART0_S1 & (1 << UART0_S1_RDRF_SHIFT)))
	//;

	//return UART0_D;

	if ((UART0_S1 & (1 << UART0_S1_RDRF_SHIFT))) {
		buffer_add(pBufferRx, UART0_D);
		//Tm_Inicie_timeout(&c_tiempo, N_TO_NEW_DATA, 400); // 1600 -> 2 seg||8000 -> 10 SEG
		/*
		 if (!buffer_is_full(pBufferRx)) {
		 //flag_xoff = NO;
		 } else {
		 UART0_D = XOFF; //0x13 XOFF || 0X19 XOFF REALTERM
		 //flag_xoff = SI;
		 }
		 */
		//uart_send_byte(UART0_D);
		//if (buffer_add(pBufferRx, UART0_D)) {
		//uart_send_byte(UART0_D);
		//	return SI;
		//} else {
		//	return NO;
		//}
	}
	return UART0_D;
	//buffer_add(pBufferRx, UART0_D);
}

void UART0_IRQHandler() {
	//uart_send_byte(UART0_D);

	// agregar dato al buffer
	buffer_add(pBufferRx, UART0_D);

}

/*Printf Function*/
void myprintf(char *str, ...) {
	va_list ap;
	va_start(ap, str);
	int i = 0;

	while (str && str[i]) {
		uart_send_byte(str[i]);
		i++;

		if (str[i] == '%') {
			i++;

			switch (str[i]) {
			case 'd':
				number = va_arg(ap, int);
				divisor_check = number;
				divisor = 0;
				power = -1;
				send_count = 0;

				while (divisor_check != 0) {
					divisor_check = divisor_check / 10;
					power++;
				}

				divisor = pow(10, power);

				while (divisor != 0) {
					send_count = number / divisor;
					number = number - send_count * divisor;
					divisor = divisor / 10;
					// Convert to ASCII value
					send_count = send_count + 48;
					uart_send_byte(send_count);
				}
				break;

			case 'c':
				uart_send_byte(va_arg(ap, uint8_t)); //int
				break;

			}
			i++;
		}
	}
	va_end(ap);
}

void myprintf_uart1(char *str, ...) {
	va_list ap;
	va_start(ap, str);
	int i = 0;

	while (str && str[i]) {
		//uart_send_byte(str[i],1);
		UART1_Write(str[i]);
		i++;

		if (str[i] == '%') {
			i++;

			switch (str[i]) {
			case 'd':
				number = va_arg(ap, int);
				divisor_check = number;
				divisor = 0;
				power = -1;
				send_count = 0;

				while (divisor_check != 0) {
					divisor_check = divisor_check / 10;
					power++;
				}

				divisor = pow(10, power);

				while (divisor != 0) {
					send_count = number / divisor;
					number = number - send_count * divisor;
					divisor = divisor / 10;
					// Convert to ASCII value
					send_count = send_count + 48;
					//uart_send_byte(send_count);
					UART1_Write(send_count);
				}
				break;

			case 'c':
				//uart_send_byte(va_arg(ap, uint8_t)); //int
				UART1_Write(va_arg(ap, uint8_t));
				break;

			}
			i++;
		}
	}
	va_end(ap);
}

void UART1_Write(uint8_t data) {
	UART1->D = data;
	while (!(UART1->S1 & UART_S1_TDRE_MASK))
		;
}
