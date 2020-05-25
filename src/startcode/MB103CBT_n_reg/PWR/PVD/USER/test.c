#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：PVD_IRQHandler(void)
**功能描述 ：电压超过或低于设置的阈值电压后，进入该中断，翻转LED
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if((EXTI->PR & 0x10000) != RESET) //
    {
        /* LED翻转 */
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        /* Clear the Key Button EXTI line pending bit */
        EXTI->PR = 0x10000;
    }
}
int main(void)
{

    delay_init();	     	//延时初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    MY_NVIC_Init(3, 3, PVD_IRQn, 2); //开启PVD中断
    EXTI->IMR |= 1 << 16; //开启16号中断线
    EXTI->RTSR |= 1 << 16; //开启16号中断线上升沿触发
    EXTI->FTSR |= 1 << 16; //开启16号中断线下降沿触发
    PWR->CR |= 0x00000800; //开启3.0V为电压阈值
    PWR->CR |= 1 << 4; //使能PVD
    while(1)
    {
    }
}

























