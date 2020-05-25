#include "sys.h"
#include "led.h"


//��ʼ��PA8��PD2Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2RSTR_IOPCRST; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOCʱ��
    GPIOC->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;     //GPIOA->CRH|=0X00000003;//PA8 �������
    GPIOC->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_MODE9;     //GPIOA->CRH|=0X00000003;//PA8 �������

    GPIOC->ODR |= GPIO_ODR_ODR6 | GPIO_ODR_ODR7 | GPIO_ODR_ODR8 | GPIO_ODR_ODR9;    // GPIOA->ODR|=1<<8;   //PA8 �����

}

/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ��
����  ֵ����
**************************************************************************/
void Led_Flash(u16 time)
{
    static int temp;
    if(++temp == time)	LED0 = ~LED0, temp = 0;
}
