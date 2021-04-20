#include "menu.h"
#include "oled_screen.h"
#include "key.h"
#include "ui.h"
#include "rtc.h"
#include "sht20.h"
#include "mpu6050.h"
#include "hp6.h"
#include "bluetooth.h"
#include "type.h"

static uint8 Cursor[2][3]= {1,0,0,0,0,0};//光标位置
uint8 *pCursor = (uint8 *)Cursor;
void Show_Main_Interface(void)
{
	uint8 i;
	
	uint8 *pCursor = (uint8 *)Cursor;
	uint8 x_offset = 16,y_offset = 0,x_gep=8,y_gep=8;//这几个参数不能随意设置，不然可能显示不正常
	uint8 heart_rate_flag =1,blood_pressure_flag =1;
	
	
	Key_Value = Key_Scan();
	switch(Key_Value)
	{
		case KEY_CENTRE:
		{//进入子菜单
			for(i=0;i<6;i++)
			{
				if(pCursor[i] != 0)
				{
					switch(i)
					{
						case Step_Count:
						{
							OLED_Clear();
							while(1)
							{
								MPU_Show();
								Key_Value = Key_Scan();
								if(Key_Value == KEY_LEFT)
								{
									OLED_Clear();
									break;
								}
							}
						}break;
						case Blue_Tooth:
						{
							OLED_Clear();
							while(1)
							{
								Bluetooth_Show();
								Key_Value = Key_Scan();
								if(Key_Value == KEY_LEFT)
								{
									OLED_Clear();
									break;
								}
							}
							
						}break;	
						case Data_Time:
						{
							OLED_Clear();
							while(1)
							{
								Show_Time();
								Key_Value = Key_Scan();
								if(Key_Value == KEY_LEFT)
								{
									OLED_Clear();
									break;
								}
							}
							
						}break;
						case Temp_Humi:
						{
							OLED_Clear();
							while(1)
							{
								SHT20_Show();
								Key_Value = Key_Scan();
								if(Key_Value == KEY_LEFT)
								{
									OLED_Clear();
									break;
								}
							}
						}break;
						case Heart_Rate:
						{
							OLED_Clear();
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
									break;
								}
							}
						}break;
						case Blood_Pressure:
						{
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
									blood_pressure_flag = 1;
									break;
								}
							}
							
						}break;
						default:break;
						
					}
				}
			}
		}break;
		case KEY_LEFT:
		{
			for(i=0;i<6;i++)
			{
				if(pCursor[i] != 0)
				{
					if(i > 0)
					{
						pCursor[i] = 0;
						pCursor[i-1] = 1;
						break;
					}
				}
			}
		}break;
		case KEY_RIGHT:
		{
			for(i=0;i<6;i++)
			{
				if(pCursor[i] != 0)
				{
					if(i < 5)
					{
						pCursor[i] = 0;
						pCursor[i+1] = 1;
						break;
					}
				}
			}
		}break;
		case KEY_UP:
		{
			for(i=0;i<6;i++)
			{
				if(pCursor[i] != 0)
				{
					if(i > 2)
					{
						pCursor[i] = 0;
						pCursor[i-3] = 1;
						break;
					}
				}
			}
		}break;
		case KEY_DOWN:
		{
			for(i=0;i<6;i++)
			{
				if(pCursor[i] != 0)
				{
					if(i < 3)
					{
						pCursor[i] = 0;
						pCursor[i+3] = 1;
						break;
					}
				}
			}
		}break;
		default:break;
	}
	for(i=0;i<6;i++)
	{
		if(pCursor[i] != 0)
		{
			OLED_Show_Pic(x_offset+(i%3)*(26+x_gep),y_offset+(i/3)*(26+y_gep),x_offset+(i%3)*(26+x_gep)+25,y_offset+(i/3)*(26+y_gep)+25,(uint8 *)ui[i],inverse);
		}
		else
		{
			OLED_Show_Pic(x_offset+(i%3)*(26+x_gep),y_offset+(i/3)*(26+y_gep),x_offset+(i%3)*(26+x_gep)+25,y_offset+(i/3)*(26+y_gep)+25,(uint8 *)ui[i],normal);
		}
	}
}
