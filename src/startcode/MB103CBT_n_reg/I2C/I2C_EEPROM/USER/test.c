#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "i2c.h"
int main(void)
{

    delay_init();	     	//延时初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    uart_init(SystemCoreClock / 1000000, 115200);
    I2CMasterTest();
    while(1)
    {
    }
}

























