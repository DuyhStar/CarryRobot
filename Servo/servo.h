#ifndef _SERVO_H_
#define _SERVO_H_

#include "pwm_config.h"
#include "uart_config.h"

void servo_ctrl_init(uint16_t* angle);
void servo_n_angle_set(uint8_t n, uint16_t angle);
void servo_angle_read(uint16_t* angle);

#endif /* _SERVO_H_ */


