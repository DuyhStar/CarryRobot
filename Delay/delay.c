/*
 * delay.c
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */
#include "delay.h"

void delay_s(uint32_t n)
{
    for(uint32_t i = 0; i < n; i++)
        SysCtlDelay(SysCtlClockGet()/3);
}

void delay_ms(uint32_t n)
{
    for(uint32_t i = 0; i < n; i++)
        SysCtlDelay(SysCtlClockGet()/3000);
}

void delay_us(uint32_t n)
{
    
    for(uint32_t i = 0; i < n; i++)
        SysCtlDelay(SysCtlClockGet()/3000000);
}
