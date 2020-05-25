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
** Descriptions:		配置ADC1为单次转换模式,通道x使能

*********************************************************************************************************/
#include "adc.h"


void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*将PA1配置为模拟输入*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/********************************************************************************************************
**函数信息 ：void ADC1_SingleChannel(uint8_t ADC_Channel_x)
**功能描述 ：配置ADC1单次转换模式
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：无
**    备注 ：
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
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC1 通道1，即PA1
    ADC_Init(ADC1, &ADC_InitStructure);

    /*屏蔽所有通道*/
    ADC_RegularChannelConfig(ADC1, 9, 0, 0);
    /*使能选中通道,后面参数保留*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_SampleTime_239_5Cycles);

    ADC_Cmd(ADC1, ENABLE);

    if(ADC_Channel_x == ADC_Channel_8)
    {
        /*温度传感器使能*/
        ADC1->ADCFG |= 0x04;
    }
}
/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get()
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~8
*puiADData ,ADC1实际转换数据
**输出参数 ：ucStatus ,0 表示数据获取失败,1 表示成功
********************************************************************************************************/
unsigned char ADC1_SingleChannel_Get(uint8_t ADC_Channel_x, unsigned int *puiADData)
{
    unsigned char ucStatus = 0;
    unsigned int  uiCnt = 0;
    /*ADCR寄存器的ADST位使能，软件启动转换*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    do
    {
        /*读当前数据寄存器值*/
        *puiADData = ADC1->ADDATA;
        uiCnt++;
    }
    while( (((*puiADData >> 21) & 0x01) == 0) &&  (uiCnt < 100));
    /*1.判断是否超时 2.判断是否有有效数据 3.判断是否为当前通道号*/
    if((uiCnt < 100) && (((*puiADData >> 21) & 0x01) == 1) && (((*puiADData >> 16) & 0x0f) == ADC_Channel_x))
    {
        ucStatus = 1;
        *puiADData &= 0xfff;
    }
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /*ADCR寄存器的ADST位失能，软件转换结束*/


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
