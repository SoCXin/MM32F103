#include "delay.h"
#include "sys.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    delay_init();
    LED_Init();		  	//初始化与LED连接的硬件接口
    TIM3_Int_Init(4999, SystemCoreClock / 10000 - 1); //10Khz的计数频率，计数到5000为500ms
    while(1)
    {
        LED2 = !LED2;
        delay_ms(500);	//判断与LED1闪烁频率是否一致
    }

}

