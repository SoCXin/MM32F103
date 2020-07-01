#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    unsigned int uiCnt = 0;

    delay_init();	     //延时初始化
    uart_init(SystemCoreClock / 1000000, 115200); //串口初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    TIM1_Int_Init(10000 - 1, SystemCoreClock / 10000 - 1); //10Khz的计数频率，计数到10000为1s
    LED1 = 1;
    while(1)
    {
        for(uiCnt = 0; ; uiCnt++)
        {
            /* 等待定时器溢出 */
            Tim1_UPStatusOVCheck_test();
            /* 打印当前数值 */
            printf("0x%x\r\n", uiCnt);
            LED1 = !LED1;
        }
    }
}






















