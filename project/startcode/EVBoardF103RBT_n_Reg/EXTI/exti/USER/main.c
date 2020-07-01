#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"


int main(void)
{
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    EXTIX_Init();		//外部中断初始化
    while(1)
    {
    }
}

