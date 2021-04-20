#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f4xx.h"
#include "type.h"
#include "usart1.h"
#include "led.h"
extern uint8 Timer12_Flag;
extern void Time6_Configuration(uint16 Psc,uint16 Arr);
extern void Time12_Configuration(uint16 Psc,uint16 Arr);
#endif



