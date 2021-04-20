#include "stm32f4xx.h"
#include "led.h"
#include "Beep.h"
#include "key.h"
#include "usart1.h"
#include "EXTI.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"

int main(void)    
{
	Led_Io_Init();
	Usart1_Init();
	ADC1_Configuration();
	
	Led_State_Control(0x01);
	Time6_Configuration(8400,100);//10us
	Time12_Configuration(84,1500);
	Time5_CH1_Capture_Configuration(84,Capture_ARR);
    while(1)
    {
		ADC1_Sampling();
//		if(Capture_End_Flag)
//		{
//			printf("按下时间：%.4f\r\n",(float)Capture_Sum_Count/1000);
//			Capture_End_Flag=0;
//		}
	}   
}


