#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "bkp.h"
int main(void)
{
    u8 i;

    delay_init();	     	//延时初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    uart_init(SystemCoreClock / 1000000, 115200);
    i = BKP_DATA();		 //往BKP写数据，如果成功则LED满闪10下，失败则快速闪烁

    if(i == 0) //写数据正确，LED闪烁5下
    {
        for(i = 0; i < 10; i++)
        {
            LED1 = !LED1;
            LED2 = !LED2;
            LED3 = !LED3;
            LED4 = !LED4;
            delay_ms(400);
        }
    }
    else//写数据不正确，LED极速闪烁
    {
        while(1)
        {
            LED1 = !LED1;
            LED2 = !LED2;
            LED3 = !LED3;
            LED4 = !LED4;
            delay_ms(50);
        }
    }

    while(1)
    {
    }
}
























