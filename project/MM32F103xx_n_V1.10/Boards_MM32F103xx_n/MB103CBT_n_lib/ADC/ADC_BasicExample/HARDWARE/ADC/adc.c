/**
******************************************************************************
* @file    adc.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the adc firmware functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/#include "adc.h"


void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*��PA1����Ϊģ������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
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


    GPIO_Configuration();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_ContinuousConvMode member */
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC1 ͨ��1����PA1
    ADC_Init(ADC1, &ADC_InitStructure);

    /*��������ͨ��*/
    ADC_RegularChannelConfig(ADC1, 9, 0, 0);
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
**������Ϣ ��ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
{
    u16 puiADData;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!(ADC1->ADSTA & ADC_FLAG_EOC));

    puiADData = ADC1->ADDATA & 0xfff;

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /*ADCR�Ĵ�����ADSTλʧ�ܣ����ת������*/

    return puiADData;
}

/********************************************************************************************************
**������Ϣ ��Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
**�������� ����ȡ����ADC1����ֵ��ƽ��ֵ
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 Get_Adc_Average(uint8_t ADC_Channel_x, uint8_t times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    for(t = 0; t < times; t++)
    {
        temp_val += ADC1_SingleChannel_Get(ADC_Channel_x);
        for(delay = 0; delay < 100; delay++);
    }
    return temp_val / times;
}


/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/
