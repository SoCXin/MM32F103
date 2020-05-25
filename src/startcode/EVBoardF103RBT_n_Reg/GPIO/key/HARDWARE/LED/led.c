#include "sys.h"
#include "led.h"


//��ʼ��PA15��PB3,PB4,PB5Ϊ�����.��ʹ����4���ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //ʹ��GPIOCʱ��

    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->ODR |= GPIO_ODR_ODR8;

    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->ODR |= GPIO_ODR_ODR13;

    LED1 = 1;
    LED2 = 1;
}

