#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

void display_init(uint32_t baud);
void qr_code1_show(void);
void qr_code2_show(void);
void color_show(void);
void tmp_show(uint8_t pos, char* str);
    
#endif /* _DISPLAY_H_ */
