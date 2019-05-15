#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_pwm.h"
#include "driverlib/fpu.h"
#include "delay.h"
#include "pwm_config.h"
#include "uart_config.h"
#include "car_ctrl.h"
#include "servo.h"
#include "camera.h"
#include "tracking.h"
#include "display.h"
#include "wifi.h"
#include "qr_code.h"
#include "timer_config.h"
#include "key.h"

int8_t  X = 0, Y = 0;
uint8_t work_x     = 4, work_y     = 7;
uint8_t product_x  = 7, product_y  = 4;
uint8_t material_x = 4, material_y = 1;
uint8_t code_x     = 6, code_y     = 1;

uint8_t  forward[8], back[8], left[8], right[8];
uint16_t servo_angle[] = {480, 758, 789, 241};
uint8_t  task1[3] = {1, 2, 3}, task2[3] = {1, 2, 3};    //从二维码中读取的任务信息(1:红. 2:绿. 3:蓝.)
uint8_t  color[3] = {1, 2, 3};                          //物块摆放的颜色顺序

void interrupt_priority_set(void);
int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//40MHz
    FPUEnable();
    FPULazyStackingEnable();
    
    UART0_init(115200);
    key0_init();
    car_ctrl_init();
//    servo_ctrl_init(servo_angle);
//    delay_s(2);
    tracking_init();
    
    printf("All Ready\n");
    interrupt_priority_set();
    IntMasterEnable();

    /*************************************/
    //功能测试
    //implement
//    waitKey();
//    car_ready_move();
//    car_move_to(4, 7, X_PRE);
//    car_move_to(1, 1,Y_PRE);    
//    while(1)
//    {}
    /*************************************/
    
    waitKey();
    car_ready_move();

    car_move_to(code_x, code_y, X_PRE);//读取加工顺序
    delay_ms(1000);
    
    for(uint8_t i = 0; i < 3; i++){
        car_move_to(material_x, material_y, X_PRE);
        car_adjust_to_center();
        //从原料区夹取物块
        delay_ms(1000);
        
        car_move_to(work_x, work_y, Y_PRE);
        car_adjust_to_center();
        
        //放下物块到加工区
        delay_ms(1000);
    }
    
    car_move_to(code_x, code_y, Y_PRE);//读取成品搬运顺序

    for(uint8_t i = 0; i < 3; i++){
        car_move_to(4, 4, X_PRE);
        car_set_point_dir(CAR_POINT_RIGHT);
        
        car_move_to(work_x, work_y, Y_PRE);
        car_adjust_to_center();
        
        //从加工区夹取物块
        
        car_move_to(4, 4, Y_PRE);
        car_set_point_dir(CAR_POINT_FORWARD);
        
        car_move_to(product_x, product_y, Y_PRE);
        car_adjust_to_center();
        
        //放下物块到成品区 
    }
    
    car_move_to(1, 1, X_PRE);
    while(1)
    {
    }
}

void interrupt_priority_set(void)
{
    IntPrioritySet(INT_UART1, 0);
    IntPrioritySet(INT_TIMER0A, 1);
    IntPrioritySet(INT_TIMER1A, 2);
}
