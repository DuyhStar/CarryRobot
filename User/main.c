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
#include "arm.h"

int8_t  X = 0, Y = 0;
uint8_t work_x     = 4, work_y     = 7;
uint8_t product_x  = 7, product_y  = 4;
uint8_t material_x = 4, material_y = 1;
uint8_t code_x     = 6, code_y     = 1;

uint8_t  forward[8], back[8], left[8], right[8];
uint16_t servo_angle[] = {701, 202, 203, 774};
uint8_t  task1[3] = {1, 2, 3}, task2[3] = {2, 3, 1};    //从二维码中读取的任务信息(1:红. 2:绿. 3:蓝.)
uint8_t  color[3] = {1, 2, 3};                           //物块摆放的颜色顺序
bool     qr_code_get = 0, color_get = 0;

extern int16_t vx, vy, wv;

void pruduct_area_adjust_pos(void);
void work_area_adjust_pos(uint8_t pos);
void interrupt_priority_set(void);
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//40MHz
    FPUEnable();
    FPULazyStackingEnable();
    
    UART0_init(115200);             //调试串口初始化
    key0_init();                    //按键初始化
    car_ctrl_init();                //小车控制初始化
    servo_ctrl_init(servo_angle);   //机械臂控制初始化
    camera_link_init(9600);         //摄像头连接初始化
    qr_code_init(9600);             //二维码模块初始化
    wifi_link_init(115200);         //Wifi连接初始化(暂时用来进行Zigbee远程通信调试串口)
    display_init(9600);             //屏幕显示初始化
    tracking_init();                //循迹模块初始化
    
    printf("All Ready\n");          //输出提示信息
    interrupt_priority_set();       //设置中断优先级
    IntMasterEnable();              //打开全局中断

    /*************************************/
    //用来进行功能测试
    //
    //
    while(1){
        printf("Zigbee\n");
        delay_ms(100);
    }
    /*************************************/
    
    waitKey();
    qr_code_change();                   //打开二维码识别
    car_ready_move();
    car_move_to(code_x, code_y, X_PRE); //走到二维码区

    //读取二维码
    while(1){
        if(qr_code_get){
            qr_code_get = 0;
            qr_code1_show();
            break;
        }
    }
    qr_code_change();                   //关闭二维码识别
    
    //摄像头读取颜色顺序
    while(!color_get){
        car_move_to(4, 3, X_PRE);
        car_move_to(4, 1, X_PRE);
    }
    IntDisable(INT_UART3);
    color_get = 0;
    color_show();
    
    for(uint8_t i = 0; i < 3; i++){
        car_move_to(material_x, material_y, Y_PRE);         //走到原料区
        car_adjust_to_center();                             //对中
        take_from_material(i);                              //从原料区夹取物块
        car_move_to(work_x + task1[i] - 2, work_y, X_PRE);  //走到加工区
        car_adjust_to_center();                             //对中
        work_area_adjust_pos(get_take_pos(i+1));
        place_to_work();                                   //放下物块到加工区
    }
    
    //读取成品搬运顺序
    qr_code_change();//打开二维码识别
    car_move_to(code_x, code_y, Y_PRE);
    //读取二维码
    while(1){
        if(qr_code_get){
            qr_code_get = 0;
            qr_code2_show();
            break;
        }
    }
    qr_code_change();//关闭二维码识别
    IntDisable(INT_UART4);

    //1
    car_move_to(work_x + task2[0] -2, work_y, X_PRE);   //走到加工区
    car_adjust_to_center();                             //对中
    
    //从加工区夹取物块
    work_area_adjust_pos(task2[0]);
    take_from_work();
    
    car_move_to(2 + task2[0], 2 + task2[0], X_PRE);     //走到中转点
    car_set_point_dir(CAR_POINT_FORWARD);               //调整方向
    car_move_to(product_x, 2 + task2[0], X_PRE);        //走到成品区
    car_adjust_to_center();                             //对中
    pruduct_area_adjust_pos();
    place_to_product();                                    //放下物块到成品区
    
    //2,3
    for(uint8_t i = 1 ; i <= 2 ; i++){
        car_move_to(task2[i]+2, task2[i-1]+2, X_PRE);   //走到中转点
        car_set_point_dir(CAR_POINT_RIGHT);             //调整方向
        car_move_to(work_x+task2[i]-2, work_y, X_PRE);  //走到加工区
        car_adjust_to_center();                         //对中
        
        //从加工区夹取物块
        work_area_adjust_pos(task2[i]);
        take_from_work();
        
        car_move_to(2+task2[i], 2+task2[i], X_PRE);     //走到中转点
        car_set_point_dir(CAR_POINT_FORWARD);           //调整方向
        car_move_to(product_x, 2+task2[i], X_PRE);      //走到成品区
        car_adjust_to_center();                         //对中
        pruduct_area_adjust_pos();
        place_to_product();                            //放下物块到成品区
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

void pruduct_area_adjust_pos(void)
{
    vx = vy = wv = 25;
    bool forward_pre =0, back_pre = 0;
    car_move(CAR_LEFT);
    while(1){
        if(forward[6] == 1 && back[0] == 1)
            break;
        else if(forward[6] == 1)
            forward_pre = 1;
        else if(back[0] == 1)
            back_pre = 1;
        else
            car_move(CAR_LEFT);
        
        if(forward_pre){
            car_rotate(CAR_RIGHT_ROTATE);
            forward_pre = 0;
        }
        else if(back_pre){
            car_rotate(CAR_LEFT_ROTATE);
            back_pre = 0;
        }
    }
    forward_pre = back_pre = 0;
    while(1){
        if(forward[6] == 0 && back[0] == 0)
            break;
        else if(forward[6] == 0)
            forward_pre = 1;
        else if(back[0] == 0)
            back_pre = 1;
        else
            car_move(CAR_LEFT);
        
        if(forward_pre){
            car_rotate(CAR_RIGHT_ROTATE);
            forward_pre = 0;
        }
        else if(back_pre){
            car_rotate(CAR_LEFT_ROTATE);
            back_pre = 0;
        }
    }
    car_stop();
    vx = vy = wv = 70;
}

void work_area_adjust_pos(uint8_t pos)
{
    vx = vy = wv = 25;
    bool forward_pre = 0, back_pre = 0;
    car_move(CAR_RIGHT);
    
    uint8_t forward_i, back_i;
    switch(pos){
        case 1: forward_i = 2; back_i =  6; break;
        case 2: forward_i = 0; back_i =  6; break;
        case 3: forward_i = 0; back_i =  4; break;
    }
    while(1){
        if(forward[forward_i] == 1 && back[back_i] == 1)
            break;
        else if(forward[forward_i] == 1)
            forward_pre = 1;
        else if(back[back_i] == 1)
            back_pre = 1;
        else
            car_move(CAR_RIGHT);
        
        if(forward_pre){
            car_rotate(CAR_LEFT_ROTATE);
            forward_pre = 0;
        }
        else if(back_pre){
            car_rotate(CAR_RIGHT_ROTATE);
            back_pre = 0;
        }
    }
    car_stop();
    vx = vy = wv = 70;
}
