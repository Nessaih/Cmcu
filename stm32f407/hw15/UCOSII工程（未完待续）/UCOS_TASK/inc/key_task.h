#ifndef __KEY_TASK_
#define __KEY_TASK_

#include "stm32f4xx.h"
#include "includes.h"
#include "type.h"


#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_STACK_PRIO 8

extern OS_STK  KEY_TASK_STACK[KEY_TASK_STACK_SIZE];

extern OS_EVENT *Press_Key_Event;

extern void Key_Task(void *pdata);

#endif


