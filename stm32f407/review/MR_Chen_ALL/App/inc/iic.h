#ifndef __IIC_H_
#define __IIC_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"
#include "delay.h"

#define IIC_REPLY 0
#define IIC_NO_REPLY 1

#define IIC_SDA_OH 		PBout(9) = 1
#define IIC_SDA_OL 		PBout(9) = 0
#define IIC_SCL_OH 		PBout(8) = 1
#define IIC_SCL_OL		PBout(8) = 0
#define IIC_SDA_IN 		GPIOB->MODER &=~ (0x03<<18);
#define IIC_SDA_OUT 	{GPIOB->MODER &=~ (0x03<<18);GPIOB->MODER |= (0x01<<18);}
#define IIC_SDA_READ 	PBin(9)

extern void IIC_IO_Init(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void IIC_Write(uint8 data);
extern uint8 IIC_Read(void);
extern void IIC_Write_Reply(uint8 reply);
extern uint8 IIC_Wait_Reply(void);


#endif



