#ifndef _TIMER_CONFIG_H_
#define _TIMER_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

void timer0_init(uint32_t ms);
void Timer0_IntHandler(void);

void timer1_init(uint32_t ms);
void Timer1_IntHandler(void);

#endif /* _TIMER_CONFIG_H_ */
