#ifndef _ARMCONTROL_H_
#define _ARMCONTROL_H_

#include "stm32f10x.h"

void joints_change_control(int pre_angle[6], int ex_angle[6], long long);
void joints_change_speed(long long);
void joints_reset_action(int pre_angle[6]);
	
#endif
