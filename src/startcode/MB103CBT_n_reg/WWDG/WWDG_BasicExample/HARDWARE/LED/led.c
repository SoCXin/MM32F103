#include "sys.h"
#include "led.h"


//��ʼ��PA15��PB3,PB4,PB5Ϊ�����.��ʹ����4���ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //RCC->APB2ENR|=1<<3;  //ʹ��GPIOBʱ��
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //RCC->APB2ENR|=1<<3;
    JTAG_Set(SWD_ENABLE);	//�ر� JTAG,����SWD

    GPIOA->CRH |= GPIO_CRH_MODE15;       //GPIOA->CRH|=0x30000000;//PA15 �������
    GPIOA->ODR |= GPIO_ODR_ODR15;          // GPIOA->ODR|=1<<15;   //PA15 �����

    GPIOB->CRL |= GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5; ////PB3,PB4,PB5�������
    GPIOB->ODR |= GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5;    ////PB.3,PB4,PB5�����

    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
}

