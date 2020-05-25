/**
******************************************************************************
* @file    tsensor.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the functions prototypes for tsensor led firmware
*          library.
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
#ifndef __TSENSOR_H
#define __TSENSOR_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//ADC 驱动代码
//////////////////////////////////////////////////////////////////////////////////

#define ADC_CH_TEMP  	ADC_Channel_16 //温度传感器通道

u16  T_Get_Temp(void);  //取得温度值
void T_Adc_Init(void); //ADC通道初始化
void ADC1_SingleChannel_Get(uint8_t ADC_Channel_x, unsigned int *puiADData); //得到某个通道10次采样的平均值
void  T_Get_Adc_Average(uint8_t ADC_Channel_x, unsigned int *puiADData);

#endif


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
