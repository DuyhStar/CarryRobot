#include "tracking.h"
#include "uart_config.h"

void tracking_init(uint32_t baud)
{
    UART1_Init(baud);
    tracking_addr_init();
    tracking_select(1);
}

void tracking_addr_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
        ;
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
}

void tracking_select(uint8_t addr)
{
    switch(addr)
    {
        case 1:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            break;
        default:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            break;
    }
}
