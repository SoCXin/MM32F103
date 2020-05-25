/**
******************************************************************************
* @file    adc.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the lcd functions.
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
*/
#define _ADC_C_

/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	adc_test.c
** Descriptions:		����ADC1Ϊ����ת��ģʽ,ͨ��xʹ��

*********************************************************************************************************/
#include "adc.h"


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
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_SampleTime_239_5Cycles);

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
