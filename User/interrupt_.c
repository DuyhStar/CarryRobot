#include "delay.h"
#include "pwm_config.h"
#include "uart_config.h"
#include "car_ctrl.h"
#include "servo.h"
#include "timer_config.h"
#include "tracking.h"

void Timer0_IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    UARTCharPut(UART1_BASE, 0x57);
}

void Timer1_IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    static uint16_t addr = 0;
    addr++;
    switch(addr)
    {
        case 1: tracking_select(addr);           break;
        case 2: tracking_select(addr);           break;
        case 3: tracking_select(addr);           break;
        case 4: tracking_select(addr); addr = 0; break;
    }
}
//void PORTF_IntHandler(void)
//{
//    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);
//    GPIOIntClear(GPIO_PORTF_BASE, s);
//    
//    if((s&GPIO_PIN_4) == GPIO_PIN_4)
//    {
//        while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))//等待按键松开
//            ;
//        extern bool key0_flag;
//        key0_flag = 1;
//    }
//}

//循迹
void IntHandler_UART1(void)
{
    UARTIntClear(UART1_BASE, UARTIntStatus(UART1_BASE, true));

    extern uint8_t track_addr;
    extern uint8_t forward[8],back[8],left[8],right[8];
    char c;
    bool bit;
    while(UARTCharsAvail(UART1_BASE)){
        c = UARTCharGetNonBlocking(UART1_BASE);
        for(uint8_t i = 0; i < 8; i++){
            bit = c&0x01;
            c = c>>1;
            switch(track_addr){
                case 1: forward[i] = !bit;  break;
                case 2: back[i]    = !bit;  break;
                case 3: left[i]    = !bit;  break;
                case 4: right[i]   = !bit;  break;
            }
        }
    }

    track_addr++;
    switch(track_addr){
        case 1: 
            tracking_select(track_addr);          
            break;
        case 2: 
            tracking_select(track_addr);     
            break;
        case 3:
            tracking_select(track_addr);         
            break;
        case 4:
            tracking_select(track_addr);
            break;
        case 5:
            tracking_select(1);   
            track_addr = 1;
            break;
    }
}

//总线舵机
//void IntHandler_UART2(void)
//{
//    UARTIntClear(UART2_BASE, UARTIntStatus(UART2_BASE, true));

//    char c;
//    while(UARTCharsAvail(UART2_BASE))
//    {
//        c = UARTCharGetNonBlocking(UART2_BASE);
//    }
//}

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
    UARTIntClear(UART5_BASE, UARTIntStatus(UART5_BASE, true));

    char c = 0;
    while(UARTCharsAvail(UART5_BASE))
    {
        c = UARTCharGetNonBlocking(UART5_BASE);
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
