#ifndef _ARM_H_
#define _ARM_H_

#include "servo.h"

void take_from_material(uint8_t pos);
void take_from_work(void);
void place_to_work(void);
void place_to_product(void);
uint8_t get_take_pos(uint8_t n);

void zhua(void);
void fang(void);

#endif /* _ARM_H_ */
