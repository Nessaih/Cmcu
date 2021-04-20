#include "stm32f4xx.h"
#include "includes.h"
#include "os_cpu.h"
#include "type.h"
#include "start_task.h"
#include "key_task.h"
#include "led_task.h"
#include "led.h"
#include "usart1.h"
#include "time.h"
#include "beep.h"
#include "key.h"
/********************************************************************************************
*  函 数 名： SysTick_Handler
*  描    述:  系统滴答中断
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  陈斌
*  完成日期:  2018-06-20
*  注意事项:  时间节拍
********************************************************************************************/
void SysTick_Handler(void)
{	
	OSIntEnter();						//进入中断
	OSTimeTick();       				//调用ucos的时钟服务程序               
	OSIntExit();       	 				//触发任务切换软中断
}

			   
/********************************************************************************************
*  函 数 名： OS_SysTick_init
*  描    述:  系统滴答初始化
*  输入参数： uint32 nms：多久进入一次中断,uint8 SYSCLK：时钟168MHZ 
*  输出参数： 无
*  返 回 值:  无
*  作    者:  陈斌
*  完成日期:  2018-06-20
*  注意事项:  时间节拍
********************************************************************************************/
void OS_SysTick_init(uint32 nms,uint8 SYSCLK)
{
 	SysTick->CTRL&=~(0X01<<2);					//SYSTICK使用外部时钟源	   
	SysTick->CTRL|=0X01<<1;   					//开启SYSTICK中断
	SysTick->LOAD=nms * SYSCLK *1000/8; 	   //每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=0X01<<0;   					//开启SYSTICK  
}


int main(void)
{
    u8 cpu_sr;
	INT8U Err;
	
    OSInit();    //初始化操作系统
	//Beep_Io_Init();
	Key_Io_Init();
	Usart1_Init();
	Led_Io_Init();
	
	
	
	
	/*OS_TICKS_PER_SEC  将一秒分分成多少份   */         
    OS_SysTick_init(1000/OS_TICKS_PER_SEC,168);//时间片配置，设置为5ms
    
    OS_ENTER_CRITICAL();//进入临界区，禁止被中断打断
    
    //创建任务函数，至少创建一个任务
	//Err=OSTaskCreate (Start_Task,NULL,&START_TASK_STACK[START_TASK_STACK_SIZE-1],START_TASK_STACK_PRIO);     
	Err=OSTaskCreate (Key_Task,NULL,&KEY_TASK_STACK[KEY_TASK_STACK_SIZE-1],KEY_TASK_STACK_PRIO); 
	Err=OSTaskCreate (Led_Task,NULL,&LED_TASK_STACK[LED_TASK_STACK_SIZE-1],LED_TASK_STACK_PRIO);         
	//printf("%d\r\n",Err);
	
    OS_EXIT_CRITICAL();// 退出临界区， 允许被中断打断
    
    OSStart();        //开启系统调度
	/* OSStart();后面的代码不会执行*/
}





