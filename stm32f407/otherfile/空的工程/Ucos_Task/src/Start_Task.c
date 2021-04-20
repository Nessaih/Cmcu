#include"Start_Task.h"
#include "type.h"
#include "led.h"

OS_STK Start_Task_STK[START_TASK_STACK_SIZE]={0};
void Start_Task(void *pdata)
{
    while(1)
    {
    PFout(6)=0;
    OSTimeDly(200);
    PFout(6)=1;
    OSTimeDly(200);
    }
}

