#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
extern u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态
extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值

int main(void)
{
    u32 temp = 0;
    uart_initwBaudRate(115200);
    delay_init();	   	 			//延时初始化
    LED_Init();		  				//初始化与LED连接的硬件接口
    TIM2_Cap_Init(0XFFFF, SystemCoreClock / 1000000 - 1);		//以1Mhz的频率计数
    while(1)
    {
        delay_ms(10);
        LED0_PWM_VAL++;
        if(LED0_PWM_VAL == 300)LED0_PWM_VAL = 0;
        if(TIM2CH1_CAPTURE_STA & 0X80) //成功捕获到了一次高电平
        {
            temp = TIM2CH1_CAPTURE_STA & 0X3F;
            temp *= 65536;					//溢出时间总和
            temp += TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
            printf("HIGH:%d us\r\n", temp);	//打印总的高点平时间
            TIM2CH1_CAPTURE_STA = 0;			//开启下一次捕获
        }
    }
}
