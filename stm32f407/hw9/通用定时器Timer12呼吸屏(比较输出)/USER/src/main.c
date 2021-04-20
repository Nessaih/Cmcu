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

	Led_State_Control(0x01);
	Time6_Configuration(8400,100);//10us
	Time12_Configuration(84,1500);
    while(1)
    {
		Led_Switch();
	}   
}


