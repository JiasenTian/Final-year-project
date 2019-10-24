#ifndef _UCOSII_TASK_H_
#define _UCOSII_TASK_H_

#include "stm32f10x.h"

void startup_task(void *p_arg);
void getarmangle_task(void *p_arg);
void changearmposture_task(void *p_arg);
void change_joint1_task(void *p_arg);
void change_joint2_task(void *p_arg);
void change_joint3_task(void *p_arg);
void change_joint4_task(void *p_arg);
void change_joint5_task(void *p_arg);
void change_joint6_task(void *p_arg);

#endif

