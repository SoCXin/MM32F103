#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "sleep.h"
int main(void)
{

    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    uart_init(SystemCoreClock / 1000000, 115200);
    printf("sleep mode ! \r\n");
    sleep_test();

    //按下K2，被唤醒
    while(1)//无限循环
    {
        printf("Wake Up ! \r\n");
        delay_ms(1000);
    }
}





























