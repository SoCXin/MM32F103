#include "sys.h"
#include "led.h"


//��ʼ��PA15��PB3,PB4,PB5Ϊ�����.��ʹ����4���ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //ʹ��GPIOCʱ��

    GPIOA->CRH |= GPIO_CRH_MODE8;       //GPIOA->CRH|=0x30000000;//PA15 �������
    GPIOA->ODR |= GPIO_ODR_ODR8;          // GPIOA->ODR|=1<<15;   //PA15 �����

    GPIOC->CRH |= GPIO_CRH_MODE13;    ////PB3,PB4,PB5�������
    GPIOC->ODR |= GPIO_ODR_ODR13;        ////PB.3,PB4,PB5�����

    LED1 = 1;
    LED2 = 1;
}

