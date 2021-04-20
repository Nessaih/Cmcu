#ifndef __USART__H_
#define __USART__H_
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "type.h"

//以下两个至少有一要加.0,表示浮点数
#define FCK  100000000		//USART1时钟是RCC_APB2:84MHz时钟
#define BAUD_RATE  9600.0		//波特率
#define MAX_LEN		256

extern uint16 LEN;
extern uint8 Buffer[MAX_LEN];
extern uint8 Receive_Flag,Time_Out;
extern uint8 Bluetooth_Control_Flag;
extern void Time9_Configuration(uint16 Psc,uint16 Arr);
extern void Usart1_Init(void);
extern void Byte_Send(uint8 Msg);
extern void Str_Send(uint8 *Str);
extern uint8 Byte_Receive(void);
extern void Str_Receive(void);
extern void Led_Switch(void);
extern void B2S(uint8 *Str);

#endif
