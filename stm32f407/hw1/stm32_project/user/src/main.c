#include "test.h"
#include "led.h"
#include "key.h"




int main(void)
{

	led_init();
	key_init();
	while(1)
	{
		if(key_scan())
		{
			LED1_TURN;
		}
	}

}



