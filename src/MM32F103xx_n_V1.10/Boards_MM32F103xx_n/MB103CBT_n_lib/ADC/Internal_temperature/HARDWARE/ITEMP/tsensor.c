/**
******************************************************************************
* @file    tsensor.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the tsensor firmware functions.
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
#include "tsensor.h"
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//ADC 驱动代码
//////////////////////////////////////////////////////////////////////////////////


//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void T_Adc_Init(void)  //ADC通道初始化
{
    ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1	, ENABLE );	 //使能GPIOA,ADC1通道时钟

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz

    ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值

    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_ContinuousConvMode member */
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
    ADC1->ADCHS &= 0xffffffe00;	/*屏蔽所有通道*/


    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 0, 0);

    ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
    ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1


}
/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get()
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(void)
{
    u16 puiADData;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!(ADC1->ADSTA & 0x01));

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
