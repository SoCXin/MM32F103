#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "wdg.h"
#include "key.h"
int main(void)
{
    delay_init();	     	//延时初始化
    KEY_Init();
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    LED1 = 1;
    uart_init(SystemCoreClock / 1000000, 115200); //串口初始化
    printf("wwdg test\r\n\r\n");
    WWDG_Init(0X7F, 0X5F, 3); //计数器值为7f,窗口寄存器为5f,分频数为8
    while(1)
    {
        LED2 = !LED2;
        delay_ms(300);
    }


}
























