#include "timer.h"
#include "led.h"

//��ʱ��2ͨ��1���벶������
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM2_Cap_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1RSTR_TIM2RST;   	//TIM2 ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2RSTR_IOPARST;    	//ʹ��PORTAʱ��

    GPIOA->CRL &= 0XFFFFFFF0;	//PA0 ���֮ǰ����
    GPIOA->CRL |= 0x8;	//PA0 ����
    GPIOA->ODR |= ~GPIO_ODR_ODR0;		//PA0 ����

    TIM2->ARR = arr;  		//�趨�������Զ���װֵ
    TIM2->PSC = psc;  		//Ԥ��Ƶ��

    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_0; 		//IC1F=0001 ���������˲��� ��Fck_int������2���¼�����Ч

    TIM2->CCER |= TIM_CCER_CC1E; 		//CC1E=1 	�������������ֵ������Ĵ�����

    TIM2->DIER |= TIM_DIER_CC1IE;   	//�������ж�
    TIM2->DIER |= TIM_DIER_UIE;   	//��������ж�
    TIM2->CR1 |= TIM_CR1_CEN;    	//ʹ�ܶ�ʱ��2
    MY_NVIC_Init(2, 0, TIM2_IRQn, 2); //��ռ2�������ȼ�0����2
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���
u8  TIM2CH1_CAPTURE_STA = 0;	//���벶��״̬
u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{
    u16 tsr;
    tsr = TIM2->SR;
    if((TIM2CH1_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(tsr & 0X01) //���
        {
            if(TIM2CH1_CAPTURE_STA & 0X40) //�Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM2CH1_CAPTURE_STA & 0X3F) == 0X3F) //�ߵ�ƽ̫����
                {
                    TIM2CH1_CAPTURE_STA |= 0X80; //��ǳɹ�������һ��
                    TIM2CH1_CAPTURE_VAL = 0XFFFF;
                }
                else TIM2CH1_CAPTURE_STA++;
            }
        }
        if(tsr & 0x02) //����1���������¼�
        {
            if(TIM2CH1_CAPTURE_STA & 0X40)		//����һ���½���
            {
                TIM2CH1_CAPTURE_STA |= 0X80;		//��ǳɹ�����һ�θߵ�ƽ����
                TIM2CH1_CAPTURE_VAL = TIM2->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
                TIM2->CCER &= ~(TIM_CCER_CC1P);			//CC1P=0 ����Ϊ�����ز���
            }
            else  								//��δ��ʼ,��һ�β���������
            {
                TIM2CH1_CAPTURE_VAL = 0;
                TIM2CH1_CAPTURE_STA = 0X40;		//��ǲ�����������
                TIM2->CNT = 0;					//���������
                TIM2->CCER |= TIM_CCER_CC1P; 				//CC1P=1 ����Ϊ�½��ز���
            }
        }
    }
    TIM2->SR = 0; //����жϱ�־λ
}


















