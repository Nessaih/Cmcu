#include "beep_task.h"
#include "beep.h"
OS_STK	BEEP_TASK_STACK[BEEP_TASK_STACK_SIZE];
void Beep_Task(void *pdata)
{
	pdata = pdata;//∑¿÷π±‡“Î∆˜”≈ªØ
	while(1)
	{
		Beep_State_Control(1);
		OSTimeDly(200);
	}
}
