#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"
   void LED_Init(void);
   void LED_ON(int);
   void LED_OFF(int);
	 void LED_ALL_ON(void);
	 void LED_ALL_OFF(void);
	 void BEEP_Init(void);
   void BEEP_ON(void);
   void BEEP_OFF(void);
#endif
