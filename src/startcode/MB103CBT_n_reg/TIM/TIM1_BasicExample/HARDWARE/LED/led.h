#ifndef __LED_H
#define __LED_H
#include "sys.h"


//LED端口定义
#define LED4 PAout(15)	// PA15
#define LED3 PBout(3)	// PB3
#define LED2 PBout(4)	// PB4
#define LED1 PBout(5)	// PB5

void LED_Init(void);	//初始化
#endif

















