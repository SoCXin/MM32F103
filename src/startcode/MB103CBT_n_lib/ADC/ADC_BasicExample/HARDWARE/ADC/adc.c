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
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, 0);

    ADC_Cmd(ADC1, ENABLE);

    if(ADC_Channel_x == ADC_Channel_8)
    {
        /*温度传感器使能*/
        ADC1->ADCFG |= 0x04;
    }
}
/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
{
    u16 puiADData;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!(ADC1->ADSTA & ADC_FLAG_EOC));

    puiADData = ADC1->ADDATA & 0xfff;

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /*ADCR寄存器的ADST位失能，软件转换结束*/

    return puiADData;
}

/********************************************************************************************************
**函数信息 ：Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
**功能描述 ：获取几次ADC1采样值的平均值
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
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
