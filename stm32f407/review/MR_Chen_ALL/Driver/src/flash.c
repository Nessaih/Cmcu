#include "flash.h"

/********************************************************************************************
*  函 数 名： Flash_Io_Init
*  描    述:  Flash控制端口初始化配置，CS片选浮空推挽输出
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_Io_Init(void)
{
	//PB14---CS非
	RCC->AHB1ENR |=0x01<<1;
	GPIOB->MODER &=~ (0x03<<28);
	GPIOB->MODER |=0x01<<28;
	GPIOB->OTYPER &=~(0x01<<14);
	GPIOB->PUPDR &=~(0x03<<28);
	GPIOB->OSPEEDR	&=~(0x03<<28);
	//GPIOB->OSPEEDR |=(0x02<<28);
	FLASH_CS_OH;
}
/********************************************************************************************
*  函 数 名： Flash_Init
*  描    述:  Flash初始化，SPI总线端口和flash片选端口
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/


#if Simulation
/***********使用IO口模拟****************/
void Flash_Init(void)
{
	Spi_Io_Init();
	Flash_Io_Init();
}

/********************************************************************************************
*  函 数 名： Flash_RW_Byte
*  描    述:  Flash读取写入函数
*  输入参数： 一字节待写入数据   
*  输出参数： 无
*  返 回 值:  读取的一字节数据
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/


uint8 Flash_RW_Byte(uint8 data)
{
	return Spi_RW(data);
}

#else
/***********************************************使用片内外设****************************************************/
/***********************************************使用片内外设****************************************************/
/***********************************************使用片内外设****************************************************/

void Flash_Init(void)
{
	
	//SPI--I0初始化
	RCC->AHB1ENR |= 0X01;//PB
	GPIOB->MODER &=~ ((0X03<<6)|(0X03<<8)|(0X03<<10));
	GPIOB->MODER |= ((0X02<<6)|(0X02<<8)|(0X02<<10));//复用功能
	GPIOB->OSPEEDR &=~ ((0X03<<6)|(0X03<<10));
	GPIOB->OSPEEDR |= ((0X02<<6)|(0X02<<10));//输出速度50Mhz
	GPIOB->PUPDR &=~ ((0X03<<6)|(0X03<<8)|(0X03<<10));//浮空
	 GPIOB->OTYPER &= ~((0X01<<3)|(0X01<<5));
	GPIOB->AFR[0] &=~ ((0X0F<<12)|(0X0F<<20)|(0X0F<<16));
	GPIOB->AFR[0] |= ((0X05<<12)|(0X05<<20)|(0X05<<16));//选择spi1复用功能
	Flash_Io_Init();
	
	//SPI1寄存器配置
	RCC->APB2ENR |= 0X01<<12;
	SPI1->CR1 &=~ ((0x01<<15)|(0x01<<11)|(0x01<<10)|(0X01<<7));//双线单向模式;数据帧八位;先发高位	
	SPI1->CR1 |= ((0X01<<9)|(0X01<<8)|((0X01<<2)));//软件管理主从模式;主模式;主配置
	SPI1->CR1 &=~ (0X07<<3);//波特率42M
	SPI1->CR1 &=~ (0X03<<0);//时钟极性，相位选择模式0
	SPI1->CR2  &=~ (0X01<<4);//SPI Motorola模式
	SPI1->I2SCFGR  &=~(0X01<<11);//选择SPI模式
	SPI1->CR1|= 0X01<<6;//SPI 使能
}

uint8 Flash_RW_Byte(uint8 data)
{
	while(!(SPI1->SR&(0X01<<1)));
	SPI1->DR = data;
	while(!(SPI1->SR&(0X01<<0)));
	data =SPI1->DR ;
	return data;
}
#endif



/********************************************************************************************
*  函 数 名： Flash_Write_Addr
*  描    述:  Flash写入24位地址
*  输入参数： 24位地址   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_Write_Addr(uint32 Addr)
{
	uint8 *temp = (uint8 *)&Addr;
	int8 i;
	for(i=2;i>=0;i--)
	{
		Flash_RW_Byte(*(temp+i));
	}
}
/*******************************************************************************************
*******************************以下为FLASH命令**********************************************
********************************************************************************************/


/********************************************************************************************
*  函 数 名： Flash_CMD_Read_ID
*  描    述:  Flash读取16位ID,若读取值为OXEF16则说明Flash器件正常
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_CMD_Read_ID(void)
{
	uint16 temp = 0;
	FLASH_CS_OL;//CS有效，选中Flash器件，Flash可与MCU通讯
	Flash_RW_Byte(FLASH_CMD_DEVICE_ID);
	Flash_Write_Addr(0x00);
	temp = Flash_RW_Byte(DUMMY_BYTE)<<8;
	temp |= Flash_RW_Byte(DUMMY_BYTE);
	FLASH_CS_OH;//CS无效，释放Flash器件，Flash不可与MCU通讯
	printf("ID:0X%X\r\n",temp);
}

void Flash_CMD_Write_Enable(void)
{
	FLASH_CS_OL;//CS有效，选中Flash器件，Flash可与MCU通讯
	Flash_RW_Byte(FLASH_CMD_WRITE_ENABLE);
	FLASH_CS_OH;//CS无效，释放Flash器件，Flash不可与MCU通讯
}
void Flash_CMD_Write_Disable(void)
{
	FLASH_CS_OL;//CS有效，选中Flash器件，Flash可与MCU通讯
	Flash_RW_Byte(FLASH_CMD_WRITE_DISABLE);
	FLASH_CS_OH;//CS无效，释放Flash器件，Flash不可与MCU通讯
}
uint8 Flash_Read_Status_Reg1(void)
{
	uint8 Status_Reg1;
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_READ_STATUS_REG1);
	Status_Reg1 = Flash_RW_Byte(DUMMY_BYTE);
	FLASH_CS_OH;
	return Status_Reg1;
}
uint8 Flash_Read_Status_Reg2(void)
{
	uint8 Status_Reg2;
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_READ_STATUS_REG2);
	Status_Reg2 = Flash_RW_Byte(DUMMY_BYTE);
	FLASH_CS_OH;
	return Status_Reg2;
}
uint8 Flash_Write_Status_Reg(uint16 Reg_value)
{
	Flash_CMD_Write_Enable();
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_WRITE_STATUS_REG);
	Flash_RW_Byte(Reg_value);
	Flash_RW_Byte((Reg_value>>8)&0X03);
	FLASH_CS_OH;
	return Flash_Wait_Busy(FLASH_WRITE_STATUS_REG_TIME_OUT);
}
uint8 Flash_Wait_Busy(uint16 Time_Out)
{
	uint16 delay = Time_Out;
	uint8 status_reg1 = 0;
	do{
		status_reg1 = Flash_Read_Status_Reg1();
		delay_ms(1);
		delay--;
	}while((status_reg1&0X01)&&delay);
	if(delay)
	{
		return 0;
	}
	return 1;	
}
/********************************************************************************************
*  函 数 名： Flash_Sector_Erase
*  描    述:  Flash单个扇区擦除
*  输入参数： uint32 ADDR扇区地址，不需要是扇区首地址，该地址处于哪个扇区则擦除哪个扇区
*  输出参数： 无
*  返 回 值:  返回0表示正常擦除，返回1表示异常
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
uint8 Flash_Sector_Erase(uint32 ADDR)
{
	Flash_CMD_Write_Enable();
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_SECTOR_ERASE);
	Flash_Write_Addr(ADDR);
	FLASH_CS_OH;
	return Flash_Wait_Busy(FLASH_SECTOR_ERASE_TIME_OUT);
}
/********************************************************************************************
*  函 数 名： Flash_Sector_Erase
*  描    述:  Flash多个个扇区擦除
*  输入参数： uint32 ADDR扇区地址，不需要是扇区首地址，该地址处于哪个扇区则擦除哪个扇区
			  uint16 num擦除扇区个数（num<=2048），擦除ADDR所在扇区，以及其后相邻的num-1个扇区
*  输出参数： 无
*  返 回 值:  返回0表示正常擦除，返回1表示异常
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_NSector_Erase(uint32 ADDR,uint16 num)
{
	uint16 i;
	for(i = 0;i < num;i ++)
	{
		Flash_Sector_Erase(ADDR+(i*FLASH_SECTOR_SIZE));
	}
}
/********************************************************************************************
*  函 数 名： Flash_Write_Page
*  描    述:  Flash写入数据(不带擦除，不跨页)
*  输入参数： uint8 *data写入的数组, uint32 nbyte数组长度, uint32 ADDR写入的起始地址
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_Write_Single_Page(uint8 *data, uint16 nbyte, uint32 ADDR)
{
	uint16 i,len;	
	len = FLASH_CALCULATE_PAGE_1ST_ADDR(ADDR) + FLASH_PAGE_SIZE - ADDR;
	if(nbyte < len)
	{
		len = nbyte;
	}
	Flash_CMD_Write_Enable();
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_WRITE_DATA);//写命令
	Flash_Write_Addr(ADDR);				//写地址
	for(i = 0;i<len;i++)
	{
		Flash_RW_Byte(*(data+i));		//写数据
	}
	FLASH_CS_OH;
	Flash_Wait_Busy(FLASH_PEGE_PROGARM_TIME_OUT);
}
/********************************************************************************************
*  函 数 名： Flash_Write_Data
*  描    述:  Flash写入数据(不带擦除，可跨页)
*  输入参数： uint8 *data写入的数组, uint32 nbyte数组长度, uint32 ADDR写入的起始地址
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_Write_Cross_Page(uint8 *data, uint32 nbyte, uint32 ADDR)
{
	uint16 i,len;	
	while(nbyte)
	{
		len = FLASH_CALCULATE_PAGE_1ST_ADDR(ADDR) + FLASH_PAGE_SIZE - ADDR;
		if(nbyte < len)
		{
			len = nbyte;
		}
		Flash_CMD_Write_Enable();
		FLASH_CS_OL;
		Flash_RW_Byte(FLASH_CMD_WRITE_DATA);//写命令
		Flash_Write_Addr(ADDR);				//写地址
		for(i = 0;i<len;i++)
		{
			Flash_RW_Byte(*(data+i));		//写数据
			//printf("%d ",*(data+i));
		}
		FLASH_CS_OH;
		Flash_Wait_Busy(FLASH_PEGE_PROGARM_TIME_OUT);
		ADDR += len;
		data += len;
		nbyte -=len; 	
	}
	FLASH_CS_OH;
	//printf("写数据执行完毕\n");
}
/********************************************************************************************
*  函 数 名： Flash_Read_Data
*  描    述:  Flash读取数据
*  输入参数： uint8 *data读取数据存储的数组, uint32 nbyte数组长度, uint32 ADDR读取的起始地址
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-31
*  注意事项:  无
********************************************************************************************/
void Flash_Read_Data(uint8 *data, uint32 nbyte, uint32 ADDR)
{
	uint32 i;
	FLASH_CS_OL;
	Flash_RW_Byte(FLASH_CMD_READ_DATA);
	Flash_Write_Addr(ADDR);
	for(i = 0;i<nbyte;i++)
	{
		*(data+i) = Flash_RW_Byte(DUMMY_BYTE);
	}
	FLASH_CS_OH;
	//printf("读数据执行完毕\n");
}


/********************************************************************************************
*  函 数 名： Flash_Copy_Sector
*  描    述:  扇区拷贝
*  输入参数： 
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-1
*  注意事项:  无
********************************************************************************************/
void Flash_Copy_Sector(uint32 ADDR_Src,uint32 ADDR_Dst)
{
	uint8 i;
	uint8 data_buff[FLASH_PAGE_SIZE];
	ADDR_Src = FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR_Src);
	ADDR_Dst = FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR_Dst);
	for(i=0;i<FLASH_PAGE_NUM_OF_EACH_SECTOR;i++)
	{
		Flash_Read_Data(data_buff,FLASH_PAGE_SIZE,ADDR_Src);
		Flash_Write_Single_Page(data_buff,FLASH_PAGE_SIZE,ADDR_Dst);
		ADDR_Src += FLASH_PAGE_SIZE;
		ADDR_Dst += FLASH_PAGE_SIZE;
	}
}
/********************************************************************************************
*  函 数 名： Flash_Copy_Date
*  描    述:  任意地址拷贝
   思    路：（1）首先计算目标地址所在页，剩余存储字节数len
			 （2）读取源地址len个字节数据，写入目标地址
			 （3）源地址更新（ADDR_Src+=len），
				  目标地址更新（ADDR_Dst+=len），
				  拷贝字节数更新（nbyte-=len）。
*  输入参数： uint32 ADDR_Src源地址,uint32 ADDR_Dst目标地址,uint32 nbyte拷贝字节数
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-1
*  注意事项:  无
********************************************************************************************/
void Flash_Copy_Date(uint32 ADDR_Src,uint32 ADDR_Dst,uint32 nbyte)
{
	uint16 len;
	uint8 data_buff[FLASH_PAGE_SIZE];
	while(nbyte)
	{
		len = FLASH_CALCULATE_PAGE_1ST_ADDR(ADDR_Dst) + FLASH_PAGE_SIZE - ADDR_Dst;
		if(nbyte < len)
		{
			len = nbyte;
		}
		Flash_Read_Data(data_buff,len,ADDR_Src);
		Flash_Write_Single_Page(data_buff,len,ADDR_Dst);
		ADDR_Src += len;
		ADDR_Dst += len;
		nbyte -= len;
	}
}
/********************************************************************************************
*  函 数 名： Flash_Write_Sector
*  描    述:  写一个扇区，不跨区,带擦除
   思    路： （1）和单页写类似，先计算该扇区还剩len个字节
			  （2）如果要写入字节数nbyte小于len则写入nbyte个字节
			  （3）如果要写入字节数nbyte大于len则写入len个字节
*  输入参数： 
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-1
*  注意事项:  无
********************************************************************************************/
void Flash_Write_Single_Sector(uint8 *data, uint32 nbyte, uint32 ADDR)
{
	uint32 ADDR_Backup = FLASH_BACKUP_SECTOR;
	uint16 offset = FLASH_CALCULATE_SECTOR_ADDR_OFFSET(ADDR);
	uint16 len = FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR) + FLASH_SECTOR_SIZE - ADDR;//扇区剩余空间
	if(nbyte<len)
	{/*写入字节数不能超过扇区剩余空间，若超过，则只写满剩余空间*/
		len = nbyte;
	}
	if(0==offset)//写入字节地址段位于扇区首
	{
		Flash_Sector_Erase(ADDR);//擦除原扇区
		Flash_Write_Cross_Page(data,len,ADDR);
	}
	else if((offset+len-1)==FLASH_SECTOR_SIZE)//写入字节地址段位于扇区尾
	{
		Flash_Sector_Erase(ADDR_Backup);//擦除备份扇区
		Flash_Copy_Sector(ADDR,ADDR_Backup);//备份原扇区数据
		Flash_Sector_Erase(ADDR);//擦除原扇区
		Flash_Copy_Date(ADDR_Backup,FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR),offset-1);
		Flash_Write_Cross_Page(data,len,ADDR);
	}
	else//写入字节地址段位于扇区中间
	{
		Flash_Sector_Erase(ADDR_Backup);//擦除备份扇区
		Flash_Copy_Sector(ADDR,ADDR_Backup);//备份原扇区数据
		Flash_Sector_Erase(ADDR);//擦除原扇区
		Flash_Copy_Date(ADDR_Backup,FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR),offset-1);
		Flash_Write_Cross_Page(data,len,ADDR);
		Flash_Copy_Date(ADDR_Backup+offset+len,ADDR+len,FLASH_SECTOR_SIZE-offset-len+1);
	}
	//printf("不跨扇区写数据执行完毕\n");
}
/********************************************************************************************
*  函 数 名： Flash_Write_Sector
*  描    述:  写一个扇区，跨扇区,带擦除
*  输入参数： 
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-08-1
*  注意事项:  无
********************************************************************************************/
void Flash_Write_Cross_Sector(uint8 *data, uint32 nbyte, uint32 ADDR)
{
	uint16 len;	
	while(nbyte)
	{
		len = FLASH_CALCULATE_SECTOR_1ST_ADDR(ADDR) + FLASH_SECTOR_SIZE - ADDR;//扇区剩余空间
		if(nbyte < len)
		{
			len = nbyte;
		}
		Flash_Write_Single_Sector(data,len,ADDR);
		ADDR += len;
		data += len;
		nbyte -=len; 	
	}
	delay_ms(7);
	//printf("跨扇区写数据执行完毕\n");
}
