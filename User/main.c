#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"


void PF_123_set_to_out(void);

int main()
{
    PF_123_set_to_out();
    while(1)
    {
    }
}
void PF_123_set_to_out(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
}
