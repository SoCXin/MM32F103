#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"


int main(void)
{
    u8 t = 0;
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
    LED1 = 0;					//点亮LED
    while(1)
    {
        t = KEY_Scan(0);		//得到键值
        switch(t)
        {
            case KEY3_PRES:
                LED1 = !LED1;
                break;
            case KEY4_PRES:
                LED2 = !LED2;
                break;
            case WKUP_PRES:
                LED2 = !LED2;
                LED1 = !LED1;
                break;
            default:
                delay_ms(10);
        }
    }
}

