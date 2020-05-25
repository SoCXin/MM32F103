/**
******************************************************************************
* @file    rtc.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the rtc inc file for the library.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/
#ifndef __RTC_H
#define __RTC_H
#include "sys.h"

//开发板
//RTC实时时钟 驱动代码

//时间结构体
typedef struct
{
    vu8 hour;
    vu8 min;
    vu8 sec;
    //公历日月年周
    vu16 w_year;
    vu8  w_month;
    vu8  w_date;
    vu8  week;
} _calendar_obj;
extern _calendar_obj calendar;	//日历结构体

extern u8 const mon_table[12];	//月份日期数据表
void Disp_Time(u8 x, u8 y, u8 size); //在制定位置开始显示时间
void Disp_Week(u8 x, u8 y, u8 size, u8 lang); //在指定位置显示星期
u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Get(void);         //更新时间
u8 RTC_Get_Week(u16 year, u8 month, u8 day);
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec); //设置时间
#endif

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

