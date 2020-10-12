/**
******************************************************************************
* @file    adc.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the adc inc file for the library.
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
#ifndef __ADC_H_
#define __ADC_H_
#include "HAL_device.h"
void ADC1_SingleChannel(uint8_t ADC_Channel_x);
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x);
u16 Get_Adc_Average(uint8_t ADC_Channel_x, uint8_t times);

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
