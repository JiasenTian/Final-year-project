#include "pwm.h"
#include "usart.h"
#include "key.h"
#include "stm32f10x.h"      
#include "gpio.h"
#include "delay.h"
#include "dma.h"
#include "ucos_ii.h"
#include "armcontrol.h"

#define STARTUP_TASK_STK_SIZE               128
void DMA_Config(uint32_t);
void GPIO_DMA_Config(void);
void COM_DMA_Init(void);
void joints_settle_data(uint8_t* dma_buffer);
void joints_control(void);
uint8_t dma_buffer[36];

int main(void){
	LED_Init();
	BEEP_Init();
	BEEP_OFF();
	GPIO_DMA_Config();
	COM_DMA_Init(); 
	TIM3_PWM_Init(14400, 100);
  TIM2_PWM_Init(14400, 100);
	DMA_Config((uint32_t)&dma_buffer);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	LED_ALL_OFF();
	LED_ALL_ON();
	delay_ms(100);
	LED_ALL_OFF();
	  TIM_SetCompare1(TIM2, 1080); // paws PA0 TIM2 Channel 1  No.1 Line joint1
 	  TIM_SetCompare2(TIM2, 1080); // PA1 TIM2 Channel 2       No.3 Line joint2
	  TIM_SetCompare3(TIM2, 1080); // PA2 Tim2 Channel 3       No.2 Line joint3
	  TIM_SetCompare4(TIM2, 1080); // PA3 Tim2 Channel 4       No.4 Line joint4      
	  TIM_SetCompare1(TIM3, 1080); // PA6 Tim2 Channel 1       No.5 Line joint5
    TIM_SetCompare2(TIM3, 1080); // PA7 Tim2 Channel 2       No.6 Line joint6
	
	while(1){
		/*int i;
		for(i = 1080; i >= 720; i--){
			TIM_SetCompare2(TIM3, i);
			delay_ms(5);
		}
		for(i = 720; i <= 1080; i++){
			TIM_SetCompare2 (TIM3, i);
			delay_ms(5);
		}*/
	  
		if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET){ //test angle
			  LED_ALL_ON();                            // 090090090090090090090090090090090095
			  joints_settle_data(dma_buffer);
			  joints_control();
			  //LED_ALL_OFF(); 
			  DMA_ClearFlag(DMA1_FLAG_TC5);		
		}
	}
	return 0; 
}
