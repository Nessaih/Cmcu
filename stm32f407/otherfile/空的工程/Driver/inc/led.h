#ifndef __LED_H_
#define __LED_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"

extern void Led_Io_Init(void);
extern void Led_state_control(uint8 Led_state);

#endif
