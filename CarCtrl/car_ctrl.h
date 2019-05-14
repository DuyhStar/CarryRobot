#ifndef _CAR_CTRL_H_
#define _CAR_CTRL_H_

#include "pwm_config.h"

#define CAR_FORWARD         0
#define CAR_BACK            1
#define CAR_LEFT            2
#define CAR_RIGHT           3

#define CAR_LEFT_ROTATE     4
#define CAR_RIGHT_ROTATE    5

#define CAR_FORWARD_LEFT    6
#define CAR_FORWARD_RIGHT   7
#define CAR_BACK_LEFT       8
#define CAR_BACK_RIGHT      9
#define CAR_LEFT_LEFT       10
#define CAR_LEFT_RIGHT      11
#define CAR_RIGHT_LEFT      12
#define CAR_RIGHT_RIGHT     13

#define  ALL_WHEEL_SPEED    50

void car_ctrl_init(void);

void car_sport(int16_t speed_x, int16_t speed_y, int16_t speed_rotate);
void car_move(uint8_t dir);
void car_rotate(uint8_t dir);
void car_turn(uint8_t dir, uint8_t range);

void car_stop(void);

void car_wheel_dir_set(bool w1, bool w2, bool w3, bool w4);
void car_wheel_speed_set(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4);

#endif /* _CAR_CTRL_H_ */
