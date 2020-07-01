#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "wdg.h"


int main(void)
{
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    KEY_Init();          //按键初始化
    delay_ms(300);   	 //让人看得到灭
    IWDG_Init(4, 625);   //与分频数为64,重载值为625,溢出时间为1s
    LED1 = 0;				 //点亮LED1
    while(1)
    {
        if(KEY_Scan(0) == WKUP_PRES)
            IWDG_Feed();//如果WK_UP按下,则喂狗
        delay_ms(100);
    }

}

