#ifndef __EXTI_H_
#define __EXTI_H_

#include "stm32f4xx.h"
#include "led.h"

extern void EXTI_Configration(void);
extern void EXTI0_IRQHandler(void);
extern void EXTI2_IRQHandler(void);
extern void EXTI3_IRQHandler(void);
extern void EXTI4_IRQHandler(void);

#endif

