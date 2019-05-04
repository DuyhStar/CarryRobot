#include "tracking.h"
#include "uart_config.h"
#include <stdio.h>
#include "timer_config.h"
#include "car_ctrl.h"

uint8_t  track_addr = 1;

void tracking_init(void)
{
    UART1_Init(9600);
    tracking_addr_init();
    tracking_select(track_addr); 
    timer0_init(10);        //触发循迹模块采集
    //timer1_init(23);        //切换循迹模块
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

void car_trk_line(uint8_t dir)
{
    switch(dir){
        case CAR_FORWARD:
        case CAR_BACK: 
            car_forward_back_trk_line(dir); 
            break;
        case CAR_LEFT: 
            car_left_trk_line();
            break;
        case CAR_RIGHT: 
            car_right_trk_line(); 
            break;
    }
}

void car_forward_back_trk_line(uint8_t dir)
{
    extern uint8_t forward[8],back[8],left[8],right[8];
    uint8_t* line_inf;
    
    switch(dir){
        case CAR_FORWARD: line_inf = forward; break;
        case CAR_BACK:    line_inf = back;    break;
    }
    
    uint8_t center = find_black_line_center(line_inf);
    
    uint8_t range = 2;
    if(center >= 6+range){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else if(center <= 6-range){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else{
        car_move(dir);
    }
}
void car_left_trk_line(void)
{
    extern uint8_t left[8], right[8];
    
    uint8_t left_center  = find_black_line_center(left);
    uint8_t right_center = (12-find_black_line_center(right));
    
    if(compare_in_range(left_center, right_center) > 0){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else if(compare_in_range(left_center, right_center) < 0){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else{
        uint8_t range = 3;
        if(left_center >= 6+range){
            car_move(CAR_FORWARD);
        }
        else if(left_center <= 6-range){
            car_move(CAR_BACK);
        }
        else{
            car_move(CAR_LEFT);
        }
    }
}
void car_right_trk_line(void)
{
    extern uint8_t left[8], right[8];
    
    uint8_t left_center  = (12-find_black_line_center(left));
    uint8_t right_center = find_black_line_center(right);
    
    if(compare_in_range(left_center, right_center) > 0){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else if(compare_in_range(left_center, right_center) < 0){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else{
        uint8_t range = 3;
        if(right_center >= 6+range){
            car_move(CAR_BACK);
        }
        else if(right_center <= 6-range){
            car_move(CAR_FORWARD);
        }
        else{
            car_move(CAR_RIGHT);
        }
    }
}

int8_t compare_in_range(uint8_t a, uint8_t b)
{
    uint8_t range = 1;
    uint8_t a_min = a-range, a_max = a+range;
    uint8_t b_min = b-range, b_max = b+range;
    
    if(a < b){
        if(a_max < b_min){
            return -1;
        }
        else{
            return 0;
        }
    }
    else if(a > b){
        if(a_min > b_max){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
    }
    return 0;
}

void car_go_n_line(uint8_t dir, uint8_t n)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    uint8_t *count_line;
    switch(dir){
        case CAR_FORWARD: count_line = left;    break;
        case CAR_BACK:    count_line = right;   break;
        case CAR_LEFT:    count_line = back;    break;
        case CAR_RIGHT:   count_line = forward; break;
    }
    uint8_t count = 0, pos = 6;
    while(1){
        car_trk_line(dir);
        if(count_line[pos]){
            pos--;
        }
        if(pos == 3){
            pos = 6;
            count++;
        }
        if(count == n){
            break;
        }
    }
    car_stop();
}

uint8_t find_black_line_center(uint8_t* line_inf)
{
    uint8_t begin = 0, end = 0;
    for(uint8_t i = 0; i <= 6; i++){
        if(line_inf[i] == 1){
            begin = i;
            break;
        }
    }
    for(int8_t i = 6; i >= 0; i--){
        if(line_inf[i] == 1){
            end = i;
            break;
        }
    }
    
    uint8_t center = begin*2 + end - begin;
    return center;
}












