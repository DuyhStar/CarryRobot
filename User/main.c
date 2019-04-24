#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_pwm.h"

#include <stdlib.h>
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

uint16_t servo_angle[] = {90, 90, 90, 90};

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
    
    while(1)
    {
    }
}



