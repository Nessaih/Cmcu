#include "led.h"

void Led_Io_Init(void)
{
	/*PF6配置成通用推挽输出*/
	RCC->AHB1ENR |= 0X01<<5; 		//开启PF端口时钟
	GPIOF->MODER &=~ ((0x03<<12)|(0x03<<18)|(0x03<<20));	//清0操作
	GPIOF->MODER |= (0x01<<12)|(0x01<<18)|(0x01<<20);		//通用输出模式
	GPIOF->OTYPER &=~ ((0x01<<6)|0x01<<9)|(0x01<<10);	//推挽输出
	GPIOF->OSPEEDR &=~ ((0X03<<12)|(0X03<<18)|(0x01<<10));	//输出速度为2MHz
	GPIOF->PUPDR &=~((0X03<<12)|(0X03<<18)|(0X03<<20));		//浮空模式
	GPIOF->ODR &=~((0X01<<6)|(0X01<<8)|(0X01<<10));		//输出低电平灯亮
	
	/*PC0配置成通用推挽输出*/
	RCC->AHB1ENR |= 0X01<<2; 		//开启PF端口时钟
	GPIOC->MODER &=~ (0x03<<0);	//清0操作
	GPIOC->MODER |= (0x01<<0);		//通用输出模式
	GPIOC->OTYPER &=~ (0x01<<0);	//推挽输出
	GPIOC->OSPEEDR &=~ (0X03<<0);	//输出速度为2MHz
	GPIOC->PUPDR &=~(0X03<<0);		//浮空模式
	GPIOC->ODR &=~(0X01<<0);		//输出低电平灯亮
}
