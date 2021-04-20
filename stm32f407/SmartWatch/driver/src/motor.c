#include "motor.h"

void Motor_Init(void)
{	
	uint16 delay=0;
	//PB10端口复用
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~ (0X03<<2);
	GPIOB->MODER |= (0X02<<20);
	GPIOB->PUPDR &=~ (0X03<<20);
	GPIOB->AFR[1] &=~ (0X0F<<8);
	GPIOB->AFR[1] |= (0X01<<8);
	//TIM2_CH3PWM输出
	RCC->APB1ENR |= 0X01<<0;//timer2时钟使能
	TIM2->CR1 |=0X01<<7;//TIM2_ARR 寄存器进行缓冲
	TIM2->CR1 &=~ ((0X01<<3)|(0X01<<2)|(0X01<<1));//关闭单脉冲模式；更新请求源；使能更新
	//TIM2->DIER |=(0X01<<3);//开启CC3IE中断使能;
	TIM2->DIER |=(0X01);//更新事件中断使能;
	TIM2->CCMR2 &=~ (0X03<<0);//CC3通道配置为输出
	TIM2->CCMR2 &=~ (0X07<<4);//ch3输出捕获/比较模式清零
	TIM2->CCMR2 |=0X07<<4;//PWM 模式 2
	TIM2->CCMR2 |=0X01<<3;//输出比较 3 预装载使能

	
	TIM2->CCER &=~(0X01<<11);//CC2 通道配置为输出： CC2NP 必须保持清零
	TIM2->CCER &=~(0X01<<9);//OCx高电平有效(即OCx_REFD到输出端口不取非)
	TIM2->CCER |=(0X01<<8);//CC3E使能输出
	
	TIM2->PSC = 100;//分频
	TIM2->ARR =1500;//计数值
	TIM2->CCR3 =1400 ;//比较值
	TIM2->EGR |=0X01;//生成更新事件
	while(!((TIM2->SR)&0X01)&&delay<1680)//查询标志,10us超时退出
	{
		delay++;
	}
	TIM2->SR &=~ 0X01;//清除更新事件标志
	//TIM2->SR &=~ (0X01<<3);//清除CC3IE中断标志
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(TIM2_IRQn ); 
	TIM2->CR1 |=0X01;//计数器使能
}
void TIM2_IRQHandler(void)
{
	/*static uint8 Timer2_Flag =1;
	if((TIM2->SR)&(0X01<<3))
	{
		//printf("TIME12中断");
		TIM2->SR &=~ (0X01<<3);//清除CC2IE中断标志
		if(TIM2->CCR3>1498)
		{
			 Timer2_Flag=0;
		}
		if(TIM2->CCR3<2)
		{
			 Timer2_Flag=1;
		}
		
		if(Timer2_Flag)
		{
			TIM2->CCR3 += 1;
		}
		else
		{
			TIM2->CCR3 -= 1;
		}
	}*/
	if((TIM2->SR)&(0X01<<0))
	{
		TIM2->SR &=~ (0X01<<0);	
	}
}
void Motor_Speed(uint8 speed_mode)
{
	switch(speed_mode)
	{
		case 0://关闭
		{
			TIM2->CCER &=~ (0X01<<8);//CC3E禁止输出
			TIM2->CR1 &=~ 0X01;//计数器关闭
		}break;
		case 1://慢速
		{
			TIM2->CCR3 = 1150;
		}break;
		case 2://快速
		{
			TIM2->CCR3 = 800;
		}break;
		case 3://高速
		{
			TIM2->CCR3 = 300;
		}break;
		default:break;
	}
}
