#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "spi.h"
int main(void)
{

    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
    uart_init(SystemCoreClock / 1000000, 115200);
    SPIM_Test();
    while(1);
}

























