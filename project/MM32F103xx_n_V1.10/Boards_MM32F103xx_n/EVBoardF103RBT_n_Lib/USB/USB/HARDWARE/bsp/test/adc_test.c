/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	adc_test.c
** Descriptions:		����ADC1Ϊ����ת��ģʽ,ͨ��xʹ��

*********************************************************************************************************/
#include "adc_test.h"

/********************************************************************************************************
**������Ϣ ��void ADC1_SingleChannel(uint8_t ADC_Channel_x)
**�������� ������ADC1����ת��ģʽ
**������� ��ADC_Channel_x , xΪ0~8
**������� ����
**    ��ע ��
********************************************************************************************************/
void ADC1_SingleChannel(uint8_t ADC_Channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_4;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_ContinuousConvMode member */
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ADC_InitStructure);

    /*��������ͨ��*/
    ADC1->ADCHS &= 0xffffffe00;
    /*ʹ��ѡ��ͨ��,�����������*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, 0);

    ADC_Cmd(ADC1, ENABLE);

    if(ADC_Channel_x == ADC_Channel_8)
    {
        /*�¶ȴ�����ʹ��*/
        ADC1->ADCFG |= 0x04;
    }
}
/********************************************************************************************************
**������Ϣ ��ADC1_SingleChannel_Get()
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
             *puiADData ,ADC1ʵ��ת������
**������� ��ucStatus ,0 ��ʾ���ݻ�ȡʧ��,1 ��ʾ�ɹ�
********************************************************************************************************/
unsigned char ADC1_SingleChannel_Get(uint8_t ADC_Channel_x, unsigned int *puiADData)
{
    unsigned char ucStatus = 0;
    unsigned int  uiCnt = 0;
    /*ADCR�Ĵ�����ADSTλʹ�ܣ��������ת��*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    do
    {
        /*����ǰ���ݼĴ���ֵ*/
        *puiADData = ADC1->ADDATA;
        uiCnt++;
    }
    while( (((*puiADData >> 21) & 0x01) == 0) &&  (uiCnt < 100));
    /*1.�ж��Ƿ�ʱ 2.�ж��Ƿ�����Ч���� 3.�ж��Ƿ�Ϊ��ǰͨ����*/
    if((uiCnt < 100) && (((*puiADData >> 21) & 0x01) == 1) && (((*puiADData >> 16) & 0x0f) == ADC_Channel_x))
    {
        ucStatus = 1;
        *puiADData &= 0xfff;
    }
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /*ADCR�Ĵ�����ADSTλʧ�ܣ����ת������*/


    return ucStatus;
}

