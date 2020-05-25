#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

#define WK_UP PAin(0)	 	//PA0  WK_UP
#define KEY3  PCin(10)	 	//PC10
#define KEY4  PAin(15)	 	//PA15


void KEY_Init(void);		//IO初始化
u8 KEY_Scan(u8 mode);		//按键扫描函数
#endif
