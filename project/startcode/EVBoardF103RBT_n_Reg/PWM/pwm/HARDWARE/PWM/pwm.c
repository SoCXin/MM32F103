#include "pwm.h"
#include "led.h"

//TIM3_CH1 PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr, u16 psc)
{
    //�˲������ֶ��޸�IO������
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; 	//TIM1ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //ʹ��PORTBʱ��
    AFIO->MAPR &= ~(1 << 7); //TIM1������Ӱ��
    AFIO->MAPR |= 1 << 6; //TIM1������Ӱ��

    GPIOA->CRH &= 0XFFFFFFF0;	//PA8���֮ǰ������
    GPIOA->CRH |= 0xb;	//���ù������

    TIM1->ARR = arr;			//�趨�������Զ���װֵ
    TIM1->PSC = psc;			//Ԥ��Ƶ������

    TIM1->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2;  	//CH1 PWM2ģʽ
    TIM1->BDTR |= 0x8000;
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE; 		//CH1Ԥװ��ʹ��

    TIM1->CCER |= TIM_CCER_CC1E;   	//OC1 ���ʹ��

    TIM1->CR1 = TIM_CR1_ARPE;   	//ARPEʹ��
    TIM1->CR1 |= TIM_CR1_CEN;    	//ʹ�ܶ�ʱ��1
}
