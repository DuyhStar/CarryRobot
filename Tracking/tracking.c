#include "tracking.h"
#include "uart_config.h"
#include <stdio.h>
#include "timer_config.h"
#include "car_ctrl.h"
#include "delay.h"

uint8_t  track_addr = 1;

uint8_t  car_point_dir = CAR_POINT_RIGHT;

void tracking_init(void)
{
    UART1_Init(9600);
    tracking_addr_init();
    tracking_select(track_addr); 
    timer0_init(10);        //触发循迹模块采集
    //timer1_init(23);        //切换循迹模块
}

void tracking_addr_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
        ;
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
}

void tracking_select(uint8_t addr)
{
    switch(addr){
        case 1:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            break;
    }
}

void track_inf_print(void)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    
    printf("F:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", forward[i]);
    
    printf("  B:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", back[i]);
    
    printf("  L:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", left[i]);
    
    printf("  R:");
    for(uint8_t i = 0; i < 7; i++)
        printf("%d", right[i]);
    
    printf("\n");
}

void car_trk_line(uint8_t dir)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    extern int16_t vx,vy;
    uint8_t* line_inf;

    switch(dir){
        case CAR_FORWARD: line_inf = forward; break;
        case CAR_BACK:    line_inf = back;    break;
        case CAR_LEFT:    line_inf = left;    break;
        case CAR_RIGHT:   line_inf = right;   break;
    }

    uint8_t center = find_black_line_center(line_inf);

    //PID: p_w1:前后循迹的旋转，p_w2:左右循迹的旋转，p_vx:左右循迹的平移
    int16_t p_wv_fb = -5, p_wv_lr = -4, p_vx_l = 5, p_vx_r = -5;
    int16_t err = center - 6;
    switch(dir){
        case CAR_FORWARD: car_sport(         vx,   0, p_wv_fb*err); break;
        case CAR_BACK:    car_sport(        -vx,   0, p_wv_fb*err); break;
        case CAR_LEFT:    car_sport( p_vx_l*err,  vy, p_wv_lr*err); break;
        case CAR_RIGHT:   car_sport( p_vx_r*err, -vy, p_wv_lr*err); break;
    }
}

void car_go_n_line(uint8_t dir, uint8_t n)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    uint8_t *count_line, line_center[4] = {12,10,8};
    
    switch(dir){
        case CAR_FORWARD: count_line = left;    break;
        case CAR_BACK:    count_line = right;   break;
        case CAR_LEFT:    count_line = back;    break;
        case CAR_RIGHT:   count_line = forward; break;
    }
    
    while(count_line[6]){
        car_move(dir);
    }
    uint8_t count = 0, i = 0, center;
    while(1){
        car_trk_line(dir);
        //计数
        center = find_black_line_center(count_line);
        if((center == line_center[i]) || (center == line_center[i]-1)){
            i++;
        }
        if(i == 3){
            i = 0;
            count++;
            update_XY(dir);
        }
        if(count == n){
            break;
        }
    }
    car_stop();
}

void update_XY(uint8_t dir)
{
    extern int8_t X, Y;
    switch(car_point_dir){
        case CAR_POINT_FORWARD:
            switch(dir) {
                case CAR_FORWARD: Y++; break;
                case CAR_BACK:    Y--; break;
                case CAR_LEFT:    X--; break;
                case CAR_RIGHT:   X++; break;
            }
            break;
        case CAR_POINT_BACK:
            switch(dir) {
                case CAR_FORWARD: Y--; break;
                case CAR_BACK:    Y++; break;
                case CAR_LEFT:    X++; break;
                case CAR_RIGHT:   X--; break;
            }
            break;
        case CAR_POINT_LEFT:
            switch(dir) {
                case CAR_FORWARD: X--; break;
                case CAR_BACK:    X++; break;
                case CAR_LEFT:    Y--; break;
                case CAR_RIGHT:   Y++; break;
            }
            break;
        case CAR_POINT_RIGHT:
            switch(dir) {
                case CAR_FORWARD: X++; break;
                case CAR_BACK:    X--; break;
                case CAR_LEFT:    Y++; break;
                case CAR_RIGHT:   Y--; break;
            }
            break;
    }
}

void car_adjust_to_center(void)
{
    extern uint8_t forward[8], back[8], left[8], right[8];
    uint8_t center_f, center_b, center_l, center_r;
    
    //调节100次
    for(uint8_t i = 0; i < 100; i++){
        //调整前后方向
        while(1){
            center_f = find_black_line_center(forward);
            center_b = 12 - find_black_line_center(back);
            
            if( (center_f == center_b) && (center_f == 6) ){
                car_stop();
                break;
            }
            else{
                if(center_f > center_b)
                    car_rotate(CAR_RIGHT_ROTATE);
                else if(center_f < center_b)
                    car_rotate(CAR_LEFT_ROTATE);
                else{
                    if(center_f > 6)
                        car_move(CAR_RIGHT);
                    else if(center_f < 6)
                        car_move(CAR_LEFT);
                    else
                        ;
                }
            }
        }
        //调整左右方向
        while(1){
            center_l = find_black_line_center(left);
            center_r = 12 - find_black_line_center(right);
            
            if( (center_l == center_r) && (center_l == 6) ){
                car_stop();
                break;
            }
            else{
                if(center_l > center_r)
                    car_rotate(CAR_RIGHT_ROTATE);
                else if(center_l < center_r)
                    car_rotate(CAR_LEFT_ROTATE);
                else{
                    if(center_l > 6)
                        car_move(CAR_FORWARD);
                    else if(center_l < 6)
                        car_move(CAR_BACK);
                    else
                        ;
                }
            }
        }
    }   
}

void car_rotate_90_degree(uint8_t dir)
{
    extern uint8_t forward[8];
    
    car_adjust_to_center();
    car_rotate(dir);
    if(dir == CAR_RIGHT_ROTATE){
        uint8_t i = 6;
        while(1){
            if(forward[i] == 1){
                i--;
            }
            if(i == 3){
                car_stop();
                break;
            }
        }
    }
    else{
        uint8_t i = 0;
        while(1){
            if(forward[i] == 1){
                i++;
            }
            if(i == 3){
                car_stop();
                break;
            }
        }
    }
    car_adjust_to_center();
    
    //更新车头方向
    static uint8_t car_point[4] = {CAR_POINT_FORWARD, CAR_POINT_LEFT, CAR_POINT_BACK, CAR_POINT_RIGHT};
    static int8_t p = 3;
    if(dir == CAR_LEFT_ROTATE){
        p++;
        if(p == 4){
            p = 0;
        }
    }
    else{
        p--;
        if(p == -1){
            p = 3;
        }
    }
    car_point_dir = car_point[p];
}

void car_rotate_n_90_degree(uint8_t dir, uint16_t n)
{
    for(uint8_t i = 0; i < n; i++)
        car_rotate_90_degree(dir);
}

void car_set_point_dir(uint8_t dir)
{
    switch(dir){
        case CAR_POINT_FORWARD:
            switch(car_point_dir){
                case CAR_POINT_FORWARD: car_rotate_n_90_degree(CAR_LEFT_ROTATE, 0);  break;
                case CAR_POINT_BACK:    car_rotate_n_90_degree(CAR_LEFT_ROTATE, 2);  break;
                case CAR_POINT_LEFT:    car_rotate_n_90_degree(CAR_RIGHT_ROTATE, 1); break;
                case CAR_POINT_RIGHT:   car_rotate_n_90_degree(CAR_LEFT_ROTATE, 1);  break;
            }
            break;
        case CAR_POINT_BACK:
            switch(car_point_dir){
                case CAR_POINT_FORWARD: car_rotate_n_90_degree(CAR_LEFT_ROTATE, 2);  break;
                case CAR_POINT_BACK:    car_rotate_n_90_degree(CAR_LEFT_ROTATE, 0);  break;
                case CAR_POINT_LEFT:    car_rotate_n_90_degree(CAR_LEFT_ROTATE, 1);  break;
                case CAR_POINT_RIGHT:   car_rotate_n_90_degree(CAR_RIGHT_ROTATE, 1); break;
            }
            break;
        case CAR_POINT_LEFT:
            switch(car_point_dir){
                case CAR_POINT_FORWARD: car_rotate_n_90_degree(CAR_LEFT_ROTATE, 1);  break;
                case CAR_POINT_BACK:    car_rotate_n_90_degree(CAR_RIGHT_ROTATE, 1); break;
                case CAR_POINT_LEFT:    car_rotate_n_90_degree(CAR_RIGHT_ROTATE, 0); break;
                case CAR_POINT_RIGHT:   car_rotate_n_90_degree(CAR_LEFT_ROTATE, 2);  break;
            }
            break;
        case CAR_POINT_RIGHT:
            switch(car_point_dir){
                case CAR_POINT_FORWARD: car_rotate_n_90_degree(CAR_RIGHT_ROTATE, 1); break;
                case CAR_POINT_BACK:    car_rotate_n_90_degree(CAR_LEFT_ROTATE, 1);  break;
                case CAR_POINT_LEFT:    car_rotate_n_90_degree(CAR_LEFT_ROTATE, 2);  break;
                case CAR_POINT_RIGHT:   car_rotate_n_90_degree(CAR_LEFT_ROTATE, 0);  break;
            }
            break;
    }
}

void car_move_to(uint8_t tar_x, uint8_t tar_y, uint8_t pre)
{
    extern int8_t X, Y;
    int8_t err_x = tar_x - X, err_y = tar_y - Y;
    
    if(pre == X_PRE){
        car_x_move(err_x);
        car_y_move(err_y);
    }
    else if(pre == Y_PRE){
        car_y_move(err_y);
        car_x_move(err_x);
    }
}

void car_x_move(int8_t err)
{
    if(err > 0){
        switch(car_point_dir){
            case CAR_POINT_FORWARD: car_go_n_line(CAR_RIGHT, err);    break;
            case CAR_POINT_BACK:    car_go_n_line(CAR_LEFT, err);     break;
            case CAR_POINT_LEFT:    car_go_n_line(CAR_BACK, err);     break;
            case CAR_POINT_RIGHT:   car_go_n_line(CAR_FORWARD, err);  break;
        }
    }
    else if(err < 0){
        switch(car_point_dir){
            case CAR_POINT_FORWARD: car_go_n_line(CAR_LEFT, -err);    break;
            case CAR_POINT_BACK:    car_go_n_line(CAR_RIGHT, -err);   break;
            case CAR_POINT_LEFT:    car_go_n_line(CAR_FORWARD, -err); break;
            case CAR_POINT_RIGHT:   car_go_n_line(CAR_BACK, -err);    break;
        }
    }
    else{
    }
}
void car_y_move(int8_t err)
{
    if(err > 0){
        switch(car_point_dir){
            case CAR_POINT_FORWARD: car_go_n_line(CAR_FORWARD, err);  break;
            case CAR_POINT_BACK:    car_go_n_line(CAR_BACK, err);     break;
            case CAR_POINT_LEFT:    car_go_n_line(CAR_RIGHT, err);    break;
            case CAR_POINT_RIGHT:   car_go_n_line(CAR_LEFT, err);     break;
        }
    }
    else if(err < 0){
        switch(car_point_dir){
            case CAR_POINT_FORWARD: car_go_n_line(CAR_BACK, -err);    break;
            case CAR_POINT_BACK:    car_go_n_line(CAR_FORWARD, -err); break;
            case CAR_POINT_LEFT:    car_go_n_line(CAR_LEFT, -err);    break;
            case CAR_POINT_RIGHT:   car_go_n_line(CAR_RIGHT, -err);   break;
        }
    }
    else{
    }
}

void car_ready_move(void)
{
    extern uint8_t back[8];
    extern int8_t Y;
    
    car_move(CAR_LEFT);
    while(!back[3])
        ;
    Y++;
    car_go_n_line(CAR_FORWARD, 1);
}   

uint8_t find_black_line_center(uint8_t* line_inf)
{
    uint8_t begin = 0, end = 0;
    for(uint8_t i = 0; i <= 6; i++){
        if(line_inf[i] == 1){
            begin = i;
            break;
        }
    }
    for(int8_t i = 6; i >= 0; i--){
        if(line_inf[i] == 1){
            end = i;
            break;
        }
    }
    //如果没有找到黑线，认为黑线在中间
    if(begin == 0 && end == 0)
        return 6;
    
    uint8_t center = begin*2 + end - begin;
    return center;
}



