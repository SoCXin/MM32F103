#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "uart.h"



//外部中断初始化程序
void EXTIX_Init(void)
{
    KEY_Init();
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //RCC->APB2ENR|=1;  //使能AFIO时钟

    Ex_NVIC_Config(GPIO_A, 0, RTIR); 		//上升沿触发

    Ex_NVIC_Config(GPIO_C, 10, FTIR);		//下降沿触发

    Ex_NVIC_Config(GPIO_A, 15, FTIR);		//下降沿触发

    MY_NVIC_Init(2, 2, EXTI0_IRQn, 2);    	//抢占2，子优先级2，组2
    MY_NVIC_Init(2, 0, EXTI15_10_IRQn, 2);	//抢占2，子优先级0，组2

}


void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //消抖
    if(WK_UP == 1)
    {
        LED2 = !LED2;
        LED1 = !LED1;
    }
    EXTI->PR = 1 << 0; //清除LINE0上的中断标志位

}


void EXTI15_10_IRQHandler(void)
{
    delay_ms(10);    //消抖

    if(KEY3 == 0)
    {
        LED1 = !LED1;
    }
    else if(KEY4 == 0)
    {
        LED2 = !LED2;
    }
    EXTI->PR = 1 << 0; //清除LINE10上的中断标志位
    EXTI->PR = 1 << 10; //清除LINE11上的中断标志位
    EXTI->PR = 1 << 15; //清除LINE13上的中断标志位
}



