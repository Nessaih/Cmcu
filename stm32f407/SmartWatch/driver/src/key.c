#include "key.h"
#include "delay.h"

uint8 Key_Value = 0;
static u8 KeyStatus[5] = {0, 0, 0,  0,  0}; //保存5个按键的实时状态
						//L  R  Dn  Up  Ok


//时钟源 100Mhz
void TIM3_Init()
{
	RCC->APB1ENR |= 1<<1;//TIM3时钟使能
	TIM3->PSC = 100-1;		//100分频  计数器时钟：1MHZ
	TIM3->ARR = 5000-1;     // 定时 5MS

	/* TIM3_TRGO ---> ADC1 */
	TIM3->CR2 &= ~(7<<4);
	TIM3->CR2 |= (2<<4);	//选择更新事件作为 TRGO	


	TIM3->CR1 |= 1;		//计数器使能
	TIM3->DIER |= 1;		//更新中断使能
	NVIC_EnableIRQ(TIM3_IRQn);//NVIC 对应的中断通道使能
}
//定时 5MS 检测一次按键
void TIM3_IRQHandler()
{
	static u8 key_buf = 0;
	
	if (TIM3->SR & 1)
	{
		TIM3->SR &= ~1;
		
		key_buf <<= 1;
		key_buf |= KEY_OK_IO;//读取OK按键状态
		
		if (key_buf == 0)	   //稳定松开
		{
			KeyStatus[4] = 0;
		}
		else if (key_buf == 0xff)//稳定按下
		{
			KeyStatus[4] = 1;
		}
	}
}


//配置ADC1_CH3采集按键接口
static void ADC1_InitCh3()
{
	/* 配置ADC部分 规则组转换 */
	RCC->AHB1ENR |= 1;
	RCC->APB2ENR |= 1<<8;//ADC1
	
	GPIOA->MODER |= 3<<6;//PA3  模拟模式
	
	ADC->CCR |= 3<<16;  // 50/8 Mhz
	
	ADC1->SQR1 &= ~(0XF<<20);//1个通道在规则组转换
	ADC1->SQR3 &= ~(0X1F<<0);
	ADC1->SQR3 |= (0X3<<0);  //通道3 第一个转换
	
	ADC1->SMPR2 |= 4<<9;		//通道3采样周期配置
	
	ADC1->CR2 &= ~(3<<28);
	ADC1->CR2 |= (1<<28);		//使能外部触发
	
	ADC1->CR2 &= ~(0XF<<24);
	ADC1->CR2 |= (0X8<<24);	//选择 TIM3 TRGO作为触发源
	
	ADC1->CR1 |= 1<<5;        //使能ADC EOC中断
	NVIC_EnableIRQ(ADC_IRQn);//使能内核对应的中断通道
	
	ADC1->CR2 |= 1;//使能ADC1
}


void Key_Init()
{
	RCC->AHB1ENR |= 1<<0; //使能PA时钟

	/* 初始化PA0 */
	GPIOA->MODER &= ~(3);//输入模式   
	GPIOA->PUPDR &= ~(3);
	
	ADC1_InitCh3();
	TIM3_Init();   //5ms
}



void ADC_IRQHandler()//定时器3，每间隔5ms触发一次AD转换
{
	u8 i;       
	u16 AdcValue;
	static u8 AdcKeyBuf[4] = {0,  0,  0,  0};
	                       //   L   R   Dn  Up
	AdcValue = ADC1->DR;
	
	AdcKeyBuf[0] <<= 1;
	AdcKeyBuf[1] <<= 1;
	AdcKeyBuf[2] <<= 1;
	AdcKeyBuf[3] <<= 1;
	
	if (AdcValue > 3723)                          // 3v以上 (3.3V)
	{
		AdcKeyBuf[0] |= 1;	 //检测到LEFT 按下
	}	
	else if (AdcValue >1737 && AdcValue < 2234) //1.4v ~ 1.8v (1.65V)
	{
		AdcKeyBuf[1] |= 1;	 //检测到RIGHT 按下
	}
	else if (AdcValue >1241 && AdcValue < 1613) //1.0v ~ 1.3v (1.10V)
	{
		AdcKeyBuf[2] |= 1;	 //检测到DOWN 按下
	}
	else if (AdcValue >744 && AdcValue < 1179)  //0.6v ~ 0.95v(0.825V)
	{
		AdcKeyBuf[3] |= 1;	 //检测到UP 按下
	}
	for (i=0; i<4; i++)
	{
		if (AdcKeyBuf[i] == 0xff) //稳定按下
		{
			KeyStatus[i] = 1;
		}
		else if (AdcKeyBuf[i] == 0x00)//稳定松开
		{
			KeyStatus[i] = 0;
		}
	}
	
}
//左键按下返回1， 右键返回2，下键返回3，上键返回4，ok键返回5
//没有按键动作，返回0.
uint8_t Key_Scan()
{
	u8 i;
	static u8 KeyLastSta[5]={0,0,0,0,0};
	
	for (i=0; i<5; i++)
	{
		if (KeyLastSta[i] != KeyStatus[i])
		{
			KeyLastSta[i] = KeyStatus[i];
			
			if (KeyStatus[i] == 1)
			{
				return  i+1;//按下生效
			}
		}
	}
	return 0;
}
