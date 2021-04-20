#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

void led_init(void);
void LED_OFF(void);
void LED_ON(void);

#define LED1_ON  	GPIOF->BSRRH = 1 << 6
#define LED1_OFF  GPIOF->BSRRL = 1 << 6
#define LED2_ON 	GPIOF->BSRRH = 1 << 9
#define LED2_OFF  GPIOF->BSRRL = 1 << 9
#define LED3_ON  	GPIOF->BSRRH = 1 << 10
#define LED3_OFF  GPIOF->BSRRL = 1 << 10
#define LED4_ON  	GPIOC->BSRRH = 1 << 0
#define LED4_OFF  GPIOC->BSRRL = 1 << 0
#define LED1_TURN GPIOF->ODR ^= 1 << 6



#endif



