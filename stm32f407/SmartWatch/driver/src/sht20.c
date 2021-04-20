#include "sht20.h"
#include "oled_screen.h"
uint16 Digital_Value;
void SHT20_Init(void)
{
	IIC_IO_Init();
	//传感器复位
	IIC_Start();
	IIC_WriteByte(SHT20ADDR_W);
	IIC_WriteByte(SHT20_RESET);
	delay_ms(15);
	IIC_Stop();
}

uint8 SHT20_Write_CMD(uint8 cmd)
{
	uint8 time_out=20,reply;
	IIC_Start();
	if(IIC_WriteByte(SHT20ADDR_W)==IIC_NO_REPLY)
	{
		//printf("SHT20_Write_CMD:返回1");
		return 1;
	}
	if(IIC_WriteByte(cmd)==IIC_NO_REPLY)
	{
		//printf("SHT20_Write_CMD:返回2");
		return 2;
	}
	do{
		delay_ms(10);
		time_out--;
		IIC_Start();
		reply=IIC_WriteByte(SHT20ADDR_R);
		
	}while((reply==IIC_NO_REPLY)&&time_out);//最多等待200ms
	if(!time_out)
	{
		printf("SHT20_Write_CMD:返回3");
		return 3;
	}
	Digital_Value = IIC_ReadByte(IIC_REPLY);//读取数据8高位
	Digital_Value <<=8;

	Digital_Value |=IIC_ReadByte(IIC_REPLY);//读取数据低8位
	IIC_ReadByte(IIC_NO_REPLY);//读校验信息
	IIC_Stop();
	return 0;
}

uint8 SHT20_Calculation(double *data,uint8 cmd)
{
	SHT20_Write_CMD(cmd);
	//printf("Digital_Value = %d\n",Digital_Value);//调试用的
	if(cmd == MEASURE_TEMPERATURE)
	{
		*data = 175.72*Digital_Value/65536 - 46.85;
		return 0;
	}
	else if(cmd == MEASURE_HUMIDITY)
	{
		*data = 125.0*Digital_Value/65536 - 6;
		return 0;
	}
	else
	{
		return 1;
	}
}
void SHT20_Print(void)
{
	double sht20_data;
	SHT20_Calculation(&sht20_data,MEASURE_TEMPERATURE);
	printf("温度:%.2lf\t",sht20_data);
	SHT20_Calculation(&sht20_data,MEASURE_HUMIDITY);
	printf("湿度:%.2lf\r\n",sht20_data);
	delay_nms(1000);//1s串口打印一次
}
void SHT20_Show(void)
{//OLED显示温湿度
	double sht20_data;
	uint8 index[10]={2,3,};
	char str_buff[10];
	
	SHT20_Calculation(&sht20_data,MEASURE_TEMPERATURE);
	Number2String(sht20_data,str_buff);
	
	OLED_Show_Multi_Chinese(2,16,index,2,normal);
	OLED_Show_Char(2,48,':',CH_SIZE2,normal);
	OLED_Show_String(2,56,str_buff,CH_SIZE2,normal);
	OLED_Show_Chinese(2,98,1,normal);//℃
	
	index[0]=4;
	index[1]=5;
	SHT20_Calculation(&sht20_data,MEASURE_HUMIDITY);
	OLED_Show_Multi_Chinese(4,16,index,2,normal);
	Number2String(sht20_data,str_buff);
	OLED_Show_Char(4,48,':',CH_SIZE2,normal);
	OLED_Show_String(4,56,str_buff,CH_SIZE2,normal);
	OLED_Show_Char(4,98,'%',CH_SIZE2,normal);
}

