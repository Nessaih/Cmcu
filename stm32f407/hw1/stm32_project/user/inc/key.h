#ifndef KEY_H
#define KEY_H

#include "stm32f4xx.h"

#define KEY1 (GPIOA->IDR & 1 << 0)



int key_scan(void);
void key_init(void);

#endif



