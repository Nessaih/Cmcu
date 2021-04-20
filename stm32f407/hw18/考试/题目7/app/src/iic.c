#include "iic.h"
#include "delay.h"
void IIC_IO_Init(void)
{
	RCC->AHB1ENR |= 0X01<<5;//PF端口使能
	GPIOF->MODER &=~ ((0x03<<0)|(0x03<<2));
	GPIOF->MODER |= ((0x01<<0)|(0x01<<2));
	GPIOF->OTYPER &=~ ((0x03<<0)|(0x03<<2));
	GPIOF->OSPEEDR &=~ ((0x03<<0)|(0x03<<2));
	GPIOF->PUPDR &=~ ((0x03<<0)|(0x03<<2));
	IIC_SDA_OH;
	IIC_SCL_OH;
}
void IIC_Start(void)
{
	IIC_SCL_OL;
	IIC_SDA_OH;//至少4.7us高电平
	IIC_SCL_OH;
	delay_us(5);
	IIC_SDA_OL;//至少4.0us低电平
	delay_us(5);
	IIC_SCL_OL;//准备发送数据
}
void IIC_Stop(void)
{
	IIC_SCL_OL;
	IIC_SDA_OL;//至少4.0us低电平
	IIC_SCL_OH;
	delay_us(5);
	IIC_SDA_OH;//至少4.7us高电平
	delay_us(5);

}
void IIC_Write(uint8 data)
{
	int8 i;
	IIC_SCL_OL;
	for(i=7;i>=0;i--)
	{
		IIC_SCL_OL;
		//if((data>>i)&0X01)
		if(data&(0x01<<i))
		{
			IIC_SDA_OH;
		}
		else
		{
			IIC_SDA_OL;
		}
		delay_us(5);
		IIC_SCL_OH;
		delay_us(5);
	}
	IIC_SCL_OL;
}
