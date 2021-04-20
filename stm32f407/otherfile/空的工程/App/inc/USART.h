#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"
#include "type.h"
#include "string.h"
#include "led.h"


extern uint8 Timeout_flag;
extern uint8 flag;
extern uint8 RX_data[512];
extern uint8 i;
extern void usart_Io_Init (void);

extern void USART1_SET (uint32 Baud );

extern void send_data(uint8 data);

extern void send_String (uint8 *String );

//extern void R_data(void);
extern void P_R_data(void);



#endif 

