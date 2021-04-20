#include "my_clock.h"
//暂时没发现问题2018-10-19 fw
uint32_t my_clock_second  = 0;
const uint8_t Leap_Year_Month_List[13] = {/*平年每月天数表，Month_List[0]无用*/0,
/*1月*/31,/*2月*/29,/*3月*/31,/*4 月*/30,/*5 月*/31,/*6 月*/30,
/*7月*/31,/*8月*/31,/*9月*/30,/*10月*/31,/*11月*/30,/*12月*/31,
};
const uint8_t Month_List[13] = {/*平年每月天数表，Month_List[0]无用*/0,
/*1月*/31,/*2月*/28,/*3月*/31,/*4 月*/30,/*5 月*/31,/*6 月*/30,
/*7月*/31,/*8月*/31,/*9月*/30,/*10月*/31,/*11月*/30,/*12月*/31,
}; 

//设置起始时间，需在程序开始运行式更改，否则为默认值
Data_Type Start_Time = {
    /*Start.second */  50, 
	/*Start.minute */  59, 
	/*Start.hour   */  23,
    /*Start.day    */  28, 
	/*Start.month  */  2, 
	/*Start.year   */  0,
	/*Start.week   */  0,
    /*year的高位20省略，取低位00*/
};
//标准日期时间，不可更改
const Data_Type Standard_Time = {
    /*Standard.second */ 0, 
	/*Standard.minute */ 0, 
	/*Standard.hour   */ 0,
    /*Standard.day    */ 1, 
	/*Standard.month  */ 1, 
	/*Standard.year   */ 0,
	/*Standard.week   */ 0
    /*year的高位20省略，取低位00*/
};
uint8_t Is_Leap_Year(uint16_t year)
{
    //判断某一年是不是闰年
    if(0 == year % 400)
    {
        return 1;
    }
    else if(0 == year % 4 && 0 != year % 100)
    {
        return 1;
    }
    return 0;
}
uint8_t Cal_Leap_Num(uint16_t year1,uint16_t year2)
{
    //计算两个年之间闰年个数,[min_year,max_year]
    uint16_t max_year,min_year,year;
    uint8_t leap_year_count = 0;//2^32秒最多可计到136年（按每年365天算），所以0~255已经足够
    max_year = year1>year2?year1:year2;
    min_year = year1<year2?year1:year2;
    for(year = min_year; year <= max_year; year ++)
    {
        if(Is_Leap_Year(year))
        {
            leap_year_count ++;
        }
    }
    return leap_year_count;
}

uint32_t Cal_Second_Num(Data_Type start,Data_Type end)
{
    uint8_t i;
    uint8_t leap_year_num = 0;
    uint32_t sum_days,sum_hours,sum_minutes,sum_seconds;

    //1.计算天数
    sum_days = (end.year - start.year - 1)*365;
	leap_year_num = Cal_Leap_Num(2000+start.year,2000+end.year);
	sum_days += leap_year_num;
	if(2 >= end.month && 1 == Is_Leap_Year(end.year))//end在2月28号之前则加上闰年天数减1
	{
		if(2 == end.month)
		{
			sum_days -= end.day < 28? 0:1;
		}
		else
		{
			sum_days -= 0;
		}
	}
    for(i = start.month + 1; i <= 12; i++)
    {
        sum_days += Month_List[i];
    }
    switch(start.month)
    {
        //31天
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        sum_days += (31-start.day); break;
        //30天
        case 4:
        case 6:
        case 9:
        case 11:
        sum_days += (30-start.day); break;
        //28天
        case 2:
        sum_days += (28-start.day); break;
        default :break;
    }
    for(i = 1; i <= end.month - 1; i++)
    {
        sum_days += Month_List[i];
    }
    sum_days += (end.day - 1);
    
    //2.计算小时
    sum_hours = sum_days * 24;
    sum_hours += (23 - start.hour);
    sum_hours += end.hour;

    //3.计算分钟
    sum_minutes = sum_hours * 60;
    sum_minutes += (59 - start.minute);
    sum_minutes += end.minute;

    //4.计算秒
    sum_seconds = sum_minutes * 60;
    sum_seconds += (60 - start.second);
    sum_seconds += end.second;
    return sum_seconds;
}
uint32_t Cal_Standard_Second(uint32_t second)
{
    /*1.计算设置起始日期时间（star_time）与标准日期时间的秒差值；
     *2.返回秒差值与当前计数秒值之和。
     *3.标准日期时间：2000年1月1日0时0分0秒
     */
    uint32_t stan_second = 0;
    stan_second = Cal_Second_Num(Standard_Time, Start_Time);
    stan_second += second;
    return stan_second;
}

PData_Type Get_Current_Time(uint32_t second)
{
	Data_Type dt;
    PData_Type pdt = &dt;
    uint8_t j,continue_flag = 1;
    uint8_t temp_second, temp_minute, temp_hour, temp_month = 1;
    uint16_t temp_year = 2000+Standard_Time.year;
    uint32_t temp_day;

	second = Cal_Standard_Second(second);
    temp_second = second % 60;          //[0,59]
    temp_minute = second / 60 % 60;     //[0,59]
    temp_hour   = second / 3600 % 24;   //[0,23]
    temp_day    = second / 86400 + 1;   //[1,+∞]
    dt.second = temp_second;
    dt.minute = temp_minute;
    dt.hour   = temp_hour;
	
    while(continue_flag)
    {
       if(Is_Leap_Year(temp_year))//如果今年是闰年
       {
            if(366 < temp_day)
            {
                temp_year ++;
                temp_day -= 366;
            }
            else
            {
                for(j = 1; j <= 12; j++)
                {
                    if(temp_day > Leap_Year_Month_List[j])
                    {
                        temp_month ++;
                        temp_day -= Leap_Year_Month_List[j];
                    }
                    else
                    {
                        continue_flag = 0;
                        break;
                    }
                }
            }
       }
       else
       {
            if(355 < temp_day)
            {
                temp_year ++;
                temp_day -= 365;
            }
            else
            {
                for(j = 1; j <= 12; j++)
                {
                    if(temp_day > Month_List[j])
                    {
                        temp_month ++;
                        temp_day -= Month_List[j];
                    }
                    else
                    {
                        continue_flag = 0;
                        break;
                    }
                }
            }
       }
    }
    dt.day   = temp_day;
    dt.month = temp_month;
    dt.year  = temp_year-2000;
    dt.week  = 0;//星期暂时未计算 
    return pdt;
}
PData_Type Get_Time(void)
{
	return Get_Current_Time(my_clock_second);
}
void my_clock_second_updata(void)
{
	my_clock_second ++;
}

void Set_Time()
{
	//
}

#ifdef __UART_H__
void Print_Time(void)
{
	Data_Type dt;
	PData_Type pdt = &dt;
	static uint8_t old_value = 99;
	
	pdt = Get_Time();
	if(old_value != pdt->second)
	{
		old_value = pdt->second;
		printf("时间:20%02d-%02d-%02d %02d:%02d:%02d\n",\
			pdt->year,pdt->month,pdt->day,pdt->hour,pdt->minute,pdt->second);
	}
}
#endif
