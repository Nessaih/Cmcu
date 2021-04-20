#include "led.h"
void Led_Io_Init(void)
{
	RCC->AHB1ENR |= 0X01;
	GPIOA->MODER &=~ (0X03<<14);
	GPIOA->MODER |= 0X01<<14;	//Êä³ö
	GPIOA->PUPDR &=~ (0X03<<7);//¸¡¿Õ
	GPIOA->OTYPER &=~ (0X01<<7);//ÍÆÍì
	PAout(7) = 0;//ÁÁµÆ
}
