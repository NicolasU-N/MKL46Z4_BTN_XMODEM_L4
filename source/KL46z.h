#ifndef KL46Z_H_
#define KL46Z_H_

#include "system_MKL46Z4.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define SIM_SOPT2                                (*(unsigned int *)0x40048004u)
#define SIM_SOPT2_UART0SRC_SHIFT                 (26u)
#define SIM_SOPT2_PLLFLLSEL_SHIFT                (16u)

#define SIM_SOPT5                                (*(unsigned int *)0x40048010u)
#define SIM_SOPT5_UART0TXSRC_SHIFT               (0U)
#define SIM_SOPT5_UART0RXSRC_SHIFT               (2U)
#define SIM_SOPT5_UART0ODE_SHIFT                 (16U)

#define SIM_SCGC4                                (*(unsigned int *)0x40048034u)
#define SIM_SCGC4_UART0_SHIFT                    (10U)
#define SIM_SCGC4_UART1_SHIFT                    (11U)
#define SIM_SCGC4_UART2_SHIFT                    (12U)

#define SIM_SCGC5                                (*(unsigned int *)0x40048038u)
#define SIM_SCGC5_SHIFT(port)                    (9U + (port))

#define SIM_SCGC6                                (*(unsigned int *)0x4004803Cu)
#define SIM_SCGC6_PIT_SHIFT                      (23U)

#define SIM_CLKDIV1                              (*(unsigned int *)0x40048044u)
#define SIM_CLKDIV1_OUTDIV4_SHIFT                (16u)
#define SIM_CLKDIV1_OUTDIV4_MASK                 (0xF0000u)

#define SYST_CSR                                 (*(unsigned int *)0xE000E010u)
#define SYST_RVR                                 (*(unsigned int *)0xE000E014u)
#define SYST_CVR                                 (*(unsigned int *)0xE000E018u)
#define SYS_CSR_FLAG_SHIFT                       (16u)
#define SYS_CSR_CLK_SHIFT                        (2u)
#define SYS_CSR_TICKINT_SHIFT                    (1u)
#define SYS_CSR_ENABLE_SHIFT                     (0u)

#define NVIC_ISER                                (*(unsigned int *)0xE000E100u)
#define NVIC_ICER                                (*(unsigned int *)0xE000E180u)
#define NVIC_ICPR                                (*(unsigned int *)0xE000E280u)
#define NVIC_IPR(n)                              (*(unsigned int *)(0xE000E400u + 0x04*(n)))
#define NVIC_IPR_PRI_N0_SHIFT                    (6U)
#define NVIC_IPR_PRI_N1_SHIFT                    (14U)
#define NVIC_IPR_PRI_N2_SHIFT                    (22U)
#define NVIC_IPR_PRI_N3_SHIFT                    (30U)

#define PIT_MCR                                  (*(unsigned int *)0x40037000u)
#define PIT_MCR_MDIS_SHIFT                       (1u)
#define PIT_LDVAL(n)                             (*(unsigned int *)(0x40037100u + 0x10*(n)))
#define PIT_CVAL(n)                              (*(unsigned int *)(0x40037104u + 0x10*(n)))
#define PIT_TCTRL(n)                             (*(unsigned int *)(0x40037108u + 0x10*(n)))
#define PIT_TCTRL_CHN_SHIFT                      (2U)
#define PIT_TCTRL_TIE_SHIFT                      (1U)
#define PIT_TCTRL_TEN_SHIFT                      (0U)
#define PIT_TFLG(n)                              (*(unsigned int *)(0x4003710Cu + 0x10*(n)))
#define PIT_TFLG_TIF_SHIFT                       (0U)

#define MCG_C1                                 (*(unsigned char *)0x40064000u)
#define MCG_C2                                 (*(unsigned char *)0x40064001u)
#define MCG_C3                                 (*(unsigned char *)0x40064002u)
#define MCG_C4                                 (*(unsigned char *)0x40064003u)
#define MCG_C5                                 (*(unsigned char *)0x40064004u)
#define MCG_C4_DMX32_SHIFT                     (7u)
#define MCG_C4_DRS_SHIFT                       (5u)

/*UART0*/
#define UART0_BDH                              (*(unsigned char *)0x4006A000u)
#define UART0_BDH_LBKDIE_SHIFT                 (7u)
#define UART0_BDH_RXEDGIE_SHIFT                (6u)
#define UART0_BDH_SBNS_SHIFT                   (5u)
#define UART0_BDH_SBR_SHIFT                    (0u)

#define UART0_BDL                              (*(unsigned char *)0x4006A001u)

#define UART0_C1                               (*(unsigned char *)0x4006A002u)
#define UART0_C1_RSRC_SHIFT                    (5u)
#define UART0_C1_M_SHIFT                       (4u)
#define UART0_C1_ILT_SHIFT                     (4u)
#define UART0_C1_PE_SHIFT                      (1u)
#define UART0_C1_PT_SHIFT                      (0u)

#define UART0_C2                               (*(unsigned char *)0x4006A003u)
#define UART0_C2_TIE_SHIFT                      (7u)
#define UART0_C2_TCIE_SHIFT                     (6u)
#define UART0_C2_RIE_SHIFT                      (5u)
#define UART0_C2_ILIE_SHIFT                     (4u)
#define UART0_C2_TE_SHIFT                       (3u)
#define UART0_C2_RE_SHIFT                       (2u)
#define UART0_C3                               (*(unsigned char *)0x4006A006u)
#define UART0_C4                               (*(unsigned char *)0x4006A00Au)
#define UART0_C4_M10_SHIFT                     (5U)
#define UART0_C5                               (*(unsigned char *)0x4006A00Bu)
#define UART0_S1                               (*(unsigned char *)0x4006A004u)
#define UART0_S1_TDRE_SHIFT                    (7U)
#define UART0_S1_TC_SHIFT                      (6U)
#define UART0_S1_RDRF_SHIFT                    (5U)
#define UART0_S2                               (*(unsigned char *)0x4006A005u)
#define UART0_D                                (*(unsigned char *)0x4006A007u)
#define UART0_MA1                              (*(unsigned char *)0x4006A008u)
#define UART0_MA2                              (*(unsigned char *)0x4006A009u)

#define PORT_PCR_IRQC_MASK                       (0xF0000U)
#define PORT_PCR_IRQC_SHIFT                      (16U)
#define PORT_PCR_MUX_MASK                        (0x700U)
#define PORT_PCR_MUX_SHIFT                       (8U)
#define PORT_PCR_PS_MASK                         (0x1U)
#define PORT_PCR_PS_SHIFT                        (0U)
#define PORT_PCR_PE_MASK                         (0x2U)
#define PORT_PCR_PE_SHIFT                        (1U)

#define PORTA                                    (0u)
#define PORTB                                    (1u)
#define PORTC                                    (2u)
#define PORTD                                    (3u)
#define PORTE                                    (4u)

#define BASE_ADDR_PORT(port)                     (0x40049000u + 0x1000*(port))
#define BASE_ADDR_GPIO(port)                     (0x400FF000u + 0x40*(port))

/*Address: Base address + offset*/
#define GPIO_PSOR(port)                          (*(unsigned int *)(BASE_ADDR_GPIO(port) + 0x04))
#define GPIO_PCOR(port)                          (*(unsigned int *)(BASE_ADDR_GPIO(port) + 0x08))
#define GPIO_PTOR(port)                          (*(unsigned int *)(BASE_ADDR_GPIO(port) + 0x0C))
#define GPIO_PDIR(port)                          (*(unsigned int *)(BASE_ADDR_GPIO(port) + 0x10))
#define GPIO_PDDR(port)                          (*(unsigned int *)(BASE_ADDR_GPIO(port) + 0x14))

#define PORT_PCR(port,pin)                       (*(unsigned int *)(BASE_ADDR_PORT(port) + 0x04*(pin)))
#define PORT_ISFR(port)                          (*(unsigned int *)(BASE_ADDR_PORT(port) + 0xA0))

#define ID_INT_UART0                             (12u)
#define RX_PIN                                   (1u)
#define TX_PIN                                   (2u)

#endif /* _KL46Z_H_ */
