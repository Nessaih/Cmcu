#include "rtc.h"

	char Week[15]={0XD2,0XBB,
					0XB6,0XFE,
					0XC8,0XFD,
					0XCB,0XC4,
					0XCE,0XE5,
					0XC1,0XF9,
					0XC8,0XD5,0
					};
void RTC_Init(void)
{
	uint16 delay = 10;
	RCC->APB1ENR |=0X01<<28;//电源接口时钟使能
	PWR->CR |= 0X01<<8;//使能备份域写保护
	RCC->BDCR |= 0X01;//LSEON使能
	do
	{
		delay_ms(1);
		delay--;
	}while(delay&&(!(RCC->BDCR&0X01)));//判断LSE是否就绪，超时退出
	if(!(RCC->BDCR&0X01))//如果LSE未就绪，换用LSI时钟
	{
//		判断LSI是否就绪。就绪则启用
//		RCC->BDCR &=~ (0X03<<8);
//		RCC->BDCR |= 0X10;
	}
	else//如果LSE就绪，选择LSE为RTC时钟源
	{
		RCC->BDCR &=~ (0X03<<8);
		RCC->BDCR |= 0X01<<8;//选择LSE为RTC时钟源
		RCC->BDCR |= 0X01<<15;//RTC使能
	}

	RTC->WPR =0XCA;
	RTC->WPR =0X53;//解锁写保护
}
uint8 BCD_encode(uint8 num,uint8 flag)
{
	uint8 temp=0;
	if(flag)//flag为真：解码0x18-->18
	{
		temp = ((num>>4)&0X0F)*10 +(num&0X0F);
	} 
	else//flag为0：编码18-->0x18
	{
		temp = ((num/10)<<4)|(num%10);
	}
	return temp;
}
void RTC_Setting(void)//开机设置时间日期
{
	uint16 delay = 10;
	uint32 temp = 0;
	
	RTC_Date dt ={18,7,27,5,9,8,30,0};//2018-7-27--17:11:30
	PRTC_Date rtc_dt = &dt;
	
	RTC->WPR =0XCA;
	RTC->WPR =0X53;//解锁写保护
	RTC->ISR |= 0X01<<7;//开始RTC初始化设置
	do
	{
		delay_ms(1);
		delay--;
	}while(delay&&(!(RTC->ISR&(0X01<<6))));//判断是否能初始化，超时退出
	RTC->CR &=~ ((0X01<<6)|(0X01<<5));//设置24小时格式,启用影子寄存器
	RTC->PRER = 255;//必须先设置同步分频（低位）
	RTC->PRER |= (127<<16);//后设置异步分频（高位），这两位期中一个改变，两个都需重新设置
	
	//日期设置
	temp = BCD_encode(rtc_dt->year-rtc_dt->year/100,0)<<16;
	temp |= (BCD_encode(rtc_dt->week,0)&0X07)<<13;
	temp |= (BCD_encode(rtc_dt->month,0)&0X1F)<<8;
	temp |= (BCD_encode(rtc_dt->day,0)&0X3F)<<0;
	RTC->DR = temp;
	temp = 0;
	//时间设置
	temp = (rtc_dt->pm&0X01)<<22;//24小时制
	temp |= (BCD_encode(rtc_dt->hour,0)&0X3F)<<16;
	temp |= (BCD_encode(rtc_dt->minute,0)&0X7F)<<8;
	temp |= (BCD_encode(rtc_dt->second,0)&0X7F)<<0;
	RTC->TR = temp;
	
	RTC->ISR &=~ (0X01<<7);//退出初始化设置
	RTC->WPR =0XFF;//重新写保护
}

void RTC_Setting_DT(PRTC_Date rtc_dt)//运行过程中设置时间日期
{
	uint16 delay = 10;
	uint32 temp = 0;
	
	RTC->WPR =0XCA;
	RTC->WPR =0X53;//解锁写保护
	RTC->ISR |= 0X01<<7;//开始RTC初始化设置
	do
	{
		delay_ms(1);
		delay--;
	}while(delay&&(!(RTC->ISR&(0X01<<6))));//判断是否能初始化，超时退出
	
	//日期设置
	temp = BCD_encode(rtc_dt->year,0)<<16;
	temp |= (BCD_encode(rtc_dt->week,0)&0X07)<<13;
	temp |= (BCD_encode(rtc_dt->month,0)&0X1F)<<8;
	temp |= (BCD_encode(rtc_dt->day,0)&0X3F)<<0;
	RTC->DR = temp;
	//时间设置
	temp = (rtc_dt->pm&0X01)<<22;//24小时制
	temp |= (BCD_encode(rtc_dt->hour,0)&0X3F)<<16;
	temp |= (BCD_encode(rtc_dt->minute,0)&0X7F)<<8;
	temp |= (BCD_encode(rtc_dt->second,0)&0X7F)<<0;
	RTC->TR = temp;
	
	RTC->ISR &=~ (0X01<<7);//退出初始化设置
	RTC->WPR =0XFF;//重新写保护

}
RTC_Date Read_DT(void)//读取时间
{
	uint32 temp = 0 ;
	RTC_Date dt ={0,0,0,0,0,0,0};//2018-7-27--17:11:30
	
	temp = RTC->DR;
	dt.year = BCD_encode((temp>>16)&0XFF,1) + 2000;
	dt.week = BCD_encode((temp>>13)&0X07,1);
	dt.month = BCD_encode((temp>>8)&0X1F,1);
	dt.day = BCD_encode((temp>>0)&0X3F,1);
	
	temp = RTC->TR;
	dt.hour = BCD_encode((temp>>16)&0XFF,1);
	dt.minute = BCD_encode((temp>>8)&0X7F,1);
	dt.second = BCD_encode((temp>>0)&0XFF,1);
	//dt.pm = BCD_encode((temp>>22)&0X001,1);//24小时制不用读取这一位
	return dt;
}

void Show_Time(RTC_Date dt)
{		
	
	printf("%d-%d-%d\t\t星期%c%c\t\t%d:%d:%d\r\n",\
	dt.year,dt.month,dt.day,Week[(dt.week-1)*2],Week[(dt.week-1)*2+1],dt.hour,dt.minute,dt.second);
}

#if  RTC_ALARM_A 
	
void RTC_ALARM_A_Init(void)
{
	uint16 delay = 10;
	uint32 alrmar = 0;
	RTC->WPR =0XCA;
	RTC->WPR =0X53;//解锁写保护
	RTC->CR &=~ (0X01<<8);//禁止闹钟A
	do
	{
		delay_ms(1);
		delay--;
	}while(delay&&(!(RTC->ISR&0X01)));//判断闹钟A是否允许更新，超时退出
	alrmar &=~ (0X01UL<<30);
	alrmar |=(BCD_encode(27,0)&0X3F)<<24;//27号
	alrmar |=(BCD_encode(10,0)&0X3F)<<16;//10点
	alrmar |=(BCD_encode(0,0)&0X7F)<<8;//0分
	alrmar |=(BCD_encode(30,0)&0X7F)<<0;//30秒
	RTC->ALRMAR = alrmar;
	RTC->CR |= (0X01<<12);//使能闹钟A中断
	EXTI->IMR |= 0X01<<17;//中断不屏蔽
	EXTI->RTSR |= 0X01<<17;//上升沿触发
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(RTC_Alarm_IRQn); 
	EXTI->PR |= (0X01<<17);//写1清楚中断挂起标志
	RTC->ISR &=~ (0X01<<8);//清楚闹钟A中断标志
	RTC->CR |= (0X01<<8);//使能闹钟A
	RTC->WPR =0XFF;//重新写保护
}
void RTC_Alarm_IRQHandler(void)
{
	
	if(EXTI->PR&(0X01<<17))
	{
		EXTI->PR |= (0X01<<17);//写1清楚中断挂起标志
	}
	if(RTC->ISR&(0X01<<8))
	{
		RTC->ISR &=~ (0X01<<8);//清楚闹钟A中断标志
		printf("闹钟A响了\n");//以后要用唤醒功能，这句可以改成别的，中断中不要打印，延时太久
		
	}
	
}
#endif

#if RTC_WAKEUP
void RTC_WakeUp_Init(void)
{
	uint16 delay = 10;
	
	RTC->WPR =0XCA;
	RTC->WPR =0X53;//解锁写保护
	RTC->CR &=~ (0X01<<10);//禁止唤醒
	do
	{
		delay_ms(1);
		delay--;
	}while(delay&&(!(RTC->ISR&(0X01<<2))));//判断闹钟A是否允许更新，超时退出
	
	RTC->CR &=~ (0X07<<0);
	RTC->CR |=0X04<<0;//唤醒时钟选择
	RTC->CR |= (0X01<<14);//使能唤醒中断
	RTC->WUTR = 2;//唤醒时间设置
	EXTI->IMR |= 0X01<<22;//中断不屏蔽
	EXTI->RTSR |= 0X01<<22;//上升沿触发
	NVIC_SetPriorityGrouping(7-2); //分组为第2组  
    NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,0,0));//设置优先级
	NVIC_EnableIRQ(RTC_WKUP_IRQn); 
	EXTI->PR |= (0X01<<22);//写1清除中断挂起标志
	RTC->ISR &=~ (0X01<<10);//清楚唤醒中断标志
	RTC->CR |= (0X01<<10);//使能唤醒
	RTC->WPR =0XFF;//重新写保护
}

void RTC_WKUP_IRQHandler(void)
{
	if(EXTI->PR&(0X01<<22))
	{
		EXTI->PR |= (0X01<<22);//写1清楚中断挂起标志
	}
	if(RTC->ISR&(0X01<<10))
	{
		RTC->ISR &=~ (0X01<<10);//清楚唤醒中断标志
		printf("唤醒\n");//以后要用唤醒功能，这句可以改成别的，中断中不要打印，延时太久
	}
}

#endif
