#include "uart_config.h"
#include "qr_code.h"
#include "delay.h"

void qr_code_init(uint32_t baud)
{
    UART4_Init(baud);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
        ;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
}

void qr_code_change(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
    delay_ms(300);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
}
