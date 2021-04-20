#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "oled_screen.h"
#include "spi.h"
#include "rtc.h"
#include "sht20.h"
#include "iic.h"
#include "hp6.h"
#include "motor.h"
#include "menu.h"

#include "mpu6050.h"
#include "stepAlgorithm.h"
#include "watchInfo.h"
#include "bluetooth.h"


int main(void)
{
	
	
	
	/**********************************计步相关参数定义***********************************/
//	u8 err;
//	u8 uiBuf[40];
//	accValue_t accValue;
//	static sportsInfo_t userSportsInfo;
//	static u8 time_cnt;
//	static timeStamp_t timeStamp;
//	short aacx,aacy,aacz;									//加速度传感器原始数据
//	static u8 tempSecond;	

//	
//	RTC_Date rtcTime_daat;
//	RTC_Date *rtcTime = &rtcTime_daat;
	/**********************************计步相关参数定义***********************************/
	RTC_Date dt={18,8,16,4,9,30,0,0};
//	static uint8 flag =1;
//	uint8 data[24];
//	char str[10];
	uint8 heart_rate_flag =1,blood_pressure_flag =1;
	
	Led_Io_Init();
	Time10_Configuration();//延时函数定时器
	Usart1_Init();
	Key_Init();
	OLED_Init();
	RTC_Init();
	RTC_Setting(dt);
	//RTC_Setting_DT(dt);
	IIC_IO_Init();
	SHT20_Init();
	Hp_6_init();
	
	BT_init();
	BT_enable();

//	MPU_Init();
//	WatchInfo_init();
//	//MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	
	while(1)
	{

		Show_Main_Interface();
		if(Bluetooth_Control_Flag == 1)
		{//打开心率测量
			OLED_Clear();
			printf("打开心率测量");
			while(1)
			{
				
				if(heart_rate_flag)
				{
					heart_rate_flag = HP_6_Measurement_rate();
				}
				Key_Value = Key_Scan();
				if(Key_Value == KEY_LEFT)
				{
					OLED_Clear();
					HP_6_CloseRate();
					heart_rate_flag =1;
					Bluetooth_Control_Flag = 0;
					break;
				}

				if(Bluetooth_Control_Flag == 2)
				{
					OLED_Clear();
					printf("关闭心率测量");
					HP_6_CloseRate();
					heart_rate_flag =1;
					Bluetooth_Control_Flag = 0;
					break;
				}
			}
			
		}
		else if(Bluetooth_Control_Flag == 3)
		{//打开血压测量
			OLED_Clear();
			while(1)
			{
				
				if(blood_pressure_flag)
				{
					blood_pressure_flag = HP_6_Measurement_bp();
				}
				Key_Value = Key_Scan();
				if(Key_Value == KEY_LEFT)
				{
					OLED_Clear();
					HP_6_CloseBp();
					blood_pressure_flag =1;
					Bluetooth_Control_Flag = 0;
					break;
				}

				if(Bluetooth_Control_Flag == 4)
				{
					OLED_Clear();
					HP_6_CloseBp();
					blood_pressure_flag =1;
					Bluetooth_Control_Flag = 0;
					break;
				}
			}
		}

	}
}
