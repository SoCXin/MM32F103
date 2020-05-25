#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

int main(void)
{

    delay_init();	     	//延时初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    LED2 = !LED1;
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        delay_ms(300);
    }
}

























