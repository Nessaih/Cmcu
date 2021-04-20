#ifndef __LED_H_
#define __LED_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"

#define LED1_STATE PFout(6)
#define LED2_STATE PFout(9)
#define LED3_STATE PFout(10)
#define LED4_STATE PCout(0)

static uint32 Led_Speed = 20;
static uint32 Led_Speed_Recording= 20;

extern void Led_Io_Init(void);
extern void Led_State_Control(u8 Led_State);
extern void Led_Delay(uint32 i);
extern void Water_Light(void);
extern void Water_Light_Control(uint8 Up_down);
extern void Water_Light_Pause(void);
extern void Water_Light_Keepon(void);
#endif
