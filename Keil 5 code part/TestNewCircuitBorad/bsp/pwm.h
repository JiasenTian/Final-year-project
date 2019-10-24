#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void TIM3_PWM_Init(u16 arr, u16 psc);
void TIM2_PWM_Init(u16 arr, u16 psc);
void joint1_set_PWM(uint16_t signal);
void joint2_set_PWM(uint16_t signal);
void joint3_set_PWM(uint16_t signal);
void joint4_set_PWM(uint16_t signal);
void joint5_set_PWM(uint16_t signal);
void joint6_set_PWM(uint16_t signal);

#endif
