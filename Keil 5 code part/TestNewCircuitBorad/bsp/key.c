#include "key.h"
#include "gpio.h"


void KEY_Init(void){
	GPIO_InitTypeDef GPIOC_InitStructure;
	
	GPIO_InitTypeDef GPIOB_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_11 | GPIO_Pin_12;
	
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	
	GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOC, &GPIOC_InitStructure);
	
	GPIO_Init(GPIOB, &GPIOB_InitStructure);
}

u8 KEY_Scan(void){
	u8 key;
	static u8 key_up = 1;
	
	key = GPIO_ReadInputData(GPIOC)&0x0f;
	if(key != 0x0f){
		if(1 == key_up){
			key_up = 0;
			return (~key)&0x0f;
		}
	}
	else {
		key_up = 1;
	}
	return 0;
}
