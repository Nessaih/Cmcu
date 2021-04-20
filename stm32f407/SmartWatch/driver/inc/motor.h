#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"

extern void Motor_Init(void);
extern void Motor_Speed(uint8 speed_mode);

#endif


