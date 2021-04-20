#ifndef __LED_H_
#define __LED_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "type.h"

#define LED1_ON 	PAout(7)=0 
#define LED1_OFF 	PAout(7)=1 
#define LED1_STATE  PAout(7)

extern void Led_Io_Init(void);


#endif
