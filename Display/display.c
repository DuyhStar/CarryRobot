#include "uart_config.h"
#include "display.h"
#include "string.h"

void display_init(uint32_t baud)
{
    UART7_Init(baud);
    tmp_show(0, "Hello,");
    tmp_show(3, "Good Luck");
}

void qr_code1_show(void)
{
    extern uint8_t task1[3];

    uint8_t t[12] = "t1.txt=\"000\"";

    t[8]  = task1[0] + 48;
    t[9]  = task1[1] + 48;
    t[10] = task1[2] + 48;

    int i = 0;
    for(;i<12;i++)
        UARTCharPut(UART7_BASE, t[i]);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
}
void qr_code2_show(void)
{
    extern uint8_t task2[3];

    uint8_t t[12] = "t1.txt=\"000\"";

    t[8]  = task2[0] + 48;
    t[9]  = task2[1] + 48;
    t[10] = task2[2] + 48;

    int i = 0;
    for(;i<12;i++)
        UARTCharPut(UART7_BASE, t[i]);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
}

void color_show(void)
{
    extern uint8_t color[3];

    uint8_t t[12] = "t2.txt=\"000\"";

    t[8]  = color[0] + 48;
    t[9]  = color[1] + 48;
    t[10] = color[2] + 48;

    int i = 0;
    for(;i<12;i++)
        UARTCharPut(UART7_BASE, t[i]);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
}

void tmp_show(uint8_t pos, char* str)
{
    uint8_t t[8] = "tn.txt=\"";
    t[1] = pos + 48;
    uint8_t len = strlen(str);

    for(uint8_t i = 0; i <= 7; i++)
        UARTCharPut(UART7_BASE, t[i]);
    
    for(uint8_t i = 0; i < len; i++)
        UARTCharPut(UART7_BASE, str[i]);

    UARTCharPut(UART7_BASE, '\"');
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
    UARTCharPut(UART7_BASE, 0xFF);
}
