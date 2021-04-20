#include "stm32f4xx.h"
#include "led.h"
#include "Beep.h"
#include "key.h"
#include "usart1.h"
#include "EXTI.h"
#include "delay.h"
#include "timer.h"
int main(void)    
{
	Led_Io_Init();
	Usart1_Init();
	EXTI_Configration();

	Led_State_Control(0x01);
	Time6_Configuration(8400,100);//10us
	Time12_Configuration(84,3000);
    while(1)
    {
		Led_Switch();
		if(TIM12->CCR2<200)
		{
			TIM12->CCR2=200;
		}
		if(TIM12->CCR2>2800)
		{
			TIM12->CCR2=200;
		}
		
	}   
}


