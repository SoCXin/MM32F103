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
    printf("uart ok!\r\n");
    IWDG_Init(4, 625);

    while(1)//按下按键喂狗，LED1点亮，串口也不打印；否则不断复位，串口不断打印，LED1也熄灭
    {
        if(KEY_Scan(0) == WKUP_PRES)
        {
            IWDG_Feed();
            LED1 = 0;
        }
        delay_ms(10);
    }
}

























