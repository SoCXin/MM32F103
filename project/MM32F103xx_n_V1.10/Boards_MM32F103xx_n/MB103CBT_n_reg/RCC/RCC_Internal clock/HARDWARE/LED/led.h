#ifndef __LED_H
#define __LED_H
#include "sys.h"


//LED端口定义
#define LED0 PCout(6)	// PC6
#define LED1 PCout(7)	// PC7	
#define LED2 PCout(8)	// PC8	
#define LED3 PCout(9)	// PC9	

void LED_Init(void);	//初始化
#endif

















