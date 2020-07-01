#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//通用定时器 驱动代码
//////////////////////////////////////////////////////////////////////////////////


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1RSTR_TIM3RST;	//TIM3时钟使能
    TIM3->ARR = arr;  	//设定计数器自动重装值
    TIM3->PSC = psc;  	//预分频器设置
    TIM3->DIER |= TIM_DIER_UIE; //允许更新中断
    TIM3->CR1 |= TIM_CR1_CEN;  //使能定时器3
    MY_NVIC_Init(1, 3, TIM3_IRQn, 2); //抢占1，子优先级3，组2
}

void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) //溢出中断
    {
        LED1 = !LED1;
    }
    TIM3->SR &= ~(TIM_SR_UIF); //清除中断标志位
}












