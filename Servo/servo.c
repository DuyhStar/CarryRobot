#include "servo.h"
#include "delay.h"
#include "arm.h"

void servo_ctrl_init(uint16_t* pos)
{
    UART2_Init(115200);
    delay_ms(50);
    for(uint8_t i = 0; i < 6; i++){
        servo_speed_set(0,300);
        servo_speed_set(1,300);
        servo_speed_set(2,300);
        servo_speed_set(3,300);
    }
    servo_position_set(0,pos[0], 0);
    servo_position_set(1,pos[1], 0);
    servo_position_set(2,pos[2], 0);
    servo_position_set(3,pos[3], 0);
    PWM0_67_init();
    fang();
}
void zhua_zi_set(uint16_t us)
{
    uint32_t pulse = PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3);
    pulse = pulse*us/20000;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, pulse);
}

static void UARTSend(uint32_t ui32UARTBase, const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
        UARTCharPut(ui32UARTBase, *pui8Buffer++);
}

//0-1023
void servo_position_set(uint8_t ID,uint16_t position, bool wait_if)
{	
	uint8_t buff[9];
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = ID;
	buff[3] = 0x05;	//length
	buff[4] = 0x03;	//command
	buff[5] = 0x2A;	//register address
	buff[6] = (position & 0xFF00)>>8;
	buff[7] = (position & 0x00FF);
	buff[8] = ~(uint8_t)(buff[2]+buff[3]+buff[4]+buff[5]+buff[6]+buff[7]);
	
	UARTSend(UART2_BASE,buff,9);
    delay_ms(100);
    
    if(wait_if == true){
        uint16_t pos, count1 = 0, count2 = 0;
        while(1){
            servo_position_get(ID, &pos);
            if(pos == 0){
                delay_ms(100);
                count1++;
                if(count1 == 20){
                    break;
                }
                continue;
            }
            int16_t err = position - pos;
            if(err < 10 && err > -10){
                break;
            }
            else{
                count2++;
                if(count2 == 30){
                    break;
                }
                delay_ms(100);
            }
        }
    }
}

void servo_speed_set(uint8_t ID,uint16_t speed)
{
	uint8_t buff[9];
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = ID;
	buff[3] = 0x05;	//length
	buff[4] = 0x03;	//command
	buff[5] = 0x2E;	//register address
	buff[6] = (speed & 0xFF00)>>8;
	buff[7] = (speed & 0x00FF);
	buff[8] = ~(uint8_t)(buff[2]+buff[3]+buff[4]+buff[5]+buff[6]+buff[7]);
	
	UARTSend(UART2_BASE,buff,9);
    delay_ms(50);
}


void servo_position_get(uint8_t ID, uint16_t* pos)
{
    while(UARTCharsAvail(UART2_BASE)){
        UARTCharGet(UART2_BASE);
    }
	uint8_t buff[8];
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = ID;
	buff[3] = 0x04;	//length
	buff[4] = 0x02;	//command
	buff[5] = 0x38;	//register address
	buff[6] = 0x02;
	buff[7] = ~(uint8_t)(buff[2]+buff[3]+buff[4]+buff[5]+buff[6]);
	UARTSend(UART2_BASE, buff, 8);
	
	uint8_t recv_buf[8];
	while(1){
		recv_buf[0] = recv_buf[1];
		recv_buf[1] = UARTCharGet(UART2_BASE);
		if(recv_buf[0] == 0xff && recv_buf[1] == 0xff)
			break;
	}
	for(int i = 2;i<8;i++){
        recv_buf[i] = UARTCharGet(UART2_BASE);
	}	
	
	if(recv_buf[0] == 0xff && recv_buf[1] == 0xff && recv_buf[2] == ID){
		uint8_t crc =~(uint8_t)(recv_buf[2]+recv_buf[3]+recv_buf[4]+recv_buf[5]+recv_buf[6]);
		if(recv_buf[7] == crc){
			uint16_t position = (recv_buf[5] << 8) + recv_buf[6];
			*pos = position;
		}
	}
    else
        pos = 0;
}
