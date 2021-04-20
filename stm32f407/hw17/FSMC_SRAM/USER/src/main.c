#include "main.h"


u8 data_buf[1024*1024]  __attribute__((at(0X68000000)));

u8 buff[1024] = {0};
int main(void)    
{
	uint16 i;
	Led_Io_Init();
	Usart1_Init();
	SysTick_Init();
	IIC_IO_Init();
	SRAM_Init();
	
	Led_State_Control(0x00);
	Time6_Configuration(8400,100);//≥¨ ±Ω” ’
	
	for (i=0; i<1024; i++)
    {
        data_buf[i] = 'a';
    }

    for (i=0; i<1024; i++)
    {
        buff[i] = data_buf[i];
    }

	for (i=0; i<20; i++)
    {
        printf("%c\t",buff[i]);
    }
	
    while(1)
    {
		;
	}   
}


