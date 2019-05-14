#ifndef _CAR_CTRL_H_
#define _CAR_CTRL_H_

#include "pwm_config.h"

#define CAR_FORWARD         0
#define CAR_BACK            1
#define CAR_LEFT            2
#define CAR_RIGHT           3

#define CAR_LEFT_ROTATE     4
#define CAR_RIGHT_ROTATE    5

void car_ctrl_init(void);

void car_sport(int16_t vx, int16_t vy, int16_t wv);
void car_move(uint8_t dir);
void car_rotate(uint8_t dir);

void car_stop(void);

void car_wheel_dir_set(bool w1, bool w2, bool w3, bool w4);
void car_wheel_speed_set(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4);

#endif /* _CAR_CTRL_H_ */
