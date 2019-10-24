#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"

#define KEY_1   (1<<1)
#define KEY_2   (1<<2)
#define KEY_3   (1<<3)
#define KEY_4   (1<<4)
#define KEY_5   (1<<5)
#define KEY_6   (1<<6)
#define KEY_7   (1<<7)
#define KEY_8   (1<<8)
#define KEY_9   (1<<9)


void KEY_Init(void);
u8 KEY_Scan(void);
#endif
