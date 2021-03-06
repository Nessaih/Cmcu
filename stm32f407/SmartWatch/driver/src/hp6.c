
#include <stdbool.h>
#include <stdint.h>
#include "hp6.h"
#include "hp6_IIC.h"
#include "delay.h"
#include "usart.h"
#include "oled_screen.h"
#include "bmp.h"

const uint16_t crc16_tab[256] =
{/**crcУ????*/
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,   
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,   
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,   
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,   
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,   
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,   
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,   
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,   
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,   
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,   
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,   
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,   
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,   
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,   
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,   
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,  
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,   
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,   
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,   
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,   
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,   
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,   
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,   
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,   
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,   
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,   
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,   
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,   
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,   
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,   
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};
/****************************************************************
* ?궨??
*/
#define PROTOCLO_HEADER			0xc8d7b6a5
#define PROTOCLO_CMD_INDEX		4
#define PROTOCLO_DATA_INDEX		5
#define PROTOCLO_BUF_MAX_LEN	24

#define HP_6_IIC_ADDRESS 		0x66
#define HP_6_READ_BIT       (0x01)        //!< If this bit is set in the address field, transfer direction is from slave to master.





/****************************************************************
* ȫ?ֱ???
*/

/****************************************************************
* ???ؾ?̬????
*/
static uint8_t tx_buf[PROTOCLO_BUF_MAX_LEN] = {0};
static uint8_t rx_buf[PROTOCLO_BUF_MAX_LEN] = {0};	

//????Ѫѹ????
const uint8_t cmd_bp_open[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//?ر?Ѫѹ????
const uint8_t cmd_bp_close[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//??ȡѪѹ????????
const uint8_t cmd_bp_result[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//???????ʲ???
const uint8_t cmd_rate_open[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//?ر????ʲ???
const uint8_t cmd_rate_close[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//??ȡ???ʲ???????
const uint8_t cmd_rate_result[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//??ȡADC ????
const uint8_t cmd_get_adc[]=
{0xc8,0xd7,0xb6,0xa5,0x91,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//???õ͹???
const uint8_t cmd_set_powersaving[]=
{0xc8,0xd7,0xb6,0xa5,0x70,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//??ȡ?汾??Ϣ
const uint8_t cmd_get_version[]=
{0xc8,0xd7,0xb6,0xa5,0xa2,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/****************************************************************
* ???غ???????
*/
/****************************************************************
** Function name:			Hp_6_PowerUp
** Descriptions:      EN?ſ???
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
void Hp_6_init(void)
{
	RCC->AHB1ENR |= 1<<2;			    //PC?˿?ʱ??ʹ?ܡ? ??????
	
	GPIOC->MODER &= ~(3 << 26); 	//32bit,????
	GPIOC->MODER |= 1 << 26;    	//????PC13 IO?ڣ?Ϊͨ??????????
	
	GPIOC->OTYPER &= ~(1 << 13);	  //????PC13Ϊ????????
	
	GPIOC->OSPEEDR &= ~(3 << 26);
	GPIOC->OSPEEDR |= 2 << 26;  	//????PC13 ???????ٶ? 50MHZ
	
	HP6IIC_Init();

  Hp_6_PowerON();	

}

/****************************************************************
** Function name:			Crc16
** Descriptions:      crcУ?? 
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:      2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
uint16_t Crc16(uint8_t *data,uint16_t len)
{
    uint16_t crc16 = 0xFFFF;
    uint32_t uIndex ; //CRC??ѯ??????
    while (len --)
    {
        uIndex = (crc16&0xff) ^ ((*data) & 0xff) ; //????CRC
				data = data + 1;
        crc16 = ((crc16>>8) & 0xff) ^ crc16_tab[uIndex];
    }
    return crc16 ;//????CRCУ??ֵ
}
/****************************************************************
** Function name:			Hp_6_I2CWrite
** Descriptions:       
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
bool Hp_6_I2CWrite(uint8_t Hp_6_address, uint8_t *pData, uint8_t bytes)
{
	uint8_t i;
	HP6IIC_Start();
	HP6IIC_WriteByte(Hp_6_address<<1);
	for(i=0; i<24; i++)
	{
		HP6IIC_WriteByte(*(pData+i));
	}
	HP6IIC_Stop();	
	return 0;
}
/****************************************************************
** Function name:			HP_6_Delay_ms
** Descriptions:            ??ʱ????????λ????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
bool Hp_6_I2CRead(uint8_t Hp_6_address, uint8_t *pData, uint8_t bytes)
{
  uint8_t i;
  HP6IIC_Start();
  HP6IIC_WriteByte((Hp_6_address<<1)|0x01);
  for(i=0;i<23;i++)
  {
    /* ?ӻ??ͻ????????? */
    *(pData+i) = HP6IIC_ReadByte(0);      
  }
  /* ?ӻ??ͻ????????? */
  *(pData+23) = HP6IIC_ReadByte(1);//ע?⣺??ȡbyte[23]????????Ӧ??    
  HP6IIC_Stop();
	return 0;
}
/****************************************************************
** Function name:			HP_6_Delay_ms
** Descriptions:            ??ʱ????????λ????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
void HP_6_Delay_ms(uint16_t ms)
{
  uint16_t time_delay = ms;
  delay_ms(time_delay); //??ʱ????????λ????
}
/****************************************************************
** Function name:			HP_6_GetResultData
** Descriptions:            	??ȡ????????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint16_t HP_6_GetResultData(uint8_t *data)
{
  uint8_t i = 0;
  
  if(!data)
  {
    return 0;
  }
  
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    data[i] = rx_buf[i];
  }
  
  return PROTOCLO_BUF_MAX_LEN;
}
/****************************************************************
** Function name:			HP_6_SendCmd
** Descriptions:            	????????ӻ?????ȡ????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_SendCmd(uint8_t *tx_buf, uint8_t *rx_buf)
{
  uint8_t state = HP_6_SUCCESS;
  uint16_t crc;	
  
  crc = Crc16(&tx_buf[4], 18);	//С????
  *(uint16_t*)(&tx_buf[22]) = crc;
  
  Hp_6_I2CWrite(HP_6_IIC_ADDRESS, tx_buf, 24);
  delay_ms(5);
  Hp_6_I2CRead(HP_6_IIC_ADDRESS, rx_buf, 24);
  
  crc = *(uint16_t*)(&rx_buf[22]);	//С????
  if(crc != Crc16(&rx_buf[4], 18))	//У??ʧ??
  {
    state = HP_6_FAILURE;
  }

  return state;
}

/****************************************************************
** Function name:			HP_6_GetRateResult
** Descriptions:            	??ȡ???ʲ???????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetRateResult(void)
{
  uint32_t i;	
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_result[i];		
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);   
}

/****************************************************************
** Function name:			HP_6_OpenRate
** Descriptions:            	???????ʲ???
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_OpenRate(void)
{
  uint32_t i;
  Hp_6_PowerON();
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_open[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);       
}

/****************************************************************
** Function name:			HP_6_OpenRate
** Descriptions:            	?ر????ʲ???
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_CloseRate(void)
{
  uint32_t i;
  uint8_t state = HP_6_SUCCESS;
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_close[i];
  }
  
  state = HP_6_SendCmd( tx_buf, rx_buf);       
	Hp_6_PowerOFF();
	return state;
}

/****************************************************************
** Function name:			HP_6_GetBpResult
** Descriptions:            	??ȡѪѹ????????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetBpResult(void)
{
  uint32_t i;	
  
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_result[i];	
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);  
}

/****************************************************************
** Function name:			HP_6_OpenBp
** Descriptions:            	???????ʲ???
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_OpenBp(void)
{
  uint32_t i;
  Hp_6_PowerON();
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_open[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);   
	
}

/****************************************************************
** Function name:			HP_6_CloseBp
** Descriptions:            	?ر?Ѫѹ????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_CloseBp(void)
{
  uint32_t i;
  uint8_t state = HP_6_SUCCESS;
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_close[i];
  }
  
  state = HP_6_SendCmd( tx_buf, rx_buf);     
  Hp_6_PowerOFF();
  return state; 	
}

/****************************************************************
** Function name:			HP_6_GetADC
** Descriptions:            	?ر?Ѫѹ????
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetADC(uint8_t num, uint8_t id)
{
  uint32_t i;
  uint16_t crc;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_get_adc[i];
  }
  tx_buf[PROTOCLO_DATA_INDEX] =  num;
  tx_buf[PROTOCLO_DATA_INDEX+1] =  id;
  
  crc = Crc16(&tx_buf[4],18);	
  *(uint16_t*)(&tx_buf[22]) = crc;
  
  return HP_6_SendCmd( tx_buf, rx_buf);      
}

/****************************************************************
** Function name:			HP_6_PowerSaving
** Descriptions:            ???õ͹???
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_PowerSaving(void)
{
  uint32_t i;
  
  for(i=0; i<PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_set_powersaving[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);
}

/****************************************************************
** Function name:			HP_6_VersionInfo
** Descriptions:            ??ȡ?汾??Ϣ
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/  
uint8_t HP_6_VersionInfo(void)
{
  uint32_t i;
  
  for(i=0; i<PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_get_version[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);	    
}

uint8 HP_6_Measurement_rate(void )
{
	static uint8 heart_rate_open_flag =1;
	uint8 data[24];
	char str[10];
	uint8 Index[3]={20,21,0};
	if(heart_rate_open_flag)
	{
		HP_6_OpenRate();
		delay_nms(1000);
		heart_rate_open_flag = 0;
	}
	if(data[7])  //???????Ի??????ʽ??????????????ɹ??????ر????ʲ???
	{
		HP_6_CloseRate();
		
		Number2String(data[7],str); // //byte 7?????ʽ???
		OLED_Show_Multi_Chinese(3,80,Index,3,normal);
		OLED_Show_String(5,90,str,CH_SIZE2,normal);
		memset(data,0,24);
		memset(tx_buf,0,24);
		memset(rx_buf,0,24);
		memset(str,0,10);
		heart_rate_open_flag = 1;//???³?ʼ????־???Ա???һ?δ򿪲???
		return 0;//????????
	}
	else //??????ȡ???????ʽ???????ÿ??1???ӻ?ȡ????ֵ
	{
		HP_6_GetRateResult();
		HP_6_GetResultData(data);
		//delay_nms(1000);
	}
	OLED_Show_Pic(8,8,61,61,(uint8 *)Pulse_bmp,normal);
	return 1;//????δ????
}




uint8 HP_6_Measurement_bp(void )
{
	static uint8 flag =1;
	uint8 data[24];
	char str[30];
	if(flag)
	{
		HP_6_OpenBp();
		delay_nms(5000);
		flag = 0;
	}
	HP_6_GetBpResult();
	HP_6_GetResultData(data);
	if(0 == data[7])                   //byte 7??Ѫѹ??????״̬:0?????У?1???????ɡ?2????ʧ??
	{
		OLED_Show_String(1,0,"testing",CH_SIZE2,normal);  
	}
	else if(1 == data[7])  
	{
		OLED_Show_String(0,0,"success",CH_SIZE2,normal);
		
		sprintf(str,"High:%03d        Low:%03d", data[10], data[11]);//byte 10??Ѫѹ?????????ĸ?ѹ,byte 11??Ѫѹ?????????ĵ?ѹ	
		OLED_Show_String(2,0,str,CH_SIZE2,normal);
		HP_6_CloseBp();
		memset(str,0,30);
		memset(data,0,24);
		flag = 1;//???³?ʼ????־???Ա???һ?δ򿪲???
		return 0;//????????
	}		 
	else if(2 == data[7])         
	{
		HP_6_CloseBp();
		OLED_Show_String(1,0,"fail   ",CH_SIZE2,normal);
	}		 
	else 
	{
		//delay_ms(1000);
	}
	
	return 1;//????δ????
}