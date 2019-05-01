#include "tracking.h"
#include "uart_config.h"
#include <stdio.h>
#include "timer_config.h"

void tracking_init(uint32_t baud)
{
    UART1_Init(baud);
    tracking_addr_init();
    timer0_init(10);        //触发循迹模块采集
    timer1_init(20);        //切换循迹模块
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
    extern uint8_t track_addr;
    track_addr = addr;
    switch(addr)
    {
        case 1:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
    }
}

void track_inf_print(void)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    
    printf("F:");
    for(uint8_t i = 0; i < 8; i++)
        printf("%d", forward[i]);
    
    printf("  B:");
    for(uint8_t i = 0; i < 8; i++)
        printf("%d", back[i]);
    
    printf("  L:");
    for(uint8_t i = 0; i < 8; i++)
        printf("%d", left[i]);
    
    printf("  R:");
    for(uint8_t i = 0; i < 8; i++)
        printf("%d", right[i]);
    
    printf("\n");
}
