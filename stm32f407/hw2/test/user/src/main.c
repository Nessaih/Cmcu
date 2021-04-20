#include "stm32f4xx.h"
#include "beep.h"
#include "led.h"
int main (void)
{
	Led_Io_Init();
	Beep_Io_Init();
	while(1)
	{
		;
	}
}
