/*
 * uart_config.c
 *
 *  Created on: 2019��3��18��
 *      Author: DuYihao
 */
#include "uart_config.h"

void UART0_init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
        ;
    
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);//Clock 16MHz
    
    UARTStdioConfig(0, Baud, 16000000);
}
void UART1_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART1_BASE, IntHandler_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART1);
}
void UART2_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    /* Unlock PD7 */
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x4C4F434B;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;

    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART2_BASE, IntHandler_UART2);
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART2);
}
void UART3_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;
    
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART3_BASE, IntHandler_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART3);
}
void UART4_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART4))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;
    
    GPIOPinConfigure(GPIO_PC4_U4RX);
    GPIOPinConfigure(GPIO_PC5_U4TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART4_BASE, IntHandler_UART4);
    UARTIntEnable(UART4_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART4);
}
void UART5_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART5_BASE, IntHandler_UART5);
    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART5);
}
void UART7_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);

    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART7_BASE, IntHandler_UART7);
    UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART7);
}
