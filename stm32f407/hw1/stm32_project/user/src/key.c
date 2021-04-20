#include "key.h"
#include "test.h"

void key_init(void)
{
	//KEY1 --PA0
	RCC->AHB1ENR |= 0x01; 		//打开时钟，GPIOA可读写
	GPIOA->MODER &= ~(3 << 0);  //配置管脚模式
	GPIOA->PUPDR &= ~(3 << 0);	//配置下拉
	GPIOA->PUPDR |= (2 << 0);		
	
	//KEY2 --PE2
	RCC->AHB1ENR |= 0x10;//打开时钟，GPIOE可读写
	
	待写。。。
}


int key_scan(void)
{
	static int key = 1;
	if(KEY1 && key)
	{
		key = 0;
		delay(1000000);
		if(KEY1)
		{
			return 1;
		}
	}
	else if(!KEY1 && key == 0)
		key = 1;
	return 0;
}
