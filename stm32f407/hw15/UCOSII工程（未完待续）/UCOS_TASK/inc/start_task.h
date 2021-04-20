#ifndef __START_TASK_
#define __START_TASK_

#include "stm32f4xx.h"
#include "includes.h"
#include "type.h"


#define START_TASK_STACK_SIZE 168
#define START_TASK_STACK_PRIO 6

extern OS_STK  START_TASK_STACK[START_TASK_STACK_SIZE];
extern void Start_Task(void *pdata);


#endif


