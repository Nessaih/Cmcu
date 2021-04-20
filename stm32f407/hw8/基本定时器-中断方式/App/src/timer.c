#include "timer.h"

/********************************************************************************************
*  函 数 名： Time6_Configuration
*  描    述:  基本定时器Timer6使用中断配置（中断方式）
*  输入参数： (uint16 Psc,uint16 Arr) 分频值，计数值
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-23
*  注意事项:  分频值，计数值都是为16位
********************************************************************************************/
void Time6_Configuration(uint16 Psc,uint16 Arr)
{	
	uint16 delay=0;
	RCC->APB1ENR |=0X01<<4;//定时器时钟使能
	RCC->APB1RSTR |= (0X01<<4);//复位定时器
	RCC->APB1RSTR &=~ (0X01<<4);//停止复位定时器
	TIM6->CR1 |=0X01<<7;	//使用影子寄存器
	TIM6->CR1 &=~ (0X01<<3);//关闭单脉冲模式
	TIM6->CR1  &=~ (0X01<<2);//更新请求源
	TIM6->CR1  &=~ (0X01<<1);//使能更新
	TIM6->DIER |= 0X01;//中断使能
	TIM6->PSC = Psc;//时钟分频
	TIM6->ARR = Arr;//计数值
	TIM6->CNT = 0X00;//清0计数器
	TIM6->EGR |= 0X01;//生成更新事件
	while(!((TIM6->SR)&0X01)&&delay<1680)//查询标志,10us超时退出
	{
		delay++;
	}
	TIM6->SR &=~ 0X01;//清除标志
	
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(TIM6_DAC_IRQn ,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 
	
	TIM6->CR1 |=0X01<<0;//计数器使能
}

//中断服务函数
void TIM6_DAC_IRQHandler()
{
	if((TIM6->SR)&0X01)
	{
		TIM6->SR &=~ 0X01;//清除标志
		
		
		/*usart1超时接收*/
		if(Time_Out)
		{
			if(--Time_Out==0)
				Flag=1;
		}
	}
}
