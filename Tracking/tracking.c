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
    switch(dir){
        case CAR_FORWARD:
        case CAR_BACK: 
            car_forward_back_trk_line(dir); 
            break;
        case CAR_LEFT: 
            car_left_trk_line();
            break;
        case CAR_RIGHT: 
            car_right_trk_line(); 
            break;
    }
}

void car_forward_back_trk_line(uint8_t dir)
{
    extern uint8_t forward[8],back[8],left[8],right[8];
    uint8_t* line_inf;
    
    switch(dir){
        case CAR_FORWARD: line_inf = forward; break;
        case CAR_BACK:    line_inf = back;    break;
    }
    
    uint8_t center = find_black_line_center(line_inf);
    
    uint8_t range = 2;
    if(center >= 6+range){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else if(center <= 6-range){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else{
        car_move(dir);
    }
}
void car_left_trk_line(void)
{
    extern uint8_t left[8], right[8];
    
    uint8_t left_center  = find_black_line_center(left);
    uint8_t right_center = (12-find_black_line_center(right));
    
    if(compare_in_range(left_center, right_center) > 0){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else if(compare_in_range(left_center, right_center) < 0){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else{
        uint8_t range = 2;
        if(left_center >= 6+range){
            car_move(CAR_FORWARD);
        }
        else if(left_center <= 6-range){
            car_move(CAR_BACK);
        }
        else{
            car_move(CAR_LEFT);
        }
    }
}
void car_right_trk_line(void)
{
    extern uint8_t left[8], right[8];
    
    uint8_t left_center  = (12-find_black_line_center(left));
    uint8_t right_center = find_black_line_center(right);
    
    if(compare_in_range(left_center, right_center) > 0){
        car_rotate(CAR_LEFT_ROTATE);
    }
    else if(compare_in_range(left_center, right_center) < 0){
        car_rotate(CAR_RIGHT_ROTATE);
    }
    else{
        uint8_t range = 2;
        if(right_center >= 6+range){
            car_move(CAR_BACK);
        }
        else if(right_center <= 6-range){
            car_move(CAR_FORWARD);
        }
        else{
            car_move(CAR_RIGHT);
        }
    }
}

int8_t compare_in_range(uint8_t a, uint8_t b)
{
    uint8_t range = 1;
    uint8_t a_min = a-range, a_max = a+range;
    uint8_t b_min = b-range, b_max = b+range;
    
    if(a < b){
        if(a_max < b_min){
            return -1;
        }
        else{
            return 0;
        }
    }
    else if(a > b){
        if(a_min > b_max){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
    }
    return 0;
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
    extern uint16_t w1_speed,w2_speed,w3_speed,w4_speed;
    extern uint8_t forward[8], back[8], left[8], right[8];
    uint8_t center_f, center_b, center_l, center_r;
    
    w1_speed = w2_speed = w3_speed = w4_speed = 20;
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
    w1_speed = w2_speed = w3_speed = w4_speed = ALL_WHEEL_SPEED;   
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

void car_rotate_n_90_degree(uint8_t dir, uint16_t n)
{
    for(uint8_t i = 0; i < n; i++)
        car_rotate_90_degree(dir);
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

void car_go_no_line_no_trk(uint8_t dir, uint8_t n)
{
    car_move(dir);
    
}












