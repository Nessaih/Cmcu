#ifndef __IIC_H_
#define __IIC_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"
#include "delay.h"

#define IIC_REPLY 0
#define IIC_NO_REPLY 1

#define IIC_SDA_OH 		PFout(1) = 1
#define IIC_SDA_OL 		PFout(1) = 0
#define IIC_SCL_OH 		PFout(0) = 1
#define IIC_SCL_OL		PFout(0) = 0


extern void IIC_IO_Init(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
void IIC_Write(uint8 data);

#endif

