#ifndef __USART1__H_
#define __USART1__H_
#include "stm32f4xx.h"
#include "type.h"

//以下两个至少有一要加.0,表示浮点数
#define FCK  84000000		//USART1时钟是RCC_APB2:84MHz时钟
#define BAUD_RATE  9600.0		//波特率


extern void Usart1_Init(void);
extern void Usart1_Send(uint8 Msg);
extern void Usart1_Str_Send(uint8 Str[]);
extern uint8 Usart1_Receive(void);

#endif
