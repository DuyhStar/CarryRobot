/*
 * pwm_config.c
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */
#include "pwm_config.h"

int pwm_clock_div;

void PWM_clock_div_set(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);
    pwm_clock_div = 16;
}

void PWM0_0123_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
        ;
    
    PWM_clock_div_set();
    
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinConfigure(GPIO_PB7_M0PWM1);
    
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

    //发生器设置
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //频率设置
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, SysCtlClockGet()/pwm_clock_div/1000);//1KHz
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, SysCtlClockGet()/pwm_clock_div/1000);//1KHz
    
    //脉宽设置
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100);//Duty:1%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)/100);//Duty:1%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100);//Duty:1%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)/100);//Duty:1%

    //打开发生器
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    
    //开启输出
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
}

void PWM0_67_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
        ;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
        ;
    
    PWM_clock_div_set();
    
    GPIOPinConfigure(GPIO_PD0_M0PWM6);
    GPIOPinConfigure(GPIO_PD1_M0PWM7);
    
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);

    //发生器工作模式设置设置
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //频率设置
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/pwm_clock_div/50);//50Hz
    
    //脉宽设置
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*75/1000);//Duty:7.5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*75/1000);//Duty:7.5%

    //打开发生器
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    
    //打开输出
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
}
