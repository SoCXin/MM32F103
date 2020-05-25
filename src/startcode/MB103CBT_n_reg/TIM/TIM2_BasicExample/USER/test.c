#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
unsigned int ucTim2Flag;//中断计数标志位
int main(void)
{

    unsigned int uiCnt = 0;
    delay_init();	     //延时初始化
    uart_init(SystemCoreClock / 1000000, 115200); //串口初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    TIM2_Int_Init(1000 - 1, SystemCoreClock / 1000000 - 1); //1Mhz的计数频率，计数到1000为1ms,采用中断方式计数
    LED1 = 1;
    while(1)
    {
        if(ucTim2Flag > 999)
        {
            ucTim2Flag = 0;

            /* 打印当前uiCnt数值 */
            printf("Time=0x%xs\r\n", uiCnt);
            uiCnt++;
            LED1 = !LED1;

        }
    }
}






















