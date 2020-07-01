#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "uart.h"



//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
    KEY_Init();
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //RCC->APB2ENR|=1;  //ʹ��AFIOʱ��

    Ex_NVIC_Config(GPIO_A, 0, RTIR); 		//�����ش���

    Ex_NVIC_Config(GPIO_C, 13, FTIR);		//�½��ش���

    Ex_NVIC_Config(GPIO_B, 10, FTIR);		//�½��ش���
    Ex_NVIC_Config(GPIO_B, 11, FTIR);		//�½��ش���

    MY_NVIC_Init(2, 2, EXTI0_IRQn, 2);    	//��ռ2�������ȼ�2����2
    MY_NVIC_Init(2, 0, EXTI15_10_IRQn, 2);	//��ռ2�������ȼ�0����2

}


void EXTI0_IRQHandler(void)
{
    delay_ms(5);    //����
    if(WK_UP == 1)
    {
        LED2 = !LED2;
    }
    EXTI->PR = 1 << 0; //���LINE0�ϵ��жϱ�־λ

}


void EXTI15_10_IRQHandler(void)
{
    delay_ms(5);    //����

    if(KEY1 == 0)
    {
        LED1 = !LED1;
    }
    else if(KEY3 == 0)
    {
        LED3 = !LED3;
    }
    else if(KEY4 == 0)
    {
        LED4 = !LED4;
    }
    EXTI->PR = 1 << 10; //���LINE10�ϵ��жϱ�־λ
    EXTI->PR = 1 << 11; //���LINE11�ϵ��жϱ�־λ
    EXTI->PR = 1 << 13; //���LINE13�ϵ��жϱ�־λ
}






