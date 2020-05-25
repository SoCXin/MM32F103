/**
******************************************************************************
* @file    inter_rcc.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the inter_rcc firmware functions.
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
**������Ϣ ��SystemClk_HSIInit (void)
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/

void SystemClk_HSIInit()
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //HSI on
    //CR�Ĵ���BIT0λ��HSIONλ����1�������������ڲ�ʱ��HSI��Ϊϵͳʱ��
    RCC_HSICmd(ENABLE);

    RCC_MCOConfig(RCC_MCO_SYSCLK);  //ͨ��PA8 pin �۲�Ƶ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//ѡ���ڲ�ʱ����Ϊϵͳʱ��
}


/********************************************************************************************************
**������Ϣ ��SystemClkPll(unsigned int pllmul)
**�������� ����ƵƵ����Ϊϵͳ����ʱ��
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16
**������� ����
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{

    RCC_PLLCmd(ENABLE); //ʹ��PLL

    while(1)
    {
        if(RCC->CR & 0x02000000) //��ѯPLL ready
        {
            break;
        }
    }

    FLASH_SetLatency(FLASH_Latency_2);//ʱ�ӵȴ�״̬

    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //ͨ��PA8 pin �۲�Ƶ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//ѡ��PLLʱ����Ϊϵͳʱ��
}




/********************************************************************************************************
**������Ϣ ��HSE_Pll_test(unsigned int pllmul)
**�������� ���ⲿ������Ϊʱ��Դ
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16
**������� ����
********************************************************************************************************/

void HSI_Pll_test(unsigned int pllmul)
{

    SystemClk_HSIInit();
    //PLL ����
    RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul); //ѡ���ڲ�ʱ��Դ��Ϊ��Ƶʱ�ӵĻ���
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**������Ϣ ��LSI_clk()
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 40KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSI_clk()
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //open lsi on
    RCC_LSICmd(ENABLE);

    while(1)
    {
        if(RCC->CSR & 0x00000002) //��ѯ�ڲ���Ƶʱ��ready
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSI);  //ͨ��PA8 pin �۲�Ƶ��
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


