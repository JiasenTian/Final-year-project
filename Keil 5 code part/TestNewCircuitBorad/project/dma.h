#ifndef _DMA_H_
#define _DMA_H_

#include "stm32f10x.h"
void DMA_Config(uint32_t buffer);
void GPIO_DMA_Config(void);
void COM_DMA_Init(void);
void DMA_getanglefromPC(void);
#endif

