#ifndef __RTC_H_
#define __RTC_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "type.h"
#include "usart1.h"

#define RTC_ALARM_A 1
#define RTC_WAKEUP 1

typedef struct RD{
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 week;
	
	uint8 hour;
	uint8 minute; 
	uint8 second;
	uint8 pm;
}RTC_Date,*PRTC_Date;

extern char Week[15];

extern void RTC_Init(void);
extern uint8 BCD_encode(uint8 num,uint8 flag);
extern void RTC_Setting(void);
extern void RTC_Setting_DT(PRTC_Date rtc_dt);
extern RTC_Date Read_DT(void);
extern void Show_Time(RTC_Date dt);
extern void RTC_ALARM_A_Init(void);

#if  RTC_ALARM_A 
extern void RTC_ALARM_A_Init(void);
#endif
#if  RTC_WAKEUP 
extern void RTC_WakeUp_Init(void);
#endif


#endif
