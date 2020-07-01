#include "timer.h"
#include "led.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM1_Int_Init(u16 arr, u16 psc)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//TIM3ʱ��ʹ��
    TIM1->ARR = arr;  	//�趨�������Զ���װֵ
    TIM1->PSC = psc;  	//Ԥ��Ƶ������
    TIM1->CR1 |= TIM_CR1_CEN;  //ʹ�ܶ�ʱ��3
}

void Tim1_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while((TIM1->SR & TIM_SR_UIF) == RESET);
    /*���¼���־λ*/

    TIM1->SR = ~TIM_SR_UIF;
}
