/**
******************************************************************************
* @file    exter_rcc.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the exter_rcc firmware functions.
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
#include "HAL_device.h"


/********************************************************************************************************
**函数信息 ：SystemClk_HSEInit (void)
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void SystemClk_HSEInit(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_DeInit();
    //HSE on
    //CR寄存器BIT16位（HSEON位）置1，作用是连接外部时钟HSE作为系统时钟
    RCC_HSEConfig(RCC_HSE_ON);
    while(1)
    {
        if(RCC_WaitForHSEStartUp() != 0)
        {
            break;
        }
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);//选择外部时钟作为系统时钟
    RCC_MCOConfig(RCC_MCO_SYSCLK);  //通过PA8 pin 观察频率
}




/********************************************************************************************************
**函数信息 ：SystemClkPll(unsigned int pllmul)
**功能描述 ：倍频频率作为系统工作时钟
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16
**输出参数 ：无
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{

    RCC_PLLCmd(ENABLE); //使能PLL

    while(1)
    {
        if(RCC->CR & 0x02000000) //查询PLL ready
        {
            break;
        }
    }

    FLASH_SetLatency(FLASH_Latency_2);//时钟等待状态

    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //通过PA8 pin 观察频率
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//选择PLL时钟作为系统时钟
}


/********************************************************************************************************
**函数信息 ：HSE_Pll_test(unsigned int pllmul)
**功能描述 ：外部晶振作为时钟源
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16
**输出参数 ：无
********************************************************************************************************/

void HSE_Pll_test(unsigned int pllmul)
{

    SystemClk_HSEInit();
    //PLL 配置
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul); //选择外部时钟源作为倍频时钟的基础
    SystemClkPll(pllmul);
}






/********************************************************************************************************
**函数信息 ：LSE_clk()
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 32.786KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void LSE_clk()
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE); //使能PWR_CR中的DBPbit

    RCC_LSEConfig(RCC_LSE_ON);

    while(1)
    {
        if(RCC->BDCR & 0x00000002) //查询外部32.768KHz低频时钟ready
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSE);  //通过PA8 pin 观察频率
}

/********************************************************************************************************
**函数信息 ：HSE_HSI_Change()
**功能描述 ：外部时钟与内部时钟之间的切换
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void HSE_HSI_Change()
{
    RCC_ClockSecuritySystemCmd(ENABLE); //CSS on
    SystemClk_HSEInit();

    RCC->CR &= 0xfffeffff; //close HESON
    //GPIO_LedTest();
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
