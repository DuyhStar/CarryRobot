#include "key.h"
#include "delay.h"

void key0_init(void)
{
    //开启外设
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}
        
    //设置PF4为上拉输入模式
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    
//    //配置中断
//    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
//    GPIOIntRegister(GPIO_PORTF_BASE, PORTF_IntHandler);   
//        
//    //开启中断
//    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
//    IntEnable(INT_GPIOF);
}
uint8_t Key0_Read(void)
{
    uint8_t s;
    s = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

    if(s != GPIO_PIN_4)
        return 1;
    else
        return 0;
}

void waitKey(void)
{
    while(1)
    {
        if(Key0_Read() == 1)
        {
            delay_ms(100);
            if(Key0_Read() == 1)
                break;
        }
    }
}
