#include "delay.h"
#include "pwm_config.h"
#include "uart_config.h"
#include "car_ctrl.h"
#include "servo.h"

//循迹
void IntHandler_UART1(void)
{
    UARTIntClear(UART1_BASE, UARTIntStatus(UART1_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART1_BASE))
    {
        c = UARTCharGetNonBlocking(UART1_BASE);
    }
}

//总线舵机
void IntHandler_UART2(void)
{
    UARTIntClear(UART2_BASE, UARTIntStatus(UART2_BASE, true));

    char c;
    while(UARTCharsAvail(UART2_BASE))
    {
        c = UARTCharGetNonBlocking(UART2_BASE);
    }
}

//摄像头
void IntHandler_UART3(void)
{
    UARTIntClear(UART3_BASE, UARTIntStatus(UART3_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART3_BASE))
    {
        c = UARTCharGetNonBlocking(UART3_BASE);
    }
}

//扫码
void IntHandler_UART4(void)
{
    UARTIntClear(UART4_BASE, UARTIntStatus(UART4_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART4_BASE))
    {
        c = UARTCharGetNonBlocking(UART4_BASE);
    }
}

//WIFI
void IntHandler_UART5(void)
{
    UARTIntClear(UART4_BASE, UARTIntStatus(UART4_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART4_BASE))
    {
        c = UARTCharGetNonBlocking(UART4_BASE);
    }
}

//屏幕
void IntHandler_UART7(void)
{
    UARTIntClear(UART7_BASE, UARTIntStatus(UART7_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART7_BASE))
    {
        c = UARTCharGetNonBlocking(UART7_BASE);
    }
}
