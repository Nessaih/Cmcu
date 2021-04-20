#include "key.h"
#include "includes.h"
void Key_Io_Init(void)
{
	//KEY1
	RCC->AHB1ENR |= 0X01;		//打开PA时钟
	GPIOA->MODER &=~ 0X03;		//清0操作，复位PA0模式,输入模式
	GPIOA->PUPDR &=~0X03;		//PA0浮空模式
	
	//KEY2,KEY3,KEY4
	RCC->AHB1ENR |= 0X01<<4;		//打开PE时钟
	GPIOE->MODER &=~ ((0X03<<4)|(0X03<<6)|(0X03<<8));//清0操作，复位PE234模式,输入模式
	GPIOE->PUPDR &=~((0X03<<4)|(0X03<<6)|(0X03<<8));//PE234浮空模式
}
	
uint8 Key_Scan(void)
{
	static uint8 Key_Sta = 1;//按键锁标志
	if((KEY1_STATE||(!KEY2_STATE)||(!KEY3_STATE)||(!KEY4_STATE))&&Key_Sta)//按键按下为高
	{
		OSTimeDly(2);//10ms
		if(KEY1_STATE)
		{
			Key_Sta = 0;
			return 1;
		}
		if(!KEY2_STATE)
		{
			Key_Sta = 0;
			return 2;
		}
		if(!KEY3_STATE)
		{
			Key_Sta = 0;
			return 3;
		}
		if(!KEY4_STATE)
		{
			Key_Sta = 0;
			return 4;
		}
	}
	else if((!KEY1_STATE)&&KEY2_STATE&&KEY3_STATE&&KEY4_STATE)
	{
		Key_Sta = 1;
	}
	return 0;
		
}

