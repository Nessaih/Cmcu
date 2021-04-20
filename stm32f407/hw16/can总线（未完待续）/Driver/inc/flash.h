#ifndef __FLASH_H_
#define __FLASH_H_

#include "stm32f4xx.h"
#include "type.h"
#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "spi.h"

#define Simulation  1

#define FLASH_CS_OL  PBout(14) = 0
#define FLASH_CS_OH  PBout(14) = 1

#define DUMMY_BYTE 					0XFF
#define FLASH_CMD_DEVICE_ID 		0X90
#define FLASH_CMD_WRITE_ENABLE		0X06
#define FLASH_CMD_WRITE_DISABLE		0X04
#define FLASH_CMD_READ_STATUS_REG1	0X05
#define FLASH_CMD_READ_STATUS_REG2	0X35
#define FLASH_CMD_WRITE_STATUS_REG	0X01
#define FLASH_CMD_SECTOR_ERASE		0X20
#define FLASH_CMD_WRITE_DATA		0X02
#define FLASH_CMD_READ_DATA			0X03


#define FLASH_WRITE_STATUS_REG_TIME_OUT 	18		//15+3
#define FLASH_PEGE_PROGARM_TIME_OUT			5		//3+2	
#define FLASH_SECTOR_ERASE_TIME_OUT			450		//400+50
#define FLASH_BLOCK32_ERASE_TIME_OUT


#define FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR) 		(ADDR &~ 0XFFFUL)
#define FLASH_CALCULATE_PAGE_1ST_ADDR(ADDR) 		(ADDR &~ 0XFFUL)
#define FLASH_CALCULATE_SECTOR_ADDR_OFFSET(ADDR) 	(ADDR &  0XFFFUL)

#define	FLASH_SECTOR_SIZE	4096
#define	FLASH_PAGE_SIZE		256
#define FLASH_PAGE_NUM_OF_EACH_SECTOR 16
#define FLASH_BACKUP_SECTOR			0X7FF000



extern void Flash_Io_Init(void);
extern void Flash_Init(void);
extern uint8 Flash_RW_Byte(uint8 data);
extern void Flash_Write_Addr(uint32 Addr);
extern void Flash_CMD_Read_ID(void);
extern void Flash_CMD_Write_Enable(void);
extern uint8 Flash_Read_Status_Reg1(void);
extern uint8 Flash_Read_Status_Reg2(void);
extern uint8 Flash_Write_Status_Reg(uint16 Reg_value);
extern uint8 Flash_Wait_Busy(uint16 Time_Out);
extern uint8 Flash_Sector_Erase(uint32 ADDR);
extern void Flash_NSector_Erase(uint32 ADDR,uint16 num);
extern void Flash_Write_Single_Page(uint8 *data, uint16 nbyte, uint32 ADDR);
extern void Flash_Write_Cross_Page(uint8 *data, uint32 nbyte, uint32 ADDR);
extern void Flash_Read_Data(uint8 *data, uint32 nbyte, uint32 ADDR);
extern void Flash_Copy_Sector(uint32 ADDR_Src,uint32 ADDR_Dst);
extern void Flash_Copy_Date(uint32 ADDR_Src,uint32 ADDR_Dst,uint32 nbyte);
extern void Flash_Write_Single_Sector(uint8 *data, uint32 nbyte, uint32 ADDR);
extern void Flash_Write_Cross_Sector(uint8 *data, uint32 nbyte, uint32 ADDR);
#endif


