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

void tracking_init(void);
void tracking_addr_init(void);
void tracking_select(uint8_t addr);

void car_trk_line(uint8_t dir);

void car_forward_back_trk_line(uint8_t dir);
void car_left_trk_line(void);
void car_right_trk_line(void);

void car_go_n_line(uint8_t dir, uint8_t n);

uint8_t find_black_line_center(uint8_t* line_inf);
int8_t  compare_in_range(uint8_t a, uint8_t b);

void track_inf_print(void);

#endif /* _TRACKING_H_ */
