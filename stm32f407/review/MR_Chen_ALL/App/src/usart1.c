#include "usart1.h"
uint8 Buffer[MAX_LEN];
uint8 Flag=0,Time_Out;;
uint16 LEN=0;
/********************************************************************************************
*  函 数 名： Usart1_Init
*  描    述:  Usart1串口初始化
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-20
*  注意事项:  无
********************************************************************************************/
void Usart1_Init(void)
{
	
	//波特率分频计算参数
	double USART1DIV;
	uint16 DIV_Man,DIV_Frac;
	
	//I0口初始化
	RCC->AHB1ENR |= 0X01;					//开启PA时钟
	GPIOA->MODER &=~ ((0X03<<18)|(0X03<20)); //PA9,PA10模式清零
	GPIOA->MODER |= ((0X02<<18)|(0X02<<20)); //PA9,PA10选用复用功能
	

	GPIOA->OSPEEDR |= 0X01<<18; 			//PA9输出速度25MHz

	GPIOA->PUPDR |= 0X01<<20; 				//PA10上拉
	
	GPIOA->AFR[1] &=~ ((0X0F<<4)|(0X0F<<8));			
	GPIOA->AFR[1] |= (0X07<<4)|(0X07<<8);	//PA9复用功能选择TX,PA10复用功能选择RX
	
	//USART1配置
	RCC->APB2ENR |= 0X01<<4;				//USART1时钟使能
	USART1->CR1 |=0X03<<2;					//发送接收使能
	
	//波特率自动设置
	USART1DIV = FCK / (8*(2-((USART1->CR1&0X01<<15)>>15))*BAUD_RATE);
	DIV_Man = ((uint16)USART1DIV);
	DIV_Frac = (uint16)((USART1DIV-DIV_Man)*16 + 0.5);//+0.5表示四舍五入
	DIV_Man <<=4 ;
	USART1->BRR = DIV_Man | DIV_Frac;
	
	//USART1->BRR = 0X222<<4 | 0X0E;		//波特率手动设置9600
	
	USART1->CR1 |= (0X01<<5);//发送中断使能，接收中断使能
	NVIC_EnableIRQ(USART1_IRQn); 
	USART1->CR1 |= 0X01<<13;		//USART1使能
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


void USART1_IRQHandler(void)
{
	uint8 Receive_Data;
    if(USART1->SR & (0X01<<5))
    {
		Time_Out = 3;//思考：Time_Out = 3;放在 if(Flag == 0)里面和外面有什么区别
		Receive_Data = USART1->DR;
        if(Flag == 0)
        {
            Buffer[LEN++] = Receive_Data;
        }
    }
}
void Led_Switch(void)
{
    if(Flag)
    {
        Flag = 0;
        Str_Send(Buffer); 
		Byte_Send('\n');
        if(strcmp((const char *)Buffer,"LED_ON") == 0)
        {
            Led_State_Control(0X0F);    
        }
        else if(strcmp((const char *)Buffer,"LED_OFF") == 0)
        {
            Led_State_Control(0X00);       
        }
        memset(Buffer,0,LEN);                           
        LEN =0;   
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

