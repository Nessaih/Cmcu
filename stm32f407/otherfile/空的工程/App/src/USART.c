#include "USART.h"



uint8 RX_data[512];
uint8 flag=0;
uint8 i=0;
uint8 Timeout_flag;



/********************************************************************************************
*  函 数 名： usart_Io_Init
*  描    述:  USART1 初始化
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  秦浩
*  完成日期:  2018-07-18
*  注意事项:  无
********************************************************************************************/


void usart_Io_Init (void)
{
    RCC->AHB1ENR |=0x01<<0;//开启PA端口
    GPIOA->MODER &=~((0x03<<18)|(0x03<<20));//清零
    GPIOA->MODER |=(0x02<<18)|(0x02<<20);//设置为复用模式
    GPIOA->OTYPER &= ~(0X01<<9);//PA9推挽输出
    GPIOA->OSPEEDR &=~(0X03<<18);
    GPIOA->OSPEEDR |=0X01<<18;
    GPIOA->PUPDR &=~(0X03<<20);//上拉输入
    GPIOA->PUPDR |=0X01<<20;
    GPIOA->AFR[1] &=~((0x0f<<4)|(0x0f<<8));//清零
    GPIOA->AFR[1] |=(0x07<<4)|(0x07<<8);//设置为AF7
    
} 



void USART1_SET (uint32 Baud )
{
    float USART_DIV;
    uint16 Div_Man;
    float Div_f;
    RCC->APB2ENR  |=0x01<<4;//USART1时钟使能；
    RCC->APB2RSTR |=0X01<<4;
    RCC->APB2RSTR &=~(0X01<<4);//复位与清零
    USART1->CR1 |=(0X01<<2)|(0X01<<3);//发送器与接收器使能
    USART1->CR2 &=~(0X03<<12);//位停止
    
    
    USART_DIV=(float)84000000/(8*2*Baud);
    Div_Man =USART_DIV ;
    Div_f=USART_DIV-Div_Man;
    USART1->BRR =(Div_Man <<4)|((uint16 )(Div_f *16));
    
    USART1->CR1 |= 0X01<<5;  //串口接收中断使能
    USART1->SR &= ~(0X01<<5); //标志位清零
    NVIC_SetPriorityGrouping(7-2); //分组  
    NVIC_EncodePriority(7-2,0,0);
    NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
    NVIC_EnableIRQ(USART1_IRQn); //中断使能
    
    
    USART1->CR1 |=(0X01<<13);// USART1使能
    
    
    

    
}




void USART1_IRQHandler(void)
 {
     uint8 Receive_data;
 
    if(USART1->SR &(0X01<<5))
    {
        Timeout_flag=3;
        Receive_data=USART1->DR ;
        if(flag==0)
        {
             RX_data[i++]=Receive_data; 
        }
        else 
        {
            flag=1;
        }
    }
 }
void send_data(uint8 data)
{
    while(!(USART1->SR &(0x01<<7)));
    
     USART1->DR =data;
    
}

void send_String (uint8 *String )
{
    while(*String !='\0')
    {
       send_data(*String ) ;
       String ++;
    }
}



//void R_data(void)
//{

//    uint8 Receive_data;
// 
//    if(USART1->SR &(0X01<<5))
//    {
//        Receive_data=USART1->DR ;
//        if((Receive_data!='$')&&(flag==0))
//        {
//             RX_data[i++]=Receive_data; 
//        }
//        else 
//        {
//            flag=1;
//        }
//    }
//}

void P_R_data(void)
{
    if(flag )
    {
        send_String (RX_data);
        if(strcmp((const char *)RX_data,"Led_ON")==0)
        {
             Led_state_control(0x0f);
        }
        else if(strcmp((const char *)RX_data,"Led_OFF")==0)
        {
             Led_state_control(0x00);    
        }
        else if(strcmp((const char *)RX_data,"Beep_ON")==0)
        {
             //Beep_State_Contrl(1);
        }
        else if(strcmp((const char *)RX_data,"Beep_OFF")==0)
        {
            // Beep_State_Contrl(0);
        }
        memset(RX_data,0,i);
        flag=0;
        i=0;
    }
    
}










#pragma import(__use_no_semihosting_swi) //???????

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

