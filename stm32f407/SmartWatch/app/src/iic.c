#include "iic.h"
void IIC_IO_Init(void)
{
	  /*****IIC_SCL配置为开漏输出****/
	RCC->AHB1ENR |= 1<<1;//使能PB时钟
	GPIOB->MODER &=~(3<<16);//清零
	GPIOB->MODER |= 1<<16;//GPIOB8配置通用输出模式
	GPIOB->OTYPER |= 1<<8;//GPIOB8配置为开漏模式
	/****IIC_SDA配置为开漏输出****/
	GPIOB->MODER &=~(3<<18);//清零
	GPIOB->MODER |= 1<<18;//GPIOB9配置普通模式
	GPIOB->OTYPER |= 1<<9;//GPIOB9配置为通用输出模式
	IIC_SDA_OH;
	IIC_SCL_OH;
}
void IIC_Start(void)
{
	delay_us(5);
  IIC_SCL_OH;
  IIC_SDA_OH;
  delay_us(5);//延时5us
  IIC_SDA_OL;
  delay_us(5);//延时5us
  IIC_SCL_OL;
}
void IIC_Stop(void)
{
	IIC_SDA_OL;
  delay_us(5);
  IIC_SCL_OH;
  delay_us(5);//延时5us
  IIC_SDA_OH;
  delay_us(5);//延时5us
}
//data ：待发送的数据
//返回值：0表示应答，1表示非应答
u8 IIC_WriteByte(u8 data)
{
  u8 i;
  u8 ack = 2;
  for(i=0;i<8;i++)  //8个时钟传输8位数据，从最高位开始传输
  {
    if(data &(1<<(7-i)))  
    {
      IIC_SDA_OH;
    }
    else
    {
      IIC_SDA_OL;
    }
    delay_us(5);//延时5us
    IIC_SCL_OH; 
    delay_us(5);//延时5us
    IIC_SCL_OL;
  }
  IIC_SDA_OH;//SDA输出高低电平切换为输入
  delay_us(5);//延时5us
  IIC_SCL_OH;
  if(IIC_SDA_READ)//SDA输入的电平为高电平表示非应答
  {
    ack = 1;
  }
  else  //SDA输入的电平为低电平表示应答
  {
    ack = 0;
  }
  delay_us(5);//延时5us
  IIC_SCL_OL;
  return ack;
}

//返回值表示读取到的数据
//形参ack：表示读取一个字节数据后给出的应答，0：应答，1：非应答
u8  IIC_ReadByte(u8 ack)
{
  u8 i;
  u8 data = 0;
  IIC_SDA_OH;//SDA输出高电平，切换为输入
  for(i=0;i<8;i++)  //产生8个时钟，读取8位数据
  {
    delay_us(5);//延时5us
    IIC_SCL_OH;
    data = data <<1;
    if(IIC_SDA_READ)
    {
      data |= 1<<0;
    }
    else   //SDA输入的低电平电平
    {
      data &=~(1<<0);
    }	
    delay_us(5); //延时5us
    IIC_SCL_OL;		
  }
  //产生第九个时钟，发送应答信号。
  if(ack == 0)
  {
    IIC_SDA_OL;   
  }
  else
  {
    IIC_SDA_OH;
  }
  delay_us(5);//延时5us
  IIC_SCL_OH;
  delay_us(5);//延时5us
  IIC_SCL_OL;
  return data;
}