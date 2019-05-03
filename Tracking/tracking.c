#include "tracking.h"
#include "uart_config.h"
#include <stdio.h>
#include "timer_config.h"
#include "car_ctrl.h"

uint8_t  track_addr;

void tracking_init(void)
{
    UART1_Init(9600);
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
    switch(addr){
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
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", forward[i]);
    
    printf("  B:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", back[i]);
    
    printf("  L:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", left[i]);
    
    printf("  R:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", right[i]);
    
    printf("\n");
}

void car_trk_line(uint8_t dir, uint8_t wide)
{
    extern uint8_t forward[8],back[8],left[8],right[8];
    uint8_t left_side = 0, right_side = 0;
    uint8_t *line_inf;
    
    switch(dir){
        case CAR_FORWARD: line_inf = forward; break;
        case CAR_BACK:    line_inf = back;    break;
        case CAR_LEFT:    line_inf = left;    break;
        case CAR_RIGHT:   line_inf = right;   break;
    }
    trk_line_wide_select(line_inf, wide, &left_side, &right_side);
    
    if( left_side && right_side ){
        car_move(dir);
    }
    else if(right_side){
        car_rotate(CAR_RIGHT_ROTATE);
        while(!line_inf[3])
            ;
    }
    else if(left_side){
        car_rotate(CAR_LEFT_ROTATE);
        while(!line_inf[3])
            ;
    }
    else{
        car_move(dir);
    }
}

void trk_line_wide_select(uint8_t* line_inf, uint8_t wide, uint8_t* left_side, uint8_t* right_side)
{
    switch(wide){
        case 0: *left_side  = line_inf[0] || line_inf[1] || line_inf[2]; 
                *right_side = line_inf[5] || line_inf[6];   
                break;
        case 1: *left_side  = line_inf[0] || line_inf[1];
                *right_side = line_inf[5] || line_inf[6];  
                break;
        case 2: *left_side  = line_inf[0];
                *right_side = line_inf[6];                 
                break;
    }
}
