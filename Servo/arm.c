#include "arm.h"
#include "servo.h"
#include "delay.h"

void take_from_material(uint8_t n)
{
    extern uint8_t color[3], task1[3];
    uint8_t pos = get_take_pos(n+1);
    
    switch(pos){
        case 1:
            servo_position_set(0, 276, 1);
            servo_position_set(3, 650, 1);
            servo_position_set(1, 252, 1);
            servo_position_set(2, 630, 1);
            zhua();
            delay_ms(500);
            servo_position_set(2, 203, 0);
            servo_position_set(0, 701, 0);
            break;
        case 2:
            servo_position_set(0, 405, 1);
            servo_position_set(3, 697, 1);
            servo_position_set(1, 320, 1);
            servo_position_set(2, 770, 1);
            zhua();
            delay_ms(500);
            servo_position_set(2, 203, 0);
            servo_position_set(0, 701, 0);
            break;
        case 3:
            servo_position_set(0, 514, 1);
            servo_position_set(3, 650, 1);
            servo_position_set(1, 252, 1);
            servo_position_set(2, 630, 1);
            zhua();
            delay_ms(500);
            servo_position_set(2, 203, 0);
            servo_position_set(0, 701, 0);
            break;
    }
}

void take_from_work(void)
{
    servo_position_set(0, 955, 0);
    servo_position_set(3, 765, 1);
    servo_position_set(2, 735, 1);
    servo_position_set(1, 190, 1);
    zhua();
    delay_ms(500);
    servo_position_set(2, 203, 0);
    servo_position_set(1, 203, 0);
    servo_position_set(0, 701, 0);
}

void place_to_work(void)
{
    servo_position_set(0, 955, 1);
    servo_position_set(1, 200, 1);
    servo_position_set(3, 760, 1);
    servo_position_set(2, 755, 1);
    fang();
    delay_ms(500);
    servo_position_set(2, 203, 1);
    servo_position_set(1, 203, 1);
    servo_position_set(0, 701, 0);
}

void place_to_product(void)
{
    servo_position_set(0, 381, 1);
    servo_position_set(1, 162, 1);
    servo_position_set(3, 783, 1);
    servo_position_set(2, 727, 1);
    fang();
    delay_ms(500);
    servo_position_set(1, 213, 1);
    servo_position_set(2, 203, 1);
    servo_position_set(0, 701, 0);
}

//获得抓取的位置
//input:第n次抓取(1~3)
//output:抓什么位置(1~3)
uint8_t get_take_pos(uint8_t n)
{
    extern uint8_t task1[3], color[3];
    int i = 0;
    for(;i<3;i++)
    {
        if(task1[n-1] == color[i])
        {
            break;
        }
    }
    return i+1;
}

void zhua(void)
{
    zhua_zi_set(1900);
}
void fang(void)
{
    zhua_zi_set(900);
}
