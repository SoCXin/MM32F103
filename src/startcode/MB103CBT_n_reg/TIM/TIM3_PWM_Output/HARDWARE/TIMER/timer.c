#include "timer.h"
#include "led.h"

//TIM3_CH1 PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    //�˲������ֶ��޸�IO������
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 	//TIM3ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //RCC->APB2ENR|=1<<3;  //ʹ��GPIOBʱ��
    AFIO->MAPR &= ~(1 << 10); //TIM3������Ӱ��
    AFIO->MAPR |= 1 << 11; //TIM3������Ӱ��

    GPIOB->CRL &= 0XFFF0FFFF;	//PB4���֮ǰ������
    GPIOB->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_CNF4_1;	//���ù������

    TIM3->ARR = arr;			//�趨�������Զ���װֵ
    TIM3->PSC = psc;			//Ԥ��Ƶ������

    TIM3->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2;  	//CH1 PWM2ģʽ
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE; 		//CH1Ԥװ��ʹ��

    TIM3->CCER |= TIM_CCER_CC1E;   	//OC1 ���ʹ��

    TIM3->CR1 = TIM_CR1_ARPE;   	//ARPEʹ��
    TIM3->CR1 |= TIM_CR1_CEN;    	//ʹ�ܶ�ʱ��1
}
