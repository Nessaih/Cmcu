#include "led_task.h"
#include "key.h"
#include "led.h"
#include "key_task.h"
#include "usart1.h"

OS_STK	LED_TASK_STACK[LED_TASK_STACK_SIZE];
void Led_Task(void *pdata)
{
	uint8 i,perr;
	uint8 key_count[5] = {0,0,0,0,0};
	void *pmsg;
	
	pdata = pdata;//防止编译器优化
	while(1)
	{
		pmsg = OSQPend(Press_Key_Event,100,&perr);
		if(perr == OS_ERR_NONE)
		{
			printf("接收到消息!\r\n");
			OSQFlush(Press_Key_Event);//清空消息队列
		}
		else
		{
			printf("未接收到消息!\r\n");	
		}
		for(i = 1;i<5;i++)
		{
			key_count[i] = *((uint8 *)pmsg+i-1);
		}
		switch(key_count[1])//KEY1
		{
			//case 0:Led_State_Control(0x00);break;
			case 1:Led_State_Control(0x01);break;
			case 2:Led_State_Control(0x03);break;
			case 3:Led_State_Control(0x07);break;
			case 4:Led_State_Control(0x0f);break;
			default:break;
		}
//		switch(key_count[2])//KEY2
//		{
//			//case 0:Led_State_Control(0x00);break;
//			case 1:Led_State_Control(0x01);break;
//			case 2:Led_State_Control(0x02);break;
//			case 3:Led_State_Control(0x04);break;
//			case 4:Led_State_Control(0x08);break;
//			default:break;
//		}
		OSTimeDly(20);
	}
}
