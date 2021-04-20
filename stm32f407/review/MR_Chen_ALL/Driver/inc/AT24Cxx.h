#ifndef __AT24Cxx_H_
#define __AT24Cxx_H_

#include "stm32f4xx.h"
#include "iic.h"
#include "string.h"

#define AT24C02_WADDR 0XA0	//器件地址（写）
#define AT24C02_RADDR 0XA1	//器件地址（读）

extern uint8 AT24_Write_Byte(uint8 data,uint16 byte_addr);
extern uint8 AT24_Write_Page(uint8 data[],uint8 byte_addr);
extern uint8 AT24_Read_Byte(uint8 byte_addr,uint8 *data);
extern uint8 AT24_Read_Continue(uint8 *data,uint16 nbyte, uint8 byte_addr);
extern uint8 AT24_Write_Continue(uint8 *data,uint16 nbyte, uint8 byte_addr);
extern void AT24C02_Clear(void);
#endif



