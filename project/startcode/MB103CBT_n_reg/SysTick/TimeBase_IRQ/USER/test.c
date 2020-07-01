#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
/********************************************************************************************************
**函数信息 ：TimingDelay_Decrement(void)
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

/********************************************************************************************************
**函数信息 ：SysTick_Handler(void)
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/********************************************************************************************************
**函数信息 ：Delay(__IO uint32_t nTime)
**功能描述 ：程序应用调用延时
**输入参数 ：nTime：延时
**输出参数 ：无
********************************************************************************************************/
void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置

    LED_Init();		  	 	//初始化与LED连接的硬件接口
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        Delay(300);
    }
}

























