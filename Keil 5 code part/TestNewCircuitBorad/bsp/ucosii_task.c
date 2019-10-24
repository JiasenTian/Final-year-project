#include "ucosii_task.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"
#include "ucos_ii.h"
#include "dma.h"
#include "stdio.h"

void DMA_Config(uint32_t);
void GPIO_DMA_Config(void);
void COM_DMA_Init(void);

#define GETARMANGLE_TASK_STK_SIZE           256
#define CHANGEARMPOSTURE_TASK_STK_SIZE      256
#define CHANGEJOINT1_TASK_STK_SIZE          256
#define CHANGEJOINT2_TASK_STK_SIZE          256
#define CHANGEJOINT3_TASK_STK_SIZE          256
#define CHANGEJOINT4_TASK_STK_SIZE          256
#define CHANGEJOINT5_TASK_STK_SIZE          256
#define CHANGEJOINT6_TASK_STK_SIZE          256


OS_EVENT *com_sem_finish;
OS_EVENT *com_mbox_angle;
OS_EVENT *com_sem_flag;
OS_EVENT *joint1_mbox;
OS_EVENT *joint2_mbox;
OS_EVENT *joint3_mbox;
OS_EVENT *joint4_mbox;
OS_EVENT *joint5_mbox;
OS_EVENT *joint6_mbox;

__align(8) static OS_STK getarmangle_task_stk[GETARMANGLE_TASK_STK_SIZE];
__align(8) static OS_STK changearmposture_task_stk[CHANGEARMPOSTURE_TASK_STK_SIZE];
__align(8) static OS_STK changejoint1_task_stk[CHANGEJOINT1_TASK_STK_SIZE];
__align(8) static OS_STK changejoint2_task_stk[CHANGEJOINT2_TASK_STK_SIZE];
__align(8) static OS_STK changejoint3_task_stk[CHANGEJOINT3_TASK_STK_SIZE];
__align(8) static OS_STK changejoint4_task_stk[CHANGEJOINT4_TASK_STK_SIZE];
__align(8) static OS_STK changejoint5_task_stk[CHANGEJOINT5_TASK_STK_SIZE];
__align(8) static OS_STK changejoint6_task_stk[CHANGEJOINT6_TASK_STK_SIZE];


extern uint8_t dma_buffer[18];
uint8_t jointstatement[6];
	 
void startup_task(void *p_arg){
	/* int i;
	 com_sem_finish = OSSemCreate(0);
	 com_sem_flag = OSSemCreate(0);
	 DMA_Config((uint32_t)&dma_buffer);
	 GPIO_DMA_Config();
	 COM_DMA_Init();
	 com_mbox_angle = OSMboxCreate((void *)0);
	 joint1_mbox = OSMboxCreate((void *)0);
	 joint2_mbox = OSMboxCreate((void *)0);
	 joint3_mbox = OSMboxCreate((void *)0);
	 joint4_mbox = OSMboxCreate((void *)0);
	 joint5_mbox = OSMboxCreate((void *)0);
	 joint6_mbox = OSMboxCreate((void *)0);
	
	 for(i = 0; i < 6; ++i){
				jointstatement[i] = 1;
	 }
	 */
	 LED_Init();
	
	//OSTaskCreate(getarmangle_task, (void *)0, &getarmangle_task_stk[GETARMANGLE_TASK_STK_SIZE - 1], 15);
	//OSTaskCreate(changearmposture_task, (void *)0, &changearmposture_task_stk[CHANGEARMPOSTURE_TASK_STK_SIZE - 1], 12);
	
	 while(1){
		 LED_ON(9);
		 delay_ms(1000);
		 //OSTimeDlyHMSM(0,0,1,0);
		 LED_OFF(9);
	 }
}

void getarmangle_task(void *p_arg){
	/*int temp = 1;
	int i;
	GPIO_DMA_Config();
  COM_DMA_Init();
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
  DMA_Config((uint32_t)(&dma_buffer));
	
	for(i = 0; i < 6; ++i){
				jointstatement[i] = 1;
	}*/
	
	OSTaskCreate(change_joint1_task, (void *)0, &changejoint1_task_stk[CHANGEJOINT1_TASK_STK_SIZE - 1], 10);
	OSTaskCreate(change_joint2_task, (void *)0, &changejoint2_task_stk[CHANGEJOINT2_TASK_STK_SIZE - 1], 9);
	OSTaskCreate(change_joint3_task, (void *)0, &changejoint3_task_stk[CHANGEJOINT3_TASK_STK_SIZE - 1], 8);
	OSTaskCreate(change_joint4_task, (void *)0, &changejoint4_task_stk[CHANGEJOINT4_TASK_STK_SIZE - 1], 7);
	OSTaskCreate(change_joint5_task, (void *)0, &changejoint5_task_stk[CHANGEJOINT5_TASK_STK_SIZE - 1], 6);
	OSTaskCreate(change_joint6_task, (void *)0, &changejoint6_task_stk[CHANGEJOINT6_TASK_STK_SIZE - 1], 5);
	 
	while(1){
		 LED_ON(8);
		 delay_ms(1000);
		 LED_OFF(8);
		/*temp = 1;
		for(i = 0; i < 6; ++i){
			if(jointstatement[i]==0){
				temp = 0;
				break;
			}
		}
		if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET && temp == 1){
			for(i = 0; i < 6; ++i){
				jointstatement[i] = 0;
		  }
		  OSMboxPost(com_mbox_angle, (void*)&dma_buffer);
			DMA_ClearFlag(DMA1_FLAG_TC5);			
		}
		OSTimeDlyHMSM(0,0,0,50);*/
	}
}

void changearmposture_task(void *p_arg){
	/*uint8_t *temp;
	INT8U err;
	*/
	while(1){
		 LED_ON(7);
		 delay_ms(100);
		 LED_OFF(7);
		/*temp = OSMboxPend(com_mbox_angle, 0, &err);
		if(err == OS_NO_ERR){
		OSMboxPost(joint1_mbox, (void *)(temp));
		OSMboxPost(joint2_mbox, (void *)(temp + 1));
		OSMboxPost(joint3_mbox, (void *)(temp + 2));
		OSMboxPost(joint4_mbox, (void *)(temp + 3));
		OSMboxPost(joint5_mbox, (void *)(temp + 4));
		OSMboxPost(joint6_mbox, (void *)(temp + 5));
		}*/
	}
}

void change_joint1_task(void *p_arg){
	uint8_t *tempangle;
	uint8_t err;
  while(1){
		/*tempangle = OSMboxPend(joint1_mbox, 0, &err);
	//	if(*tempangle == '1'){
	  LED_ON(2);
	  OSTimeDlyHMSM(0,0,1,0);
	  //LED_OFF(2);
	//}
		jointstatement[0] = 1;
	*/
		 LED_ON(6);
		delay_ms(100);
		 LED_OFF(6);
	}
}

void change_joint2_task(void *p_arg){
  uint8_t *tempangle;
	uint8_t err;
	while(1){
		/*tempangle = OSMboxPend(joint2_mbox, 0, &err);
		//if(*tempangle == '2'){
	  LED_ON(3);
	  //OSTimeDlyHMSM(0,0,1,0);
	  //LED_OFF(3);
		//}
		jointstatement[1] = 1;*/
		LED_ON(5);
		delay_ms(100);
		LED_OFF(5);
	}
}

void change_joint3_task(void *p_arg){
	uint8_t *tempangle;
	uint8_t err;
	while(1){
		/*tempangle = OSMboxPend(joint3_mbox, 0, &err);
		//if(*tempangle == '3'){
	  LED_ON(4);
	  //OSTimeDlyHMSM(0,0,1,0);
	 // LED_OFF(4);
	//	}
		jointstatement[2] = 1;*/
		LED_ON(4);
		delay_ms(100);
		LED_OFF(4);
	}
}

void change_joint4_task(void *p_arg){
	uint8_t *tempangle;
	uint8_t err;
	while(1){
		/*tempangle = OSMboxPend(joint4_mbox, 0, &err);
		//if(*tempangle == '4'){
	  LED_ON(5);
	 // OSTimeDlyHMSM(0,0,1,0);
	 // LED_OFF(5);
		//}
		jointstatement[3] = 1;*/
		LED_ON(3);
		delay_ms(100);
		LED_OFF(3);
	}
}

void change_joint5_task(void *p_arg){
	uint8_t *tempangle;
	uint8_t err;
	while(1){
		/*tempangle = OSMboxPend(joint5_mbox, 0, &err);
		//if(*tempangle == '5'){
	  LED_ON(6);
	 // OSTimeDlyHMSM(0,0,1,0);
	  //LED_OFF(6);
		jointstatement[4] = 1;
	 // }*/
		LED_ON(2);
		delay_ms(100);
		LED_OFF(2);
  }
}

void change_joint6_task(void *p_arg){
	uint8_t *tempangle;
	uint8_t err;
	while(1){
		LED_ON(1);
		delay_ms(100);
		LED_OFF(1);
		/*tempangle = OSMboxPend(joint6_mbox, 0, &err);
		//if(*tempangle == '6'){
	  LED_ON(7);
		delay_ms(100);
		LED_OFF(7);
	 // OSTimeDlyHMSM(0,0,1,0);
	 // LED_OFF(7);
		jointstatement[5] = 1;
	//	}*/
  }
}
