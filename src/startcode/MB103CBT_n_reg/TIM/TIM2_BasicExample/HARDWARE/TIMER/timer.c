#include "timer.h"
#include "led.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//TIM3ʱ��ʹ��
    TIM2->ARR = arr;  	//�趨�������Զ���װֵ
    TIM2->PSC = psc;  	//Ԥ��Ƶ������
    TIM2->DIER |= TIM_DIER_UIE; //��������ж�
    TIM2->CR1 |= TIM_CR1_CEN;  //ʹ�ܶ�ʱ��3
    TIM2->CR1 |= TIM_CR1_CEN;  //ʹ�ܶ�ʱ��3
    MY_NVIC_Init(1, 3, TIM2_IRQn, 2); //��ռ1�������ȼ�3����2

}


void TIM2_IRQHandler(void)
{
    /*�����ֶ�����жϱ�־λ*/
    if(TIM2->SR & TIM_SR_UIF) //����ж�
    {
        /*ȫ�ֱ�־*/
        ucTim2Flag++;
    }
    TIM2->SR &= ~(TIM_SR_UIF); //����жϱ�־λ

}



