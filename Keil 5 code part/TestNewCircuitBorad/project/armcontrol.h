#ifndef _ARMCONTROL_H_
#define _ARMCONTROL_H_

#include "stm32f10x.h"

void joints_control(void);
void joints_settle_data(uint8_t* dma_buffer);
void joints_change_speed(double base);
void joints_reset_action(int pre_angle[6]);
int getchangeangle(int old_angle, int new_angle);
int getangle(uint8_t dma_buffer[36], int i);
int discern_number(uint8_t dma_buffer[36], int i);
void joint1_change_control(int signal);
void joint2_change_control(int signal);
void joint3_change_control(int signal);
void joint4_change_control(int signal);
void joint5_change_control(int signal);
void joint6_change_control(int signal);
void changeFormation(uint8_t* dma_buffer);

#endif
