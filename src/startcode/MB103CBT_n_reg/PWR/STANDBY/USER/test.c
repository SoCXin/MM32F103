#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "wkup.h"

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，按下按键WAKE UP
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{

    uart_init(SystemCoreClock / 1000000, 115200);	 	//串口初始化为115200
    printf("\r\n请按下wake up按键3秒不放\r\n");
    delay_init();	   	 	//延时初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    LED1 = 0;
    WKUP_Init();			//初始化WK_UP按键，同时检测是否正常开机？
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(250);//延时250ms
        printf("已从standby模式唤醒\r\n");
    }
}




