#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"
#include "type.h"

#define KEY_OK_IO  (GPIOA->IDR & 1)  //OK¼üµÄ ½Ó¿Ú

#define KEY_LEFT	1
#define KEY_RIGHT	2
#define KEY_DOWN	3
#define KEY_UP		4
#define KEY_CENTRE	5

extern uint8 Key_Value;

void Key_Init(void);
uint8_t Key_Scan(void);



#endif 

