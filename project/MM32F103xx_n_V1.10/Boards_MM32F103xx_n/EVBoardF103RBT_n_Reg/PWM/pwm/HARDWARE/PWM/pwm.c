#include "pwm.h"
#include "led.h"

//TIM3_CH1 PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr, u16 psc)
{
    //此部分需手动修改IO口设置
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; 	//TIM1时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //使能PORTB时钟
    AFIO->MAPR &= ~(1 << 7); //TIM1部分重影像
    AFIO->MAPR |= 1 << 6; //TIM1部分重影像

    GPIOA->CRH &= 0XFFFFFFF0;	//PA8清除之前的设置
    GPIOA->CRH |= 0xb;	//复用功能输出

    TIM1->ARR = arr;			//设定计数器自动重装值
    TIM1->PSC = psc;			//预分频器设置

    TIM1->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2;  	//CH1 PWM2模式
    TIM1->BDTR |= 0x8000;
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE; 		//CH1预装载使能

    TIM1->CCER |= TIM_CCER_CC1E;   	//OC1 输出使能

    TIM1->CR1 = TIM_CR1_ARPE;   	//ARPE使能
    TIM1->CR1 |= TIM_CR1_CEN;    	//使能定时器1
}
