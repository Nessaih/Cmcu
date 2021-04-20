#include "beep.h"

void Beep_Io_Init(void)
{
	/*PF8配置成通用推挽输出*/
	RCC->AHB1ENR |= 0X01<<5; 		//开启PF端口时钟
	GPIOF->MODER &=~ (0x03<<16);	//清0操作
	GPIOF->MODER |= (0x01<<16);		//通用输出模式
	GPIOF->OTYPER &=~ (0x01<<8);	//推挽输出
	GPIOF->OSPEEDR &=~ (0X03<<16);	//输出速度为2MHz
	GPIOF->PUPDR &=~(0X03<<16);		//浮空模式
	
	
}
void Beep_State_Contrl(u8 Beep_State)
{
	if(1==Beep_State)
		GPIOF->ODR |=(0X01<<8);			//输出高电平蜂鸣器响	
	else
		GPIOF->ODR &=~(0X01<<8);		//输出低电平电平蜂鸣器不响	
}

