#include "AT24Cxx.h"
/********************************************************************************************
*  函 数 名： AT24_Write_Byte
*  描    述:  AT24C02写一个字节数据
*  输入参数： 写入数据，写入地址  
*  输出参数： 无
*  返 回 值:  0代表写入成功，1代表写入失败
*  作    者:  付伟
*  完成日期:  2018-07-28
*  注意事项:  无
********************************************************************************************/
uint8 AT24_Write_Byte(uint8 data,uint16 byte_addr)
{
	IIC_Start();
	IIC_Write(AT24C02_WADDR);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 1;
	}
	IIC_Write(byte_addr);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 2;
	}
	IIC_Write(data);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 3;
	}
	IIC_Stop();
	return 0;
}
/********************************************************************************************
*  函 数 名： AT24_Write_Page
*  描    述:  AT24C02写入16个字节数据
*  输入参数： 写入数据数组，写入起始地址  
*  输出参数： 无
*  返 回 值:  0代表写入成功，1代表写入失败
*  作    者:  付伟
*  完成日期:  2018-07-28
*  注意事项:  每次只能写16个字节，写超过会写不进去，如果写的数据超过16字节，可以通过反复调用此函数写入
********************************************************************************************/
uint8 AT24_Write_Page(uint8 data[],uint8 byte_addr)//data[16]
{
	uint8 i;
	IIC_Start();
	IIC_Write(AT24C02_WADDR);
	if(IIC_Wait_Reply()==IIC_REPLY)
	{
		IIC_Write(byte_addr);
		if(IIC_Wait_Reply()==IIC_REPLY)
		{
			IIC_Write(data[0]);
			for(i=1;i<16;i++)
			{
				if(IIC_Wait_Reply()==IIC_REPLY)
				{
					IIC_Write(data[i]);
				}
			}
			if(IIC_Wait_Reply()==IIC_REPLY)
			{
					IIC_Stop();
					return 0;
			}
		}
			
	}
	return 1;
}
/********************************************************************************************
*  函 数 名： AT24_Write_Continue
*  描    述:  任意字节写
*  输入参数： 
*  输出参数： 无
*  返 回 值: 
*  作    者:  付伟
*  完成日期:  2018-07-30
*  注意事项:  无
********************************************************************************************/
/*方法一：参考陈工的思路*/
uint8 AT24_Write_Continue(uint8 *data,uint16 nbyte, uint8 byte_addr)
{
	uint8 i,len;
	
	while(nbyte)
	{
		len = 8 - byte_addr%8;
		if(nbyte < len)
		{
			len = nbyte;
		}
		IIC_Start();
		IIC_Write(AT24C02_WADDR);
		if(IIC_Wait_Reply()==IIC_NO_REPLY)
		{
			return 1;
		}
		IIC_Write(byte_addr);
		if(IIC_Wait_Reply()==IIC_NO_REPLY)
		{
			return 2;
		}

		for(i=0;i<8;i++)
		{
			IIC_Write(*(data+i));
			if(IIC_Wait_Reply()==IIC_NO_REPLY)
			{
				return 3;
			}
		}
		IIC_Stop();		
		delay_ms(5);
		byte_addr += len;
		data += len;
		nbyte -=len; 
	}
	return 0;
}

/********************************************************************************************
*  函 数 名： AT24_Read_Byte
*  描    述:  AT24C02读取指定地址一字节数据
*  输入参数： 数据地址  
*  输出参数： 无
*  返 回 值:  读取数据数值
*  作    者:  付伟
*  完成日期:  2018-07-28
*  注意事项:  无
********************************************************************************************/
uint8 AT24_Read_Byte(uint8 byte_addr,uint8 *data)
{
	IIC_Start();
	IIC_Write(AT24C02_WADDR);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 1;
	}
	IIC_Write(byte_addr);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 2;
	}
	IIC_Start();
	IIC_Write(AT24C02_RADDR);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 3;
	}
	*data = IIC_Read();
	IIC_Write_Reply(IIC_NO_REPLY);
	IIC_Stop();
		
	return 0;
}
/********************************************************************************************
*  函 数 名： AT24_Read_Continue
*  描    述:  AT24C02连续读取nbyte字节数据
*  输入参数： 读取数据存入数组，读取数据长度（byte），读取起始地址  
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-30
*  注意事项:  无
********************************************************************************************/
uint8 AT24_Read_Continue(uint8 *data,uint16 nbyte, uint8 byte_addr)
{
	uint16 i; //读的字节最多有256个，不能定义为uint8
	IIC_Start();
	IIC_Write(AT24C02_WADDR);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 1;
	}
	IIC_Write(byte_addr);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 2;
	}
	IIC_Start();
	IIC_Write(AT24C02_RADDR);
	if(IIC_Wait_Reply()==IIC_NO_REPLY)
	{
		return 3;
	}	
	for(i=0;i<nbyte;i++)
	{
		*(data+i) = IIC_Read();
		if(i == nbyte - 1)
		{
			IIC_Write_Reply(IIC_NO_REPLY);
			IIC_Stop();
		}
		else
		{
			IIC_Write_Reply(IIC_REPLY);
		}
	}
	return 0;
}

void AT24C02_Clear(void)
{
	uint8 zero[256];
	memset(zero,0,256);
	AT24_Write_Continue(zero,256,0X00);
	delay_nms(5);
}
	
