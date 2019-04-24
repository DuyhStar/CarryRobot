#include "camera.h"

void camera_link_init(uint32_t baud)
{
    UART3_Init(baud);
}
