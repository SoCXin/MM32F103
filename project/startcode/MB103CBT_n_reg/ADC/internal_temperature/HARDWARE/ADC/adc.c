#include "adc.h"
#include "delay.h"



void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ʹ��ADC1ʱ��

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1��λ
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //��λ����

    /*��ADC���óɵ����������ת��ģʽ*/
    ADC1->ADCFG |= ADCFG_ADCPRE_8 | ADCFG_ADCPRE_10; //8��Ƶ
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //���η�����ģʽ,�����Ҷ���
    ADC1->ADCR |= ADCR_ADMD_PERIOD;

    ADC1->ADCHS &= 0xffffffe00;	/*��������ͨ��*/
    ADC1->ADCHS = ADCHS_SENSOREN;//ʹ��ͨ��8���¶ȴ�����

    ADC1->ADCFG |= ADCFG_TVSEN; //�����¶ȴ�����

    ADC1->ADCFG |= ADCFG_ADEN;//ADC1ʹ��

}


/********************************************************************************************************
**������Ϣ ��ADC1_SingleChannel_Get()
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(void)
{
    u16 puiADData;

    ADC1->ADCR |= 0x100;

    while(!(ADC1->ADSTA & 0x01));

    puiADData = ADC1->ADDATA & 0xfff;

    ADC1->ADSTA |= 0x1;
    /*ADCR�Ĵ�����ADSTλʧ�ܣ����ת������*/
    return puiADData;
}

/********************************************************************************************************
**������Ϣ ��Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
**�������� ����ȡ����ADC1����ֵ��ƽ��ֵ
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 Get_Adc_Average(uint8_t times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    for(t = 0; t < times; t++)
    {
        temp_val += ADC1_SingleChannel_Get();
        for(delay = 0; delay < 100; delay++);
    }
    return temp_val / times;
}
























