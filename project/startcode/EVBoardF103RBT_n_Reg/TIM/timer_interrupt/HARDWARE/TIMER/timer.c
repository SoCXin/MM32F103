#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//ͨ�ö�ʱ�� ��������
//////////////////////////////////////////////////////////////////////////////////


//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1RSTR_TIM3RST;	//TIM3ʱ��ʹ��
    TIM3->ARR = arr;  	//�趨�������Զ���װֵ
    TIM3->PSC = psc;  	//Ԥ��Ƶ������
    TIM3->DIER |= TIM_DIER_UIE; //��������ж�
    TIM3->CR1 |= TIM_CR1_CEN;  //ʹ�ܶ�ʱ��3
    MY_NVIC_Init(1, 3, TIM3_IRQn, 2); //��ռ1�������ȼ�3����2
}

void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) //����ж�
    {
        LED1 = !LED1;
    }
    TIM3->SR &= ~(TIM_SR_UIF); //����жϱ�־λ
}












