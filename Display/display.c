#include "uart_config.h"

void display_init(uint32_t baud)
{
    UART7_Init(baud);
}
