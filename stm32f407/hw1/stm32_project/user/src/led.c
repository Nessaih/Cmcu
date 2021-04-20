#include "led.h"



//PF   6  9  10   
//PC   0           低电平有效

void led_init(void)
{
//打开RCC外设时钟对应位
	RCC->AHB1ENR |= 1 << 2;
	RCC->AHB1ENR |= 1 << 5;
	
//配置IO口模式寄存器
	GPIOF->MODER &= 0XFFC33FFF;
	GPIOF->MODER |= 0X00141000;
	GPIOC->MODER &= ~(3 << 0);
	GPIOC->MODER |= 1 << 0;
//配置输出类型
	GPIOF->OTYPER &= 0XF9BF;
	GPIOC->OTYPER &= ~(1 << 0);
//配置输出速度
	GPIOF->OSPEEDR &= 0XFFC33FFF;
	GPIOF->OSPEEDR |= 0X00141000;
	GPIOC->OSPEEDR &= ~(3 << 0);
	GPIOC->OSPEEDR |= (1 << 0);
//设置初始化状态为高电平
	GPIOF->ODR |= 0X0640;
	GPIOC->ODR |= 1 << 0;
}

void LED_ON(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

void LED_OFF(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}


