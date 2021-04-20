#include "stm32f4xx.h"
#include "led.h"
#include "Beep.h"
#include "key.h"




int main(void)    
{
	uint8 Key_Value;
	Led_Io_Init();
    Beep_Io_Init();
    Key_Io_Init();
	
	Led_State_Control(0x0f);

	Beep_State_Control(0);

    while(1)
    {
		Key_Value=Key_Scan();
		Water_Light(2);
		switch(Key_Value)
		{
			case 1:Water_Light_Speed(1);break;//º”ÀŸ
			case 2:Water_Light_Speed(0);break;//ºıÀŸ
			case 3:Water_Light(1);;break;//‘›Õ£
			case 4:Water_Light(0);break;
		}	
    } 
        
        
}


