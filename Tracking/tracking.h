#ifndef _TRACKING_H_
#define _TRACKING_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#define X_PRE 0
#define Y_PRE 1

#define CAR_POINT_FORWARD 0
#define CAR_POINT_BACK    1
#define CAR_POINT_LEFT    2
#define CAR_POINT_RIGHT   3

void    tracking_init(void);
void    tracking_addr_init(void);
void    tracking_select(uint8_t addr);

void    car_trk_line(uint8_t dir);

void    car_ready_move(void);
void    car_move_to(uint8_t tar_x, uint8_t tar_y, uint8_t pre);
void    car_x_move(int8_t err);
void    car_y_move(int8_t err);
void    car_go_n_line(uint8_t dir, uint8_t n);
void    update_XY(uint8_t dir);

void    car_adjust_to_center(void);
void    car_rotate_90_degree(uint8_t dir);
void    car_rotate_n_90_degree(uint8_t dir, uint16_t n);
void    car_set_point_dir(uint8_t dir);

uint8_t find_black_line_center(uint8_t* line_inf);

void    track_inf_print(void);

#endif /* _TRACKING_H_ */
