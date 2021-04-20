#include "usart.h"
#include "menu.h"
#include "key.h"
uint8 Buffer[MAX_LEN];
uint8 Receive_Flag=0,Time_Out=3;
uint16 LEN=0;
uint8 Bluetooth_Control_Flag = 0;
/********************************************************************************************
*  函 数 名： Time9_Configuration
*  描    述:  基本定时器Timer6使用中断配置（中断方式）
*  输入参数： (uint16 Psc,uint16 Arr) 分频值，计数值,
			  输入(0,0)则默认配置为(10000,100)
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-9
*  注意事项:  时钟源是100MHz，分频值，计数值都是为16位
********************************************************************************************/
void Time9_Configuration(uint16 Psc,uint16 Arr)
{	
	uint16 delay=0;
	if(Psc==0&&Arr==0)
	{
		Psc = 10000;
		Arr = 100; 
		
	}
	RCC->APB2ENR |=0X01<<16;//定时器时钟使能
	RCC->APB2RSTR |= (0X01<<16);//复位定时器
	RCC->APB2RSTR &=~ (0X01<<16);//停止复位定时器
	TIM9->CR1 |=0X01<<7;	//使用影子寄存器
	TIM9->CR1  &=~ (0X01<<2);//更新请求源
	TIM9->CR1  &=~ (0X01<<1);//使能更新
	TIM9->DIER |= 0X01;//更新事件中断使能(上溢中断，用作超时接收)
	TIM9->PSC = Psc-1;//时钟分频
	TIM9->ARR = Arr-1;//计数值
	TIM9->CNT = 0X00;//清0计数器
	TIM9->EGR |= 0X01;//生成更新事件
	while(!((TIM9->SR)&0X01)&&delay<1000)//查询标志,10us超时退出
	{
		delay++;
	}
	if(delay>=1000)
		printf("\nTIM9标志位超时\n");
	TIM9->SR &=~ 0X01;//清除标志
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(TIM1_BRK_TIM9_IRQn ,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn); 
	
	TIM9->CR1 |=0X01<<0;//计数器使能
}
//中断服务函数
void TIM1_BRK_TIM9_IRQHandler()
{
	if(TIM9->SR&0X01)
	{
		TIM9->SR &=~ 0X01;//清除标志
		//LED1_OFF;
		
		/*usart1超时接收*/
		if(Time_Out)
		{
			if(--Time_Out==0)
			{
				Receive_Flag=1;
			}
			
		}
	}
}
/********************************************************************************************
*  函 数 名： Usart1_Init
*  描    述:  Usart1串口初始化
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-9
*  注意事项:  无
********************************************************************************************/
void Usart1_Init(void)
{
	
	//波特率分频计算参数
	double USART1DIV;
	uint16 DIV_Man,DIV_Frac,OVER8;
	
	//I0口初始化
	RCC->AHB1ENR |= 0X01;					//开启PA时钟
	GPIOA->MODER &=~ ((0X03<<18)|(0X03<20)); //PA9,PA10模式清零
	GPIOA->MODER |= ((0X02<<18)|(0X02<<20)); //PA9,PA10选用复用功能
	
	GPIOA->OTYPER &=~ (0x01<<9);			//PA9推挽输出
	GPIOA->OSPEEDR &=~ (0X03<<18); 			//PA9输出速度清零
	GPIOA->OSPEEDR |= 0X01<<18; 			//PA9输出速度25MHz
	GPIOA->PUPDR &=~ (0X03<<18);				//PA9 浮空模式
	
	GPIOA->PUPDR &=~ (0X03<<20); 			//PA10上拉清零
	GPIOA->PUPDR |= 0X01<<20; 				//PA10上拉
	
	GPIOA->AFR[1] &=~ ((0X0F<<4)|(0X0F<<8));			
	GPIOA->AFR[1] |= (0X07<<4)|(0X07<<8);	//PA9复用功能选择TX,PA10复用功能选择RX
	
	//USART1配置
	RCC->APB2ENR |= 0X01<<4;				//USART1时钟使能
	RCC->APB2RSTR |= 0X01<<4;				//USART1复位一次
	RCC->APB2RSTR &=~ (0X01<<4);			//USART1不复位
	
	USART1->CR1 &=~ (0X01<<15|(0X01<<12));//16倍过采样;字长8位
	USART1->CR1 |=0X03<<2;					//发送接收使能
	USART1->CR2 &=~(0X03<<12);				//一个停止位
	
	//波特率自动设置
	OVER8 = (USART1->CR1&0X01<<15)>>15;
	USART1DIV = FCK / (8*(2-OVER8)*BAUD_RATE);
	DIV_Man = (uint16)USART1DIV;
	DIV_Frac = (uint16)((USART1DIV-DIV_Man)*16 + 0.5);//+0.5表示四舍五入
	if(DIV_Frac>15)
	{
		DIV_Man++ ; 
		DIV_Frac -= 16;
	}
	USART1->BRR = (DIV_Man<<4)| DIV_Frac;

	
	USART1->CR1 |= (0X01<<5);//发送中断使能，接收中断使能
	USART1->SR &=~ (0X01<<5);//中断标志位清除
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(USART1_IRQn); 
	USART1->CR1 |= 0X01<<13;		//USART1使能
	Time9_Configuration(0,0);
}

void Byte_Send(uint8 Msg)
{
	uint32 delay = 0;
	while(!(USART1->SR&(0X01<<7))&&(delay<168000))//TXE=0上一次数据没传完,等待1us
	{
		delay++;
	}
	if(USART1->SR&(0X01<<7))//TXE=1表示上一次数据传完
		USART1->DR = Msg;

}

void Str_Send(uint8 Str[])
{
	uint16 i; 
	for(i=0;Str[i]!='\0';i++)
	{
		Byte_Send(Str[i]);
	}
}

uint8 Byte_Receive(void)
{
	uint8 Msg='\0';
	uint32 delay = 0;
	while(!(USART1->SR&(0X01<<5))&&(delay<1680))//RXNE=0接收数据未完成,等待0.01us
	{
		delay++;
	}
	if(USART1->SR&(0X01<<5))//RXNE=1 接收完成
		Msg=USART1->DR ;
	return Msg;

}


//void USART1_IRQHandler(void)
//{
//	uint8 Receive_Data;
//    if(USART1->SR & (0X01<<5))
//    {
//		Time_Out = 3;//思考：Time_Out = 3;放在 if(Receive_Flag == 0)里面和外面有什么区别
//		Receive_Data = USART1->DR;
//        if(Receive_Flag == 0)
//        {
//            Buffer[LEN++] = Receive_Data;
//        }
//    }
//}
void USART1_IRQHandler(void)
{
	uint8 Receive_Data,i;
    if(USART1->SR & (0X01<<5))
    {
		Receive_Data = USART1->DR;
		Buffer[LEN++] = Receive_Data;
		if(Buffer[0]==0xd0&&Buffer[1]==0x01)
		{//打开心率测量
			Bluetooth_Control_Flag = 1;
		}
		else if(Buffer[0]==0xd0&&Buffer[1]==00)
		{//关闭心率测量
			Bluetooth_Control_Flag = 2;
		}
		else if(Buffer[0]==0x90&&Buffer[1]==0x01)
		{
			Bluetooth_Control_Flag = 3;
		}
		else if(Buffer[0]==0x90&&Buffer[1]==0x00)
		{
			Bluetooth_Control_Flag = 4;
		}
		if(LEN>=18)
		{
			LEN = 0;
			memset(Buffer,0,18);
		}
    }
}

void B2S(uint8 *Str)
{
	while(*Str!='\0')
	{
		if(*Str<='Z'&&*Str>='A')
			*Str += 32;
		Str++;
	}
}
void Led_Switch(void)
{
    if(Receive_Flag)
    {
        Receive_Flag = 0;
        Str_Send(Buffer); 
		Byte_Send('\n');
        if(strcmp((const char *)Buffer,"LED_ON") == 0)
        {
			LED1_ON;    
        }
        else if(strcmp((const char *)Buffer,"LED_OFF") == 0)
        {
            LED1_OFF;       
        }
        memset(Buffer,0,LEN);                           
        LEN =0;   
    }
}

#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
	while((USART1->SR & (0X01<<7)) == 0);
		USART1->DR=ch;
    return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}

