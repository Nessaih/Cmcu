#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f4xx.h"
#include "type.h"

#define MS_BASE 21000
#define US_BASE 21

extern void SysTick_Init(void);
extern void delay_ms(uint16 nms);
extern void delay_us(uint32 nus);
extern void delay_nms(uint32 nms);

#endif



