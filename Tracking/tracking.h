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
void track_inf_print(void);

void car_trk_line(uint8_t dir, uint8_t wide);
void trk_line_wide_select(uint8_t* line_inf, uint8_t wide, uint8_t* left, uint8_t* right);
uint8_t find_black_pos(uint8_t* line_inf);

void car_left_trk_line(void);

#endif /* _TRACKING_H_ */
