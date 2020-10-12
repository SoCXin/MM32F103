#include "wkup.h"
#include "led.h"
#include "delay.h"



//系统进入待机模式
void Sys_Enter_Standby(void)
{
    //关闭所有外设(根据实际情况写)
    RCC->APB2RSTR |= 0X01FC; //复位所有IO口
    Sys_Standby();//进入待机模式
}
//检测WKUP脚的信号
//返回值1:连续按下3s以上
//      0:错误的触发
u8 Check_WKUP(void)
{
    u8 t = 0;	//记录按下的时间
    LED1 = 0; //亮灯DS0
    LED2 = 0; //亮灯DS0
    LED3 = 0; //亮灯DS0
    LED4 = 0; //亮灯DS0
    while(1)
    {
        if(WKUP_KD)
        {
            t++;			//已经按下了
            delay_ms(30);
            if(t >= 100)		//按下超过3秒钟
            {
                LED1 = 0;	 	//点亮DS0
                LED2 = 0;	 	//点亮DS0
                LED3 = 0;	 	//点亮DS0
                LED4 = 0;
                return 1; 	//按下3s以上了
            }
        }
        else
        {
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;
            return 0; //按下不足3秒
        }
    }
}
//中断,检测到PA0脚的一个上升沿.
//中断线0线上的中断检测
void EXTI0_IRQHandler(void)
{
    EXTI->PR = EXTI_PR_PR0; //清除LINE10上的中断标志位

}
//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //先使能外设IO PORTA时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   //开启辅助时钟

    GPIOA->CRL &= 0XFFFFFFF0; //PA0设置成输入
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    Ex_NVIC_Config(GPIO_A, 0, RTIR); //PA0上升沿触发

    //(检查是否是正常开)机
    if(Check_WKUP() == 0)Sys_Standby();  //不是开机,进入待机模式
    MY_NVIC_Init(2, 2, EXTI0_IRQn, 2); //抢占2，子优先级2，组2
}
















