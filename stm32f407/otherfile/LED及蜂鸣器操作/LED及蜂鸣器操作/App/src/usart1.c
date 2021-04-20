#include "usart1.h"
uint8 Buffer[MAX_LEN];
uint8 Flag=0;
uint16 LEN=0;
void Usart1_Init(void)
{
	
	//波特率分频计算参数
	float USART1DIV;
	uint16 DIV_Man,DIV_Frac;
	
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
	USART1DIV = FCK / (8*(2-((USART1->CR1&0X01<<15)>>15))*BAUD_RATE);
	DIV_Man = ((uint16)USART1DIV)<<4 ;
	DIV_Frac = (uint16)((USART1DIV-DIV_Man/16)*16 + 0.5);//+0.5表示四舍五入
	USART1->BRR = DIV_Man | DIV_Frac;
	
	//USART1->BRR = 0X222<<4 | 0X0E;		//波特率手动设置9600
	
	USART1->CR1 |= (0X01<<5);//发送中断使能，接收中断使能
	USART1->SR &=~ (0X01<<5);//中断标志位清除
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
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

//void Str_Receive(void)          
//{
//    uint8 Receive_Data;
//    if(USART1->SR & (0X01<<5))
//    {
//        //Receive_Data = Byte_Receive();
//		Receive_Data = USART1->DR;
//        if((Receive_Data != '$') && (Flag == 0))
//        {
//            Buffer[LEN++] = Receive_Data;
//        }
//        else
//        {
//            Flag = 1; //表示接受完成    
//        }  
//    }  
//}
void USART1_IRQHandler(void)
{
	uint8 Receive_Data;
    if(USART1->SR & (0X01<<5))
    {
        //Receive_Data = Byte_Receive();
		Receive_Data = USART1->DR;
        if((Receive_Data != '$') && (Flag == 0))
        {
            Buffer[LEN++] = Receive_Data;
        }
        else
        {
            Flag = 1; //表示接受完成    
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
            Led_State_Control(0X01);    
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

