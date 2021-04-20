#include "sram.h"

void SRAM_Init(void)
{
	/*IO口初始化，设为复用功能，PD0,1,4,5,8～15；PE0,1,7～15；//PF0～5,12～15；/PG0～5,10*/	
	RCC->AHB1ENR|=0XF<<3;    	//使能PD,PE,PF,PG时钟  
	GPIOD->MODER |=0XAAAA0A0A;  //配置成复用输出
	GPIOE->MODER |=0XAAAA800A;  //配置成复用输出
	GPIOF->MODER |=0XAA000AAA;  //配置成复用输出
	GPIOG->MODER |=0X00200AAA;  //配置成复用输出
	GPIOE->OSPEEDR |=0XFFFFC00F;   //配置成输出速率100M
	GPIOF->OSPEEDR |=0XFF000FFF;   //配置成输出速率100M
	GPIOG->OSPEEDR |=0X00300FFF;   //配置成输出速率100M

	/*复用功能选择FSMC*/
	GPIOD->AFR[0] = 0X00CC00CC;         
	GPIOD->AFR[1] = 0XCCCCCCCC;
	GPIOE->AFR[0] = 0XC00000CC;         
	GPIOE->AFR[1] = 0XCCCCCCCC;
	GPIOF->AFR[0] = 0X00CCCCCC;         
	GPIOF->AFR[1] = 0XCCCC0000;
	GPIOG->AFR[0] = 0X00CCCCCC;         
	GPIOG->AFR[1] = 0X00000C00;

	/*FSMC寄存器配置*/
	RCC->AHB3ENR|=1<<0;   
	FSMC_Bank1_BCR |= (0X01<<12)|(0X01<<1);//写使能;45位默认16位数据宽度；地址、数据线非复用
	FSMC_Bank1_BTR &=~(0X0F<<20);//时钟不分频
	FSMC_Bank1_BTR |= 0X08<<8;//数据保持时间9*6ns = 54≈55
	FSMC_Bank1_BTR &=~(0X0F<<4);//地址保持时间0
	FSMC_Bank1_BTR &=~(0X0F<<0);//建立地址时间0
	FSMC_Bank1_BCR |=1<<0; 	//使能
}

