/*
 * delay.h
 *
 *  Created on: 2019.4.1
 *      Author: DuYihao
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void delay_s(uint32_t n);
void delay_ms(uint32_t n);
void delay_us(uint32_t n);


#endif /* _DELAY_H_ */
