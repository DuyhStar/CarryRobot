#ifndef _CAR_CTRL_H_
#define _CAR_CTRL_H_

#include "pwm_config.h"

void car_ctrl_init(void);

void car_go_forward(uint16_t speed);
void car_go_back(uint16_t speed);
void car_go_left(uint16_t speed);
void car_go_right(uint16_t speed);

void car_turn_left(uint16_t speed);
void car_turn_right(uint16_t speed);

void car_stop(void);

void car_wheel_dir_set(bool w1, bool w2, bool w3, bool w4);
void car_wheel_speed_set(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4);

#endif /* _CAR_CTRL_H_ */
