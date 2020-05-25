#include "sys.h"
#include "led.h"


//初始化PA15和PB3,PB4,PB5为输出口.并使能这4个口的时钟
//LED IO初始化
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //使能GPIOC时钟

    GPIOA->CRH |= GPIO_CRH_MODE8;       //GPIOA->CRH|=0x30000000;//PA15 推挽输出
    GPIOA->ODR |= GPIO_ODR_ODR8;          // GPIOA->ODR|=1<<15;   //PA15 输出高

    GPIOC->CRH |= GPIO_CRH_MODE13;    ////PB3,PB4,PB5推挽输出
    GPIOC->ODR |= GPIO_ODR_ODR13;        ////PB.3,PB4,PB5输出高

    LED1 = 1;
    LED2 = 1;
}

