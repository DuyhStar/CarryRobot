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
#include "CarCtrl/car_ctrl.h"

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |  SYSCTL_XTAL_16MHZ);//50MHz

    UART0_init(115200);
    car_ctrl_init();
    
    while(1)
    {
    }
}
