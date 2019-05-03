#ifndef _KEY_H_
#define _KEY_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

void key0_init(void);
//void PORTF_IntHandler(void);
uint8_t Key0_Read(void);
void waitKey(void);

#endif /* _KEY_H_ */
