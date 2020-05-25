#include "wdg.h"
#include "led.h"


//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer, u16 rlr)
{
    RCC->CSR |= RCC_CSR_LSION; //���ڲ�����ʱ��
    IWDG->KR = 0X5555; //ʹ�ܶ�IWDG->PR��IWDG->RLR��д
    IWDG->PR = prer; //���÷�Ƶϵ��
    IWDG->RLR = rlr; //�Ӽ��ؼĴ��� IWDG->RLR
    IWDG_Feed();//reload
    IWDG->KR = 0XCCCC; //ʹ�ܿ��Ź�
}
//ι�������Ź�
void IWDG_Feed(void)
{
    IWDG->KR = 0XAAAA; //reload
}
//����WWDG������������ֵ,Ĭ��Ϊ���.
u8 WWDG_CNT = 0x7f;
//��ʼ�����ڿ��Ź�
//tr   :T[6:0],������ֵ
//wr   :W[6:0],����ֵ
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч
//Fwwdg=PCLK1/(4096*2^fprer).
void WWDG_Init(u8 tr, u8 wr, u8 fprer)
{
    RCC->APB1ENR |= RCC_APB1ENR_WWDGEN; 	//ʹ��wwdgʱ��
    WWDG_CNT = tr & WWDG_CNT; //��ʼ��WWDG_CNT.
    WWDG->CFR |= fprer << 7; //PCLK1/4096�ٳ�2^fprer
    WWDG->CFR &= 0XFF80;
    WWDG->CFR |= wr;     		//�趨����ֵ
    WWDG->CR |= WWDG_CNT; 	//�趨������ֵ
    WWDG->CR |= WWDG_CR_WDGA;  		//�������Ź�
    MY_NVIC_Init(2, 3, WWDG_IRQn, 2); //��ռ2�������ȼ�3����2
    WWDG->SR = 0X00; 			//�����ǰ�����жϱ�־λ
    WWDG->CFR |= WWDG_CFR_EWI;      //ʹ����ǰ�����ж�
}
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG->CR = (cnt & 0x7F); //������7λ������
}
//���ڿ��Ź��жϷ������
void WWDG_IRQHandler(void)
{
    WWDG_Set_Counter(WWDG_CNT);//���贰�ڿ��Ź���ֵ!
    WWDG->SR = 0X00; //�����ǰ�����жϱ�־λ
    LED2 = !LED2;
}




















