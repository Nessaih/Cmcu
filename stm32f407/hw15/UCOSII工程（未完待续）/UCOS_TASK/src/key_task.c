#include "key_task.h"
#include "key.h"
#include "usart1.h"

OS_STK	KEY_TASK_STACK[KEY_TASK_STACK_SIZE];
OS_EVENT *Press_Key_Event;

void Key_Task(void *pdata)
{
	uint8 i;
	uint8 key_count[5] = {0,0,0,0,0};//key_count[0]存哪一个按键按下，其他四个存每个按键累计被按的次数（0-4）
	pdata = pdata;//防止编译器优化
	Press_Key_Event = OSQCreate ((void *)&key_count[1],4);//创建消息队列
	if(Press_Key_Event == NULL)
	{
		printf("创建消息队列失败!\r\n");
	}
	else
	{
		printf("创建消息队列成功!\r\n");	
	}
	
	while(1)
	{
		key_count[0] = Key_Scan();
		switch(key_count[0])
		{
			case 1:key_count[1]++;break;
			case 2:key_count[2]++;break;
			case 3:key_count[3]++;break;
			case 4:key_count[4]++;break;
			default:break;
		}
		for(i = 1;i<5;i++)
		{
			if(key_count[i]>4)
			{
				key_count[i] = 1;
			}
		}
		
		OSQPost (Press_Key_Event,(void *)&key_count[1]);
		
		OSTimeDly(40);//200ms扫描一次按键
	}
}
