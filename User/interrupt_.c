#include "delay.h"
#include "pwm_config.h"
#include "uart_config.h"
#include "car_ctrl.h"
#include "servo.h"
#include "timer_config.h"
#include "tracking.h"

uint8_t compare(uint8_t *t, char* p)
{
    uint8_t s0 = 0,s1 = 0,s2 = 0;
    s0 = (t[0]==(p[0]-48));
    s1 = (t[1]==(p[1]-48));
    s2 = (t[2]==(p[2]-48));
    if(s0&&s1&&s2)
        return 1;
    else
        return 0;
}

void Timer0_IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    UARTCharPut(UART1_BASE, 0x57);
}

void Timer1_IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    
    TimerDisable(TIMER1_BASE, TIMER_BOTH);
    
    extern bool trk_cnt_flag;
    trk_cnt_flag = 1;
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

    uint8_t c = 0, i= 0, temp[3] = {4,4,4};
    extern uint8_t color[3];
    extern bool    color_get;
    while(UARTCharsAvail(UART3_BASE))
    {
        c = UARTCharGetNonBlocking(UART3_BASE);
        if(i == 3)
            break;
        temp[i] = c;
        i++;
    }
    uint8_t s0 = compare(temp,"123");
    uint8_t s1 = compare(temp,"132");
    uint8_t s2 = compare(temp,"213");
    uint8_t s3 = compare(temp,"231");
    uint8_t s4 = compare(temp,"321");
    uint8_t s5 = compare(temp,"312");
    if(s0 || s1 || s2 || s3 ||s4 || s5)
    {
        color[0] = temp[0];
        color[1] = temp[1];
        color[2] = temp[2];
        color_get = 1;
    }
}

//扫码
void IntHandler_UART4(void)
{
    UARTIntClear(UART4_BASE, UARTIntStatus(UART4_BASE, true));

    uint8_t c = 0, i= 0, temp[3] = {4,4,4};
    while(UARTCharsAvail(UART4_BASE))
    {
        c = UARTCharGetNonBlocking(UART4_BASE);
        if(i == 3)
            break;
        temp[i] = c - 48;
        i++;
    }
    uint8_t s0 = compare(temp,"123");
    uint8_t s1 = compare(temp,"132");
    uint8_t s2 = compare(temp,"213");
    uint8_t s3 = compare(temp,"231");
    uint8_t s4 = compare(temp,"321");
    uint8_t s5 = compare(temp,"312");
    if(s0 || s1 || s2 || s3 ||s4 || s5)
    {
        extern uint8_t task1[3], task2[3];
        extern bool qr_code_get;
        task1[0] = temp[0];
        task1[1] = temp[1];
        task1[2] = temp[2];
        task2[0] = temp[0];
        task2[1] = temp[1];
        task2[2] = temp[2];
        qr_code_get = 1;
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
//void IntHandler_UART7(void)
//{
//    UARTIntClear(UART7_BASE, UARTIntStatus(UART7_BASE, true));

//    char c = 0;
//    while(UARTCharsAvail(UART7_BASE))
//    {
//        c = UARTCharGetNonBlocking(UART7_BASE);
//    }
//}
