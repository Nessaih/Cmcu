#include "main.h"

int main(void)    
{
	Led_Io_Init();
	Usart1_Init();
	SysTick_Init();

	Led_State_Control(0x00);
	Time6_Configuration(8400,100);//≥¨ ±Ω” ’
	
	
	
    while(1)
    {
		;
	}   
}


