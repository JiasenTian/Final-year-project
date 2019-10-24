#include "gpio.h"

void BEEP_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void BEEP_ON(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

void BEEP_OFF(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}

void LED_Init(void){
	GPIO_InitTypeDef GPIOA_InitStructure;
	
	GPIO_InitTypeDef GPIOB_InitStructure;
	
	GPIO_InitTypeDef GPIOC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_8;
	
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;
	
	GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6;
	
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIOB_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIOC_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIOA_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIOB_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIOC_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	GPIO_Init(GPIOB, &GPIOB_InitStructure);
	
	GPIO_Init(GPIOC, &GPIOC_InitStructure);
}

void LED_ON(int LED_NUMBER){
	switch(LED_NUMBER){
		case 1:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		  break;
		case 2:
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		  break;
		case 3:
			GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		  break;
		case 4:
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		  break;
		case 5:
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		  break;
		case 6:
			GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		  break;
		case 7:
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		  break;
		case 8:
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		  break;
		case 9:
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		  break;
	}
}

void LED_OFF(int LED_NUMBER){
	switch(LED_NUMBER){
		case 1:
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
		  break;
		case 2:
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		  break;
		case 3:
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
		  break;
		case 4:
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
		  break;
		case 5:
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
		  break;
		case 6:
			GPIO_SetBits(GPIOC, GPIO_Pin_7);
		  break;
		case 7:
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
		  break;
		case 8:
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
		  break;
		case 9:
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
		  break;
	}
}

void LED_ALL_ON(){
	int i;
	for(i = 1; i <= 9; ++i){
		LED_ON(i);
	}
}

void LED_ALL_OFF(){
	int i;
	for(i = 1; i <= 9; ++i){
		LED_OFF(i);
	}
}
