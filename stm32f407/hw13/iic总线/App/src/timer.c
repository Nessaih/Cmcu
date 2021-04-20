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
	GPIOB->MODER &= ~((uint32)0X03<<30);
    GPIOB->MODER |= (uint32)0X02<<30;//复用功能
    GPIOB->OTYPER &= ~(0X01<<15);//推挽输出
	GPIOB->OSPEEDR &=~ ((uint32)0X03<<30);//输出速度复位
	GPIOB->OSPEEDR |=((uint32)0X02<<30);
    GPIOB->PUPDR &= ~((uint32)0X03<<30);//浮空
	GPIOB->AFR[1] &=~((uint32)0X0F<<28);
	GPIOB->AFR[1] |=((uint32)0X09<<28);//复用功能选择
	
	/*TIM12配置*/
	RCC->APB1ENR |= 0X01<<6;//timer12时钟使能
	RCC->APB1RSTR |= (0X01<<6);//复位定时器
	RCC->APB1RSTR &=~ (0X01<<6);//停止复位定时器
	TIM12->CR1 |=0X01<<7;//TIM12_ARR 寄存器进行缓冲
	TIM12->CR1 &=~ ((0X01<<3)|(0X01<<2)|(0X01<<1));//关闭单脉冲模式；更新请求源；使能更新
	TIM12->DIER |=(0X01<<2)|(0X01) ;//开启CC2IE中断使能;更新中断使能
	
	
	TIM12->CCMR1 &=~ (0X03<<8);//CC2通道配置为输出
	TIM12->CCMR1 &=~ (0X07<<12);//ch2输出捕获/比较模式清零
	TIM12->CCMR1 |=0X07<<12;//PWM 模式 2
	TIM12->CCMR1 |=0X01<<11;//输出比较 2 预装载使能
	//TIM12->CCMR1 |=0X01<<10;//输出比较 2 快速使能
	
	TIM12->CCER &=~(0X01<<7);//CC2 通道配置为输出： CC2NP 必须保持清零
	TIM12->CCER &=~(0X01<<5);//OC1 高电平有效(即OC2_REFD到输出端口不取非)
	TIM12->CCER |=(0X01<<4);//ch2使能输出
	
	TIM12->PSC = Psc-1;//分频
	TIM12->ARR =Arr-1;//计数值
	TIM12->CCR2 =10 ;//比较值
	
	TIM12->EGR |=0X01;//生成更新事件
	while(!((TIM12->SR)&0X01)&&delay<1680)//查询标志,10us超时退出
	{
		delay++;
	}
	TIM12->SR &=~ 0X01;//清除更新事件标志
	TIM12->SR &=~ (0X01<<2);//清除CC2IE中断标志
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn ); 

	TIM12->CR1 |=0X01;//计数器使能
}

//中断服务函数
void TIM8_BRK_TIM12_IRQHandler(void)
{
	if((TIM12->SR)&(0X01<<2))
	{
		//printf("TIME12中断");
		TIM12->SR &=~ (0X01<<2);//清除CC2IE中断标志
		if(TIM12->CCR2>1498)
		{
			 Timer12_Flag=0;
		}
		if(TIM12->CCR2<2)
		{
			 Timer12_Flag=1;
		}
		
		if(Timer12_Flag)
		{
			TIM12->CCR2=TIM12->CCR2+1;
		}
		else
		{
			TIM12->CCR2=TIM12->CCR2-1;
		}
	}
	if((TIM12->SR)&(0X01<<0))
	{
		TIM12->SR &=~ (0X01<<0);	
	}
}

/********************************************************************************************
*  函 数 名： Time5_CH1_Capture_Configuration
*  描    述:  基本定时器Timer6使用中断配置（中断方式）
*  输入参数： (uint16 Psc,uint16 Arr) 分频值，计数值
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-24--15:16
*  注意事项:  分频值，计数值都是为16位
********************************************************************************************/
void Time5_CH1_Capture_Configuration(uint16 Psc,uint16 Arr)
{
	uint16 delay=0;
	/*PA0初始化，选择复用功能TIM11_CH1*/
	RCC->AHB1ENR |= 0X01; //PA时钟使能
	GPIOA->MODER &= ~(0X03<<0);
    GPIOA->MODER |= 0X02<<0;//复用功能
    GPIOA->OTYPER &= ~(0X01<<0);//推挽输出
	GPIOA->OSPEEDR &=~ (0X03<<0);//输出速度复位
	GPIOA->OSPEEDR |=(0X02<<0);
    GPIOA->PUPDR &= ~(0X03<<0);//浮空
	GPIOA->AFR[0] &=~(0X0F<<0);
	GPIOA->AFR[0] |=(0X02<<0);//PA0复用功能选择TIM5_CH1
	
	/*TIM5配置*/
	RCC->APB1ENR |= 0X01<<3;//timer5时钟使能
	RCC->APB1RSTR |= (0X01<<3);//复位定时器
	RCC->APB1RSTR &=~ (0X01<<3);//停止复位定时器
	
	TIM5->CR1 &=~ (0X03<<8);//CKD不分频
	TIM5->CR1 |=0X01<<7;//TIM5_ARR 寄存器进行缓冲
	TIM5->CR1 &=~ ((0X03<<5)|(0X01<<4)|(0X01<<3)|(0X01<<2)|(0X01<<1));//边沿对齐模式；增计数模式；关闭单脉冲模式；更新请求源；使能更新
	TIM5->DIER |=(0X01<<1)|(0X01) ;//开启CC1IE中断使能;更新事件中断使能
	TIM5->CCMR1 &=~ (0X03<<0);
	TIM5->CCMR1 |=0X01<<0;// CC1 通道配置为输入，IC1映射到 TI1上。
	TIM5->CCMR1 &=~(0X03<<2);//输入捕获不分频
	TIM5->CCMR1 &=~(0X0F<<4);//无滤波器	
	TIM5->CCER &=~(0X01<<3);
	TIM5->CCER &=~(0X01<<1);//上升沿触发捕获00
	TIM5->CCER |=(0X01<<0);//捕获使能
	
	TIM5->PSC = Psc-1;//分频
	TIM5->ARR =Arr-1;//计数值
	
	TIM5->EGR |=0X01;//生成更新事件
	while(!((TIM5->SR)&0X01)&&delay<1680)//查询标志,10us超时退出
	{
		delay++;
	}
	TIM5->SR &=~ 0X01;//清除更新事件标志
	TIM5->SR &=~ (0X01<<1);//清除CC2IE中断标志
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(TIM5_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(TIM5_IRQn ); 

	TIM5->CR1 |=0X01;//计数器使能
}

//中断服务函数
uint8 Capture_Start_Flag = 0,Capture_End_Flag;
uint32 Overflow_Count=0;//更新事件次数
uint16 Capture_Up_Count,Capture_Down_Count;
uint32 Capture_Sum_Count;
void TIM5_IRQHandler(void)
{
	if((TIM5->SR)&(0X01<<0))
	{
		TIM5->SR &=~ (0X01<<0);	//清除更新事件
		if(Capture_Start_Flag==1)
		{
			Overflow_Count++;
		}
	}
	
	if((TIM5->SR)&(0X01<<1))
	{
		TIM5->SR &=~ (0X01<<1);	//清除捕获标志
		if(Capture_Start_Flag == 0)
		{
			Capture_Up_Count = TIM5->CCR1;
			TIM5->CCER |=0X01<<1;//切换为下降沿捕获
			Capture_Start_Flag = 1;
		}
		else
		{
			Capture_Down_Count = TIM5->CCR1;
			Capture_Sum_Count = Overflow_Count * Capture_ARR - Capture_Up_Count + Capture_Down_Count;
			TIM5->CCER &=~(0X01<<1);//切换为上升沿捕获
			Overflow_Count = 0;
			Capture_Start_Flag = 0;
			Capture_End_Flag =1;
		}
	}
}

