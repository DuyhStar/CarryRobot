#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_pwm.h"

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

uint8_t  track_addr;
uint8_t  forward[8],back[8],left[8],right[8];
uint16_t servo_angle[] = {90, 90, 90, 90};
uint8_t  task[3]       = {1,2,3};                       //从二维码中读取的任务信息(1:红. 2:绿. 3:蓝.)
uint8_t  color[3]      = {1,2,3};                       //物块摆放的颜色顺序

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//50MHz

    UART0_init(115200);
    car_ctrl_init();
    servo_ctrl_init(servo_angle);
    tracking_init(9600);
    display_init(9600);
    wifi_link_init(9600);
    camera_link_init(9600);
    qr_code_init(9600);

    timer0_init(100);
    
    UARTprintf("All Ready\n");
    tracking_select(2);
    car_go_left(50);
    delay_s(1);
    car_go_right(50);
    delay_s(1);
    car_go_forward(50);
    delay_s(1);
    car_go_back(50);
    delay_s(1);
    car_stop();
    while(1)
    {
        //car_go_forward(90);
//        for(uint8_t i = 0; i < 8; i++)
//        {
//            UARTprintf("%d",back[i]);
//        }
//        UARTprintf("\n");
//        delay_ms(500);
//         car_stop();
//        delay_ms(500);
    }
}









