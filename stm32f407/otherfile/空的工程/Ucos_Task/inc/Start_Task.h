#ifndef __START_TASK_H_
#define __START_TASK_H_

#include "includes.h"

#define START_TASK_STACK_SIZE 128
#define START_TASK_STACK_PRIO 5


extern void Start_Task(void *pdata);
extern OS_STK Start_Task_STK[START_TASK_STACK_SIZE];

#endif


