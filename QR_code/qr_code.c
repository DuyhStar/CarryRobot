#include "uart_config.h"

void qr_code_init(uint32_t baud)
{
    UART4_Init(baud);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
        ;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
}

void qr_code_trig(bool s)
{
    if(s == 1)
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
    else
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
}
