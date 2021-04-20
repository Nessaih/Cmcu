#ifndef __KEY__H_
#define __KEY__H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"

#define KEY1_STATE PAin(0)
#define KEY2_STATE PEin(2)
#define KEY3_STATE PEin(3)
#define KEY4_STATE PEin(4)



extern void Delay(uint32 Time);
extern void Key_Io_Init(void);
extern uint8 Key_Scan(void);

#endif
