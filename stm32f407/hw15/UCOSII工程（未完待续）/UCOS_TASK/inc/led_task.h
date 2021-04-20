#ifndef __LED_TASK_
#define __LED_TASK_

#include "stm32f4xx.h"
#include "includes.h"
#include "type.h"


#define LED_TASK_STACK_SIZE 168
#define LED_TASK_STACK_PRIO 6

extern OS_STK  LED_TASK_STACK[LED_TASK_STACK_SIZE];
extern void Led_Task(void *pdata);


#endif


