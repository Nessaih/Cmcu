#ifndef __BEEP_TASK_
#define __BEEP_TASK_

#include "stm32f4xx.h"
#include "includes.h"
#include "type.h"


#define BEEP_TASK_STACK_SIZE 128
#define BEEP_TASK_STACK_PRIO 7

extern OS_STK  BEEP_TASK_STACK[BEEP_TASK_STACK_SIZE];
extern void BEEP_Task(void *pdata);


#endif


