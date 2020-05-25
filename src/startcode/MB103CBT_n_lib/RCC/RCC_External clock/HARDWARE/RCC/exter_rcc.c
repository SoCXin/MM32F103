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
**������Ϣ ��SystemClk_HSEInit (void)
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/

void SystemClk_HSEInit(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_DeInit();
    //HSE on
    //CR�Ĵ���BIT16λ��HSEONλ����1�������������ⲿʱ��HSE��Ϊϵͳʱ��
    RCC_HSEConfig(RCC_HSE_ON);
    while(1)
    {
        if(RCC_WaitForHSEStartUp() != 0)
        {
            break;
        }
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);//ѡ���ⲿʱ����Ϊϵͳʱ��
    RCC_MCOConfig(RCC_MCO_SYSCLK);  //ͨ��PA8 pin �۲�Ƶ��
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

void HSE_Pll_test(unsigned int pllmul)
{

    SystemClk_HSEInit();
    //PLL ����
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul); //ѡ���ⲿʱ��Դ��Ϊ��Ƶʱ�ӵĻ���
    SystemClkPll(pllmul);
}






/********************************************************************************************************
**������Ϣ ��LSE_clk()
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 32.786KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSE_clk()
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE); //ʹ��PWR_CR�е�DBPbit

    RCC_LSEConfig(RCC_LSE_ON);

    while(1)
    {
        if(RCC->BDCR & 0x00000002) //��ѯ�ⲿ32.768KHz��Ƶʱ��ready
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSE);  //ͨ��PA8 pin �۲�Ƶ��
}

/********************************************************************************************************
**������Ϣ ��HSE_HSI_Change()
**�������� ���ⲿʱ�����ڲ�ʱ��֮����л�
**������� ����
**������� ����
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
