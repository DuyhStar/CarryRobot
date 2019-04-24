#include "timer_config.h"

void timer0_init(uint32_t ms)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
        ;

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);//使用全宽32位定时器周期计数模式

    TimerLoadSet(TIMER0_BASE, TIMER_BOTH, ms*(SysCtlClockGet()/1000));

    TimerIntRegister(TIMER0_BASE, TIMER_BOTH, Timer0_IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER0_BASE, TIMER_BOTH);
}
