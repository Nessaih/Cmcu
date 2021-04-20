#ifndef __BEEP_H_
#define __BEEP_H_

#include "stm32f4xx.h"
#include "sys.h"

#define 	BEEP_STATE		PFout(8)


extern void Beep_Io_Init(void);
extern void Beep_State_Control(u8 Beep_State);

#endif



