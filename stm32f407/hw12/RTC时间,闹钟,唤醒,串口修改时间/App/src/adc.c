#include "adc.h"
#include "usart1.h"

/********************************************************************************************
*  函 数 名： ADC1_Configuration
*  描    述:  ADC1初始化配置(中断方式)
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-25
*  注意事项:  无
********************************************************************************************/
void ADC1_Configuration(void)
{
	RCC->AHB1ENR |=0X01;
	RCC->APB2ENR |=0X01<<8;//ADC1时钟使能
	GPIOA->MODER |=0X03<<10;//PA5模拟
	ADC1->CR1 &=~ ((0X03<<24)|(0X01<<8));//分辨率12位，禁止扫描
	ADC1->CR1 |= 0X01<<5;//EOCIE中断使能
	ADC1->CR2 &=~ (0X01<<11);//右对齐
	ADC1->CR2 |= (0X01<<1);//连续转换模式
	ADC1->CR2 |= (0X01<<10);//EOCS
	ADC1->SMPR1 &=~(0X07<<18);
	ADC1->SMPR1 |= (0X03<<18);//通道16采样时间56个周期
	ADC1->SQR1 &=~ (0X0F<<20);//组中序列长度1
	ADC1->SQR3 |=0X10;//IN16在序列中第一
	ADC->CCR |= (0X01<<23);//将温度传感器从掉电模式中唤醒
	ADC->CCR &=~ (0X03<<16);
	ADC->CCR |= (0X03<<16);//4分频:21MHz
	ADC1->SR &=~ (0X01<<1);//清除ECO标志
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(7-2,1,0));//设置优先级
	NVIC_EnableIRQ(ADC_IRQn); 
	ADC1->CR2 |= (0X01<<0);//ADC使能
	ADC1->CR2 |= 0X01<<30;//软件触发转换
}
uint16 Old_Value=0;
uint16 Digital_Value,Analog_Value;
//中断函数
void ADC_IRQHandler(void)
{
	if(ADC1->SR&(0X01<<1))
	{
		ADC1->SR &=~ (0X01<<1);//清除ECO标志
		{
			Digital_Value = ADC1->DR;
			Analog_Value=3300.0/4096*Digital_Value;
		}
	}
}
	
void ADC1_Temperature_Calculation(void)
{
	float Chip_Temperature;
	if((Analog_Value / 300) != Old_Value)
    {
		Old_Value = Analog_Value / 300;
		/*Analog_Value是扩大1000倍的数据，原始公式为：
		Chip_Temperature = ((Analog_Value/1000 - 0.76)/2.5)+25;*/
		Chip_Temperature = ((Analog_Value - 760)/2.5)+25;//2500不加.0，表达式会自动转化整型计算，舍掉小数部分
		printf("%.2f\r\n", Chip_Temperature);
    }
}
