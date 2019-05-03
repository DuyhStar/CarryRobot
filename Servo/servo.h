#ifndef _SERVO_H_
#define _SERVO_H_

#include "pwm_config.h"
#include "uart_config.h"

void servo_ctrl_init(uint16_t* pos);
void servo_position_set(uint8_t ID,uint16_t position);
void servo_speed_set(uint8_t ID,uint16_t speed);
void servo_position_get(uint8_t ID, uint16_t* pos);

#endif /* _SERVO_H_ */


