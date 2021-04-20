#include "stm32f4xx.h"
#include "led.h"
#include "Beep.h"
#include "key.h"
#include "usart1.h"
#include "EXTI.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "rtc.h"
int main(void)    
{

	RTC_Date dt={2018,7,27,5,10,0,25,0};
	uint8 old = 0;
	Led_Io_Init();
	Usart1_Init();
	//ADC1_Configuration();
	RTC_Init();
	RTC_Setting(dt);
	//RTC_Setting_DT(dt);
//	RTC_WakeUp_Init();
//	RTC_ALARM_A_Init();
	Led_State_Control(0x01);
	Time6_Configuration(8400,100);//10us
//	Time12_Configuration(84,1500);//ºôÎüÆÁ
//	Time5_CH1_Capture_Configuration(84,Capture_ARR);//°´¼üÊ±¼ä
    while(1)
    {
		//Led_Switch();
		dt = Read_DT();
		if(dt.second != old)
		{
			Show_Time(dt);
			old = dt.second;
		}
		Usart_Setting_DT();
	}   
}


