#ifndef _UART_CONFIG_H_
#define _UART_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include <stdio.h>

void UART0_init(uint32_t Baud);
int  fputc(int ch, FILE *f);

void UART1_Init(uint32_t Baud);
void IntHandler_UART1(void);

void UART2_Init(uint32_t Baud);
//void IntHandler_UART2(void);

void UART3_Init(uint32_t Baud);
void IntHandler_UART3(void);

void UART4_Init(uint32_t Baud);
void IntHandler_UART4(void);

void UART5_Init(uint32_t Baud);
void IntHandler_UART5(void);

void UART7_Init(uint32_t Baud);
void IntHandler_UART7(void);

#endif /* _UART_CONFIG_H_ */
