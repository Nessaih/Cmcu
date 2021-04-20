#include "iic.h"
void IIC_IO_Init(void)
{
	RCC->AHB1ENR |= 0X01<<1;//PB端口使能
	GPIOB->MODER &=~ ((0x03<<16)|(0x03<<18));
	GPIOB->MODER |= ((0x01<<16)|(0x01<<18));
	GPIOB->OTYPER &=~ ((0x01<<8)|(0x01<<9));
	GPIOB->OSPEEDR &=~ ((0x03<<16)|(0x03<<18));
	GPIOB->PUPDR &=~ ((0x03<<16)|(0x03<<18));
	IIC_SDA_OH;
	IIC_SCL_OH;
}
void IIC_Start(void)
{
	IIC_SCL_OL;
	IIC_SDA_OUT;
	IIC_SDA_OH;//至少4.7us高电平
	IIC_SCL_OH;
	delay_us(5);
	IIC_SDA_OL;//至少4.0us低电平
	delay_us(5);
}
void IIC_Stop(void)
{
	IIC_SCL_OL;
	IIC_SDA_OUT;
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
	IIC_SDA_OUT;
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
uint8 IIC_Read(void)
{
	int8 i;
	uint8 data = 0;
	IIC_SCL_OL;
	IIC_SDA_IN;
	for(i=7;i>=0;i--)
	{
		IIC_SCL_OL;
		delay_us(5);
		IIC_SCL_OH;
		delay_us(5);
		if(IIC_SDA_READ)
		{
			data |= 0x01<<i;
		}
	}
	return data;
}
void IIC_Write_Reply(uint8 reply)
{
	IIC_SCL_OL;
	IIC_SDA_OUT;
	if(reply==IIC_REPLY)
	{
		IIC_SDA_OL;
	}
	else
	{
		IIC_SDA_OH;
	}
	IIC_SCL_OH;
	delay_us(5);
	IIC_SCL_OL;
}
uint8 IIC_Wait_Reply(void)
{
	uint8 reply = 0;
	IIC_SCL_OL;
	IIC_SDA_IN;

	IIC_SCL_OH;
	delay_us(2);
	reply  = IIC_SDA_READ;
	if(reply==IIC_NO_REPLY)
	{
		IIC_Stop();
	}
	return reply;
}
