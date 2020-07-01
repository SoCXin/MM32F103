#include "adc.h"
#include "delay.h"



void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ʹ��ADC1ʱ��

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1��λ
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //��λ����

    /*��ADC���óɵ����������ת��ģʽ*/
    ADC1->ADCFG |= 7 << 4; //16��Ƶ
    ADC1->ADCFG |= 0x7 << 10;
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //���η�����ģʽ,�����Ҷ���
    ADC1->ADCR |= ADCR_ADMD_PERIOD;

    ADC1->ADCHS &= 0xffffffe00;	/*��������ͨ��*/
    ADC1->ADCHS = ADCHS_SENSOREN;//ʹ��ͨ��8���¶ȴ�����

    ADC1->ADCFG |= ADCFG_TVSEN; //�����¶ȴ�����

    ADC1->ADCFG |= ADCFG_ADEN;//ADC1ʹ��

}
//���ADCֵ
u16 Get_Adc(void)
{

    u32 temp;
    u16 i, j, val[3];

    for(i = 0; i < 3; i++) //��ʼת��
    {


        ADC1->ADCR |= ADCR_ADST;
        do
        {
            temp = ADC1->ADDATA;
        }
        while(((temp >> 21) & 0x01) == 0);
        val[i] = temp & 0xfff;

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

    ADC1->ADCR &= ~ADCR_ADST; //����ת��
    return val[1] & 0xfff;
}




























