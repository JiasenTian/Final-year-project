#include "armcontrol.h"
#include "pwm.h"
#include "delay.h"
#include "gpio.h"

double base_ms = 1000.0; 
int old_angle[6];
int old_PWM[6];
int new_angle[6];
int new_PWM[6];
int change[6];
int changerate[6];


void joints_settle_data(uint8_t* dma_buffer){
	int i;
	int cont = 0;
	for(i = 0; i < 36; i += 6){
		if(old_angle[cont] == getangle(dma_buffer, i))
		  old_angle[cont] = getangle(dma_buffer, i);
		if(old_angle[0] == 90)
			LED_ON(2);
		  else
			LED_OFF(2);
		old_PWM[cont] = 360 + old_angle[cont] * 8;
		new_angle[cont] = getangle(dma_buffer, i + 3);
		if(new_angle[0] == 95)
			LED_ON(3);
		else
			LED_OFF(3);
		new_PWM[cont] = 360 + new_angle[cont] * 8;
		change[cont] = getchangeangle(old_PWM[cont], new_PWM[cont]);
		changerate[cont] = change[cont]/8;
		old_angle[cont] = new_angle[cont];
		cont++;
		
	}
}

void joints_control(){
	 int i;
	 for(i = 1; i <= 8; ++i){
		 joint1_change_control(old_PWM[0] + changerate[0]);
		 joint2_change_control(old_PWM[1] + changerate[1]);
		 joint3_change_control(old_PWM[2] + changerate[2]);
		 joint4_change_control(old_PWM[3] + changerate[3]);
		 joint5_change_control(old_PWM[4] + changerate[4]);
		 joint6_change_control(old_PWM[5] + changerate[5]);
		 old_PWM[0] += changerate[0];
		 old_PWM[1] += changerate[1];
		 old_PWM[2] += changerate[2];
		 old_PWM[3] += changerate[3];
		 old_PWM[4] += changerate[4];
		 old_PWM[5] += changerate[5];
		 delay_ms(100);
	 }
}

void joints_change_speed(double base){
	 base_ms = base;
}

void joints_reset_action(int pre_angle[6]);

int getchangeangle(int old_PWM, int new_PWM){
	return old_PWM - new_PWM;
}

int discern_number(uint8_t* dma_buffer, int i){
	if(*(dma_buffer + i) == '1')
		return 1;
	else if(*(dma_buffer + i) == '2')
		return 2;
	else if(*(dma_buffer + i) == '3')
		return 3;
	else if(*(dma_buffer + i) == '4')
		return 4;
	else if(*(dma_buffer + i) == '5')
		return 5;
	else if(*(dma_buffer + i) == '6')
		return 6;
	else if(*(dma_buffer + i) == '7')
		return 7;
	else if(*(dma_buffer + i) == '8')
		return 8;
	else if(*(dma_buffer + i) == '9')
		return 9;
	else
		return 0;
}

int getangle(uint8_t* dma_buffer, int i){
	int temp = 0;
	temp = 100 * discern_number(dma_buffer, i) + 10 * discern_number(dma_buffer, i + 1) + discern_number(dma_buffer, i + 2 );
	return temp;
}

void joint1_change_control(int signal){
	joint1_set_PWM((uint16_t) signal);
}

void joint2_change_control(int signal){
	joint2_set_PWM((uint16_t) signal);
}
void joint3_change_control(int signal){
	joint3_set_PWM((uint16_t) signal);
}

void joint4_change_control(int signal){
	joint4_set_PWM((uint16_t) signal);
}

void joint5_change_control(int signal){
	joint5_set_PWM((uint16_t) signal);
}

void joint6_change_control(int signal){
	joint6_set_PWM((uint16_t) signal);
}
