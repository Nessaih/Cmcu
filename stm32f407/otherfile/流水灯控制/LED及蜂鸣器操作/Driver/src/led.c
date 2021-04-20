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
void Led_State_Control(u8 Led_State)
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
void Led_Delay(uint32 i) //延时 i*100 us
{
	uint32 j;
	for(;i>0;i--)
		for(j=0;j<=16800;j++);
}
uint8 Water_Light(uint8 If_Pause)
{
	/*
	If_Pause==0 不暂停
	
	If_Pause==1 暂停 
	*/
		Led_State_Control(0X01);
		if(If_Pause) return If_Pause;
		Led_Delay(Led_Speed);

		Led_State_Control(0X02);
		if(If_Pause) return If_Pause;
		Led_Delay(Led_Speed);

		Led_State_Control(0X04);
		if(If_Pause) return If_Pause;
		Led_Delay(Led_Speed);

		Led_State_Control(0X08);
		if(If_Pause) return If_Pause;
		Led_Delay(Led_Speed);
}
void Water_Light_Speed(uint8 Up_down)
{
	if(Led_Speed>0||Led_Speed<500)
	{
		if(Up_down)
			Led_Speed -= 20;
		else
			Led_Speed += 20;
	}
}

