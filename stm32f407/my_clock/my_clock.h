#ifndef __MY_CLOCK__
#define __MY_CLOCK__
#include "SWM181.h"
#include "uart.h"
typedef struct data_type{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t week;
}Data_Type,*PData_Type;
PData_Type Get_Current_Time(uint32_t second);
PData_Type Get_Time(void);
void my_clock_second_updata(void);

#ifdef __UART_H__
void Print_Time(void);
#endif
#endif //__MY_CLOCK__
