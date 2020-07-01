/**
******************************************************************************
* @file    rcc_test.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the rcc_test inc file for the library.
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
#ifndef __RCC_TEST_H_
#define __RCC_TEST_H_
#include "sys.h"
void SystemClk_HSEInit(void);
void SystemClk_HSIInit(void);
void SystemClkPll(unsigned int pllmul);
void HSE_Pll_test(unsigned int pllmul);
void HSI_Pll_test(unsigned int pllmul);
void LSI_clk(void);
void LSE_clk(void);
void HSE_HSI_Change(void);
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

