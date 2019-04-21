/*
 * car_ctrl.c
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */
#include "car_ctrl.h"

uint16_t w1_speed = 50;
uint16_t w2_speed = 50;
uint16_t w3_speed = 50;
uint16_t w4_speed = 50;

//1. PA5, PA6
//2. PA7, PD2
//3. PD3, PE2
//4. PE3, PC5
void car_ctrl_init(void)
{
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
    
    PWM0_0123_init();
}

void car_go_forward(uint16_t speed)
{
    car_wheel_dir_set(1, 1, 1, 1);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_go_back(uint16_t speed)
{
    car_wheel_dir_set(0, 0, 0, 0);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_go_left(uint16_t speed)
{
    car_wheel_dir_set(0, 1, 1, 0);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_go_right(uint16_t speed)
{
    car_wheel_dir_set(1, 0, 0, 1);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_turn_left(uint16_t speed)
{
    car_wheel_dir_set(0, 0, 1, 1);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_turn_right(uint16_t speed)
{
    car_wheel_dir_set(1, 1, 0, 0);
    car_wheel_speed_set(w1_speed, w2_speed, w3_speed, w4_speed);
}

void car_stop(void)
{
    //关掉4路PWM输出
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    
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

void car_wheel_speed_set(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, s1*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100));//1
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, s2*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100));//2
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, s3*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100));//3
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, s4*(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100));//4
}

void car_wheel_dir_set(bool w1, bool w2, bool w3, bool w4)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, ((w1==1)?0xFF:0x00));//1. PA5, PA6
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, ((w1==0)?0xFF:0x00));

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, ((w1==1)?0xFF:0x00));//2. PA7, PD2
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ((w1==0)?0xFF:0x00));

    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, ((w1==1)?0xFF:0x00));//3. PD3, PE2
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, ((w1==0)?0xFF:0x00));

    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, ((w1==1)?0xFF:0x00));//4. PE3, PC5
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, ((w1==0)?0xFF:0x00));
}
