#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    delay_init();	     	//延时初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(400);
    }
}

























