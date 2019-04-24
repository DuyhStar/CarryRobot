#include "wifi.h"
#include "uart_config.h"

void wifi_link_init(uint32_t baud)
{
    UART5_Init(baud);
}


