#include "spi.h"
/********************************************************************************************
*  函 数 名： Spi_Io_Init
*  描    述:  SPI端口模拟传输
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Spi_Io_Init(void)
{
	RCC->AHB1ENR |= 0X01<<1;
	GPIOB->MODER &=~((0X03<<6)|(0X03<<8)|(0X03<<10));
	GPIOB->MODER |= (0X01<<6)|(0X01<<10);
	GPIOB->OTYPER &=~ (0X01<<3)|(0X01<<5);
	GPIOB->OSPEEDR &=~ (0X03<<6)|(0X03<<10);
	GPIOB->OSPEEDR |= (0X02<<6)|(0X02<<10);
	GPIOB->PUPDR &=~((0X03<<6)|(0X03<<8)|(0X03<<10));
	GPIOB->PUPDR |=(0X01<<8);//PB4上拉输入
	SPI_SCK_OL;
	SPI_MOSI_OL;
}

uint8 Spi_RW(uint8 data)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		SPI_SCK_OL;
		if(data&0X80)
		{
			SPI_MOSI_OH;
		}
		else
		{
			SPI_MOSI_OL;
		}
		delay_us(1);
		SPI_SCK_OH;
		delay_us(1);
		data <<= 1;
		if(SPI_READ)
		{
			data |=0X01;
		}
	}
	SPI_SCK_OL;
	return data;
}
