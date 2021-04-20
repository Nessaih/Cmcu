#include "led.h"

/********************************************************************************************
*  函 数 名： Led_Io_Init
*  描    述:  LED端口初始化
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  陈斌
*  完成日期:  2018-07-18
*  注意事项:  无
********************************************************************************************/
void Led_Io_Init(void)
{
    /*PF6,PF9,PF10 配置成通用推挽输出模式*/
    RCC->AHB1ENR |= (0X01<<2)|(0X01<<5);        //开启PC.PF端口时钟
 
    GPIOF->MODER &= ~((0X03<<12)|(0X03<<18)|(0X03<<20));    //清零操作
    GPIOF->MODER |= (0X01<<12)|(0X01<<18)|(0X01<<20);       //通用输出模式
    GPIOF->OTYPER &= ~((0X01<<6)|(0X01<<9)|(0X01<<10));     //推挽模式
    GPIOF->OSPEEDR &= ~((0X03<<12)|(0X03<<18)|(0X03<<20));  //输出速度为2MHZ
    GPIOF->PUPDR &=  ~((0X03<<12)|(0X03<<18)|(0X03<<20));    //浮空模式
    //GPIOF->ODR &= ~((0X01<<6)|(0X01<<9)|(0X01<<10));       //输出低电平灯亮  
    
    GPIOC->MODER &= ~(0X03<<0);
    GPIOC->MODER |= 0X01<<0;
    GPIOC->OTYPER &= ~(0X01<<0);
    GPIOC->OSPEEDR &= ~(0X03<<0);
    GPIOC->PUPDR &= ~(0X03<<0);
    //GPIOC->ODR &= ~(0X01<<0); //输出低电平灯亮  
}
/********************************************************************************************
*  函 数 名： Led_State_Control
*  描    述:  LED灯状态控制
*  输入参数： u8 Led_State ：D0~D3控制LED1，LED2，LED3，LED4的亮灭,1为点亮，0为关闭
							D4~D7控制LED1，LED2，LED3，LED4的翻转,1为翻转，0为保持
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-22
*  注意事项:  低位和高位不能同时控制,高位为0时控制低位，高位不为0时控制低位
********************************************************************************************/
void Led_State_Control(u8 Led_State)
{
	if(!(Led_State&0XF0))//低位控制状态
	{
		//LED1控制
		if(Led_State&0x01)
		{
			LED1_STATE = 0;
		}
		else
		{
			LED1_STATE = 1;
		}
		
		//LED2控制
		if(Led_State&0x02)
		{
			LED2_STATE = 0;//亮
		}
		else
		{
			LED2_STATE = 1;//灭
		}
		
		//LED3控制
		if(Led_State&0x04)
		{
			LED3_STATE = 0;
		}
		else
		{
			LED3_STATE = 1;
		}
		
		//LED4控制
		if(Led_State&0x08)
		{
			LED4_STATE = 0;
		}
		else
		{
			LED4_STATE = 1;
		}
	}
	else//高位控制翻转
	{
		//LED1控制
		if(Led_State&0x10)
		{
			LED1_STATE =! LED1_STATE;
		}
		//LED2控制
		if(Led_State&0x20)
		{
			LED2_STATE =! LED2_STATE;
		}
		//LED3控制
		if(Led_State&0x40)
		{
			LED3_STATE =! LED3_STATE;
		}
		//LED4控制
		if(Led_State&0x80)
		{
			LED4_STATE =! LED4_STATE;
		}
	}
	
	
	
}
void Led_Delay(uint32 i) //延时 i*100 us
{
	uint32 j;
	for(;i>0;i--)
		for(j=0;j<=16800;j++);
}
void Water_Light(void)
{
	if(Led_Speed!=0XFFFF)
	{
		Led_State_Control(0X01);
		Led_Delay(Led_Speed);
		Led_State_Control(0X02);
		Led_Delay(Led_Speed);
		Led_State_Control(0X04);
		Led_Delay(Led_Speed);
		Led_State_Control(0X08);
		Led_Delay(Led_Speed);
	}
}
void Water_Light_Control(uint8 Up_down)
{
	if(Led_Speed>0||Led_Speed<500)
	{
		if(Up_down)
			Led_Speed -= 20;
		else
			Led_Speed += 20;
	}
}
void Water_Light_Pause(void)
{
	if(Led_Speed!=0XFFFF)
		Led_Speed_Recording=Led_Speed;
	Led_Speed=0XFFFF;
}
void Water_Light_Keepon(void)
{
	Led_Speed=Led_Speed_Recording;
}
