#include "timer.h"
uint8 Timer12_Flag =1;
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

/********************************************************************************************
*  函 数 名： Time12_Configuration
*  描    述:  基本定时器Timer6使用中断配置（中断方式）
*  输入参数： (uint16 Psc,uint16 Arr) 分频值，计数值
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-23--19:16
*  注意事项:  分频值，计数值都是为16位
********************************************************************************************/
void Time12_Configuration(uint16 Psc,uint16 Arr)
{
	uint16 delay=0;
	/*PB15初始化，选择复用功能TIM12_CH2*/
	RCC->AHB1ENR |= 0X01<<1; 
	GPIOB->MODER &= ~(0X03UL<<30);
    GPIOB->MODER |= 0X02UL<<30;//复用功能
    GPIOB->OTYPER &= ~(0X01<<15);//推挽输出
	GPIOB->OSPEEDR &=~ (0X03UL<<30);//输出速度复位
	GPIOB->OSPEEDR |=(0X02UL<<30);
    GPIOB->PUPDR &= ~(0X03UL<<30);//浮空
	GPIOB->AFR[1] &=~(0X0FUL<<28);
	GPIOB->AFR[1] |=(0X09UL<<28);//复用功能选择
	
	/*TIM12配置*/
	RCC->APB1ENR |= 0X01<<6;//timer12时钟使能
	RCC->APB1RSTR |= (0X01<<6);//复位定时器
	RCC->APB1RSTR &=~ (0X01<<6);//停止复位定时器
	TIM12->CR1 |=0X01<<7;//TIM12_ARR 寄存器进行缓冲
	TIM12->CR1 &=~ ((0X01<<3)|(0X01<<2)|(0X01<<1));//关闭单脉冲模式；更新请求源；使能更新
	TIM12->DIER &=~(0X01<<2)|(0X01) ;//开启CC2IE中断使能关闭;更新中断使能关闭
	
	
	TIM12->CCMR1 &=~ (0X03<<8);//CC2通道配置为输出
	TIM12->CCMR1 &=~ (0X07<<12);//ch2输出捕获/比较模式清零
	TIM12->CCMR1 |=0X07<<12;//PWM 模式 2
	TIM12->CCMR1 |=0X01<<11;//输出比较 2 预装载使能
	
	TIM12->CCER &=~(0X01<<7);//CC2 通道配置为输出： CC2NP 必须保持清零
	TIM12->CCER &=~(0X01<<5);//OC1 高电平有效(即OC2_REFD到输出端口不取非)
	TIM12->CCER |=(0X01<<4);//ch2使能输出
	
	TIM12->PSC = Psc-1;//分频
	TIM12->ARR =Arr-1;//计数值
	TIM12->CCR2 =500 ;//比较值
	
	TIM12->EGR |=0X01;//生成更新事件
	while(!((TIM12->SR)&0X01)&&delay<1680)//查询标志,10us超时退出
	{
		delay++;
	}
	TIM12->SR &=~ 0X01;//清除更新事件标志
	TIM12->SR &=~ (0X01<<2);//清除CC2IE中断标志
	
	TIM12->CR1 |=0X01;//计数器使能
}

