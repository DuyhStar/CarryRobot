#include "servo.h"

void servo_ctrl_init(uint16_t* angle)
{
    PWM0_67_init();
    UART2_Init(115200);
}

void servo_n_angle_set(uint8_t n, uint16_t angle)
{
    
}

void servo_angle_read(uint16_t* angle)
{
    
}
