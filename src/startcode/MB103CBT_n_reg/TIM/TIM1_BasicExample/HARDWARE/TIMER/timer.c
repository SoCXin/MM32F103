#include "timer.h"
#include "led.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM1_Int_Init(u16 arr, u16 psc)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//TIM3时钟使能
    TIM1->ARR = arr;  	//设定计数器自动重装值
    TIM1->PSC = psc;  	//预分频器设置
    TIM1->CR1 |= TIM_CR1_CEN;  //使能定时器3
}

void Tim1_UPStatusOVCheck_test(void)
{
    /*等待事件更新标志位*/
    while((TIM1->SR & TIM_SR_UIF) == RESET);
    /*清事件标志位*/

    TIM1->SR = ~TIM_SR_UIF;
}
