#include "adc.h"
#include "delay.h"


//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3
void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ʹ��ADC1ʱ��
    GPIOA->CRL &= 0XFFFFFF0F; //PA1 anolog����

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1��λ
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //��λ����

    /*��ADC���óɵ����������ת��ģʽ*/
    ADC1->ADCFG |= 7 << 4; //16��Ƶ
    ADC1->ADCFG |= 0x7 << 10;
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //���η�����ģʽ,�����Ҷ���
    ADC1->ADCR |= ADCR_ADMD_PERIOD;
    ADC1->ADCHS = ADCHS_CHEN1; //ʹ��ͨ��1
    ADC1->ADCFG |= ADCFG_ADEN;//ADC1ʹ��
    ADC1->ADCR |= ADCR_ADST;//��ʼת��

}
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(void)
{
    u32 temp;
    u16 i, j, val[3];


    for(i = 0; i < 3; i++)
    {
        ADC1->ADCR |= ADCR_ADST;//��ʼת��
        do
        {
            temp = ADC1->ADDR1;
        }
        while(((temp >> 21) & 0x01) == 0x00);
        val[i] = temp;
    }

    for(i = 0; i < 3; i++)
    {
        for(j = i + 1; j < 3 - i; j++)
        {
            if(val[i] > val[j])
            {
                temp = val[i];
                val[i] = val[j];
                val[j] = temp;
            }
        }
    }
    return val[1] & 0xfff;
}




























