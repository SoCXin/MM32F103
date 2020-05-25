#include "sys.h"
#include "led.h"


//初始化PA15和PB3,PB4,PB5为输出口.并使能这4个口的时钟
//LED IO初始化
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //RCC->APB2ENR|=1<<3;  //使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //RCC->APB2ENR|=1<<3;
    JTAG_Set(SWD_ENABLE);	//关闭 JTAG,开启SWD

    GPIOA->CRH |= GPIO_CRH_MODE15;       //GPIOA->CRH|=0x30000000;//PA15 推挽输出
    GPIOA->ODR |= GPIO_ODR_ODR15;          // GPIOA->ODR|=1<<15;   //PA15 输出高

    GPIOB->CRL |= GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5; ////PB3,PB4,PB5推挽输出
    GPIOB->ODR |= GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5;    ////PB.3,PB4,PB5输出高

    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
}

