#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3
#define KEY4_PRES	4		//KEY4 

#define KEY1  PCin(13)	 	//PC13
#define WK_UP PAin(0)	 	//PA0  WK_UP
#define KEY3  PBin(10)	 	//PB10
#define KEY4  PBin(11)	 	//PB11


void KEY_Init(void);		//IO��ʼ��
u8 KEY_Scan(u8 mode);		//����ɨ�躯��
#endif
