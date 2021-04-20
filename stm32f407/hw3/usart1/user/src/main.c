#include"usart1.h"
int main(void)
{
	unsigned char msg[]="fuwei\n";
	Usart1_Init();
	while(1)
	{
		Usart1_Str_Send(msg);	
	}
}
