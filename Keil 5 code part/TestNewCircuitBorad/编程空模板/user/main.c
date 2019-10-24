#include "stm32f10x.h"

int main(void)
{
	
	uint32_t a;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_11);
	
	
	while(1)
	{
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		for(a=0;a<990000;a++);
	
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		for(a=0;a<990000;a++);
		
//		GPIO_ResetBits(GPIOC, GPIO_Pin_11);
//			for(a=0;a<1000;a++);
//	
//		GPIO_SetBits(GPIOC, GPIO_Pin_11);
//for(a=0;a<1000;a++);
			
	}
}

