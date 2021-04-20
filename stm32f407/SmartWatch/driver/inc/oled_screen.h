#ifndef __OLED_SCREEN_H__
#define __OLED_SCREEN_H__

#include "stm32f4xx.h"
#include "sys.h"
#include "type.h"
#include "spi.h"
#include "delay.h"

#define normal		0//正常显示
#define inverse		1//反白显示

#define MAX_COLUMNC	128

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED_DC  PA15
#define OLED_DC		PAout(15)//1表示给OLED传数据，0表示给OLED传命令

//OLED_CS  PB7	
#define OLED_CS_SET		PBout(7) = 1  	//片选禁止
#define OLED_CS_CLR		PBout(7) = 0	//片选使能

//OLED_RES  PB13
#define OLED_RST_SET	PBout(13) = 1  	//不复位
#define OLED_RST_CLR	PBout(13) = 0	//拉低复位

/*字符字体大小：高八位存字宽、低八位存字高
如：F8X16[]表示一个字符占8列，16行，则字体大小为0X0810*/
#define CH_SIZE1   0X0608
#define CH_SIZE2   0X0810
#define CH_SIZE3   0X1018

extern void OLED_Io_Init(void);
extern void OLED_Init(void);
extern uint8 OLED_Reset(void);
extern void OLED_Write_Byte(uint8 data,uint8 cmd);
extern void OLED_Clear(void);
extern void OLED_Set_PC(uint8 page,uint8 column);
extern void OLED_Set_Pos(uint8 x,uint8 y);
extern void OLED_OFF(void);
extern void OLED_ON(void);
extern void OLED_Set_Contrast(uint8 contrast);
extern void OLED_Show_Char(uint8 page,uint8 column,char ch,uint16 ch_size,uint8 mode);
extern void OLED_Show_String(uint8 page,uint8 column,char *str,uint16 ch_size,uint8 mode);
extern void Number2String(double num,char *str);
extern void OLED_Show_Chinese(uint8 page,uint8 column,uint8 index,uint8 mode);
extern void OLED_Show_Multi_Chinese(uint8 page,uint8 column,uint8 index[],uint8 n,uint8 mode);
extern void OLED_Show_Pic(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 BMP[],uint8 mode);
void OLED_Show_Pic_t(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 BMP[],uint8 mode);
#endif

