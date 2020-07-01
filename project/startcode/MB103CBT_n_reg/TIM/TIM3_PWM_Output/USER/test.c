#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    u16 led0pwmval = 0;
    u8 dir = 1;

    delay_init();	     //延时初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    TIM3_PWM_Init(899, 0); //不分频

    while(1)//无限循环
    {
        delay_ms(4);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval > 500)dir = 0;
        if(led0pwmval == 0)dir = 1;
        TIM3->CCR1 = led0pwmval; //输出比较值
    }
}






















