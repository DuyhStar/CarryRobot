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

void tracking_init(uint32_t baud);
void tracking_addr_init(void);
void tracking_select(uint8_t addr);



#endif /* _TRACKING_H_ */
