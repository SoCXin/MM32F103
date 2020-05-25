#include "timer.h"
#include "led.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//TIM3时钟使能
    TIM2->ARR = arr;  	//设定计数器自动重装值
    TIM2->PSC = psc;  	//预分频器设置
    TIM2->DIER |= TIM_DIER_UIE; //允许更新中断
    TIM2->CR1 |= TIM_CR1_CEN;  //使能定时器3
    TIM2->CR1 |= TIM_CR1_CEN;  //使能定时器3
    MY_NVIC_Init(1, 3, TIM2_IRQn, 2); //抢占1，子优先级3，组2

}


void TIM2_IRQHandler(void)
{
    /*必须手动清除中断标志位*/
    if(TIM2->SR & TIM_SR_UIF) //溢出中断
    {
        /*全局标志*/
        ucTim2Flag++;
    }
    TIM2->SR &= ~(TIM_SR_UIF); //清除中断标志位

}



