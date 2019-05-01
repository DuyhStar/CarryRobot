#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_pwm.h"
#include "driverlib/fpu.h"

#include "delay.h"
#include "pwm_config.h"
#include "uart_config.h"
#include "car_ctrl.h"
#include "servo.h"
#include "camera.h"
#include "tracking.h"
#include "display.h"
#include "wifi.h"
#include "qr_code.h"
#include "timer_config.h"

uint8_t  track_addr,forward[8],back[8],left[8],right[8];
uint16_t servo_angle[] = {90, 90, 90, 90};
uint8_t  task[3]       = {1,2,3};                       //从二维码中读取的任务信息(1:红. 2:绿. 3:蓝.)
uint8_t  color[3]      = {1,2,3};                       //物块摆放的颜色顺序

void interrupt_priority_set(void);
int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//50MHz
    FPUEnable();
    FPULazyStackingEnable();
    
    UART0_init(115200);
//    car_ctrl_init();
//    servo_ctrl_init(servo_angle);
    tracking_init(9600);
    
    printf("All Ready\n");
    interrupt_priority_set();
    IntMasterEnable();
    while(1)
    {
        track_inf_print();
        delay_ms(100);
    }
}

void interrupt_priority_set(void)
{
    IntPrioritySet(INT_UART1, 0);
    IntPrioritySet(INT_TIMER0A, 1);
    IntPrioritySet(INT_TIMER1A, 2);
}







