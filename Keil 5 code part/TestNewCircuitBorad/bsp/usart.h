#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"
#include "misc.h"

void bsp_InitCOM1(u32 baud);
void ComPutChar(USART_TypeDef* USARTx, u8 ch);
void ComPutStr(USART_TypeDef* USARTTx, u8* ptr, u16 len);
void ComPutCStr(USART_TypeDef* USARTx, const char *ptr);
	
#endif
