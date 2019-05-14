#include "car_ctrl.h"
#include <stdio.h>

uint16_t w1_speed = ALL_WHEEL_SPEED;
uint16_t w2_speed = ALL_WHEEL_SPEED;
uint16_t w3_speed = ALL_WHEEL_SPEED;
uint16_t w4_speed = ALL_WHEEL_SPEED;

//1. PA5, PA6
//2. PA7, PD2
//3. PD3, PE2
//4. PE3, PC5
void car_ctrl_init(void)
{
    PWM0_0123_init();
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
        ;
    
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    
    car_stop();
}

//vx: 前后移动速度，向前为正。(-99,-1)&&(1,99)
//vy: 左右移动速度，向左为正。(-99,-1)&&(1,99)
//wv: 旋转速度，  逆时针为正。(-99,-1)&&(1,99)
void car_sport(int16_t vx, int16_t vy, int16_t wv)
{
    int16_t v1, v2, v3, v4;
    int16_t k = 1;          //k = abs(Xn) + abs(Yn)
    v1 = vx - vy - k*wv;
    v2 = vx + vy - k*wv;
    v3 = vx - vy + k*wv;
    v4 = vx + vy + k*wv;
    
    bool dir1,dir2,dir3,dir4;
    dir1 = v1 > 0? 0:1;
    dir2 = v2 > 0? 0:1;
    dir3 = v3 > 0? 0:1;
    dir4 = v4 > 0? 0:1;
    car_wheel_dir_set(dir1, dir2, dir3, dir4);
    
    v1 = v1 > 0? (v1):(-v1);
    v2 = v2 > 0? (v2):(-v2);
    v3 = v3 > 0? (v3):(-v3);
    v4 = v4 > 0? (v4):(-v4);
    car_wheel_speed_set(v1, v2, v3, v4);
}

void car_move(uint8_t dir)
{
    switch(dir)
    {
        case CAR_FORWARD:   car_wheel_dir_set(0, 0, 0, 0);  break;
        case CAR_BACK:      car_wheel_dir_set(1, 1, 1, 1);  break;
        case CAR_LEFT:      car_wheel_dir_set(1, 0, 1, 0);  break;
        case CAR_RIGHT:     car_wheel_dir_set(0, 1, 0, 1);  break;
    }
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}
void car_rotate(uint8_t dir)
{
    switch(dir)
    {
        case CAR_LEFT_ROTATE:   car_wheel_dir_set(1, 1, 0, 0);  break;
        case CAR_RIGHT_ROTATE:  car_wheel_dir_set(0, 0, 1, 1);  break;
    }
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_turn(uint8_t dir, uint8_t range)
{
    switch(dir)
    {
        case (CAR_FORWARD_LEFT):
            car_wheel_dir_set(0, 0, 0, 0);  
            car_wheel_speed_set(w1_speed+range, w2_speed+range, w3_speed-range, w4_speed-range);
            break;
        case (CAR_FORWARD_RIGHT):
            car_wheel_dir_set(0, 0, 0, 0);
            car_wheel_speed_set(w1_speed-range, w2_speed-range, w3_speed+range, w4_speed+range);
            break;
        
        case (CAR_BACK_LEFT):
            car_wheel_dir_set(1, 1, 1, 1);  
            car_wheel_speed_set(w1_speed-range, w2_speed-range, w3_speed+range, w4_speed+range);
            break;
        case (CAR_BACK_RIGHT):
            car_wheel_dir_set(1, 1, 1, 1); 
            car_wheel_speed_set(w1_speed+range, w2_speed+range, w3_speed-range, w4_speed-range);
            break;
        
        case (CAR_LEFT_LEFT):
            car_wheel_dir_set(1, 0, 0, 1);
            car_wheel_speed_set(w1_speed-range, w2_speed+range, w3_speed-range, w4_speed+range);
            break;
        case (CAR_LEFT_RIGHT):
            car_wheel_dir_set(1, 0, 0, 1);
            car_wheel_speed_set(w1_speed+range, w2_speed-range, w3_speed+range, w4_speed-range);
            break;
        
        case (CAR_RIGHT_LEFT):
            car_wheel_dir_set(0, 1, 1, 0);
            car_wheel_speed_set(w1_speed+range, w2_speed-range, w3_speed+range, w4_speed-range);
            break;
        case (CAR_RIGHT_RIGHT):
            car_wheel_dir_set(0, 1, 1, 0);
            car_wheel_speed_set(w1_speed-range, w2_speed+range, w3_speed-range, w4_speed+range);
            break;
    }
}

void car_stop(void)
{
    //方向控制信号全部置高，刹车
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0xFF);//1. PA5, PA6
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF);
    
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xFF);//2. PA7, PD2
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xFF);
    
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0xFF);//3. PD3, PE2
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0xFF);
    
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0xFF);//4. PE3, PC5
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0xFF);
}

//range:[1, 99]
void car_wheel_speed_set(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4)
{
    s1 = ( (s1 == 0)? (1):(s1) );
    s2 = ( (s2 == 0)? (1):(s2) );
    s3 = ( (s3 == 0)? (1):(s3) );
    s4 = ( (s4 == 0)? (1):(s4) );
    
    s1 = ( (s1 > 99)? (99):(s1) );
    s2 = ( (s2 > 99)? (99):(s2) );
    s3 = ( (s3 > 99)? (99):(s3) );
    s4 = ( (s4 > 99)? (99):(s4) );
    
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, s1*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100));//1
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, s2*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100));//2
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, s3*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100));//3
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, s4*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100));//4
}

void car_wheel_dir_set(bool w1, bool w2, bool w3, bool w4)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, ((w1==1)?0xFF:0x00));//1. PA5, PA6
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, ((w1==0)?0xFF:0x00));

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, ((w2==1)?0x00:0xFF));//2. PA7, PD2
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ((w2==0)?0x00:0xFF));
    
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, ((w3==1)?0x00:0xFF));//3. PD3, PE2
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, ((w3==0)?0x00:0xFF));
    
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, ((w4==1)?0x00:0xFF));//4. PE3, PC5
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, ((w4==0)?0x00:0xFF));
}

