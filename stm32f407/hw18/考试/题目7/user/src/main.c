#include "stm32f4xx.h"
#include "delay.h"
#include "iic.h"
int main (void)
{
	SysTick_Init();
	IIC_IO_Init();
	IIC_Start();
	IIC_Write(0xff);
	IIC_Stop();
	while(1)
	{
	
	}
}
