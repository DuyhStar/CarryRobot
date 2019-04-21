/*
 * uart_config.h
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */

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

void UART0_init(uint32_t Baud);

//void UART1_Init(uint32_t Baud);
//void IntHandler_UART1();

//void UART2_Init(uint32_t Baud);
//void IntHandler_UART2();

//void UART3_Init(uint32_t Baud);
//void IntHandler_UART3();

//void UART4_Init(uint32_t Baud);
//void IntHandler_UART4();

////void UART5_Init(uint32_t Baud);
////void IntHandler_UART5();

//void UART7_Init(uint32_t Baud);
//void IntHandler_UART7();

#endif /* _UART_CONFIG_H_ */
