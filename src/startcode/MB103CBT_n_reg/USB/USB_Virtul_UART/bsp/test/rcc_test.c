/**
  ******************************************************************************
  * @file    rcc_test.c
  * @brief   ����ϵͳʱ��
  *
  ******************************************************************************
  */
#include "rcc_test.h"


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
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // ���츴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_DeInit();
    /*HSE on*/
    /*CR�Ĵ���BIT16λ��HSEONλ����1�������������ⲿʱ��HSE��Ϊϵͳʱ��*/
    RCC_HSEConfig(RCC_HSE_ON);
    while(1)
    {
        if(RCC_WaitForHSEStartUp() != 0)
        {
            break;
        }
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);                                                                //ѡ���ⲿʱ����Ϊϵͳʱ��
    RCC_MCOConfig(RCC_MCO_SYSCLK);                                                                         //ͨ��PA8 pin �۲�Ƶ��
}

/********************************************************************************************************
**������Ϣ ��SystemClk_HSIInit (void)
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/

void SystemClk_HSIInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //MCO  PA8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // ���츴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*HSI on*/
    /*CR�Ĵ���BIT0λ��HSIONλ����1�������������ڲ�ʱ��HSI��Ϊϵͳʱ��*/
    RCC_HSICmd(ENABLE);

    RCC_MCOConfig(RCC_MCO_SYSCLK);                                                                         //ͨ��PA8 pin �۲�Ƶ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);                                                                //ѡ���ڲ�ʱ����Ϊϵͳʱ��
}


/********************************************************************************************************
**������Ϣ ��SystemClkPll(unsigned int pllmul)
**�������� ����ƵƵ����Ϊϵͳ����ʱ��
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16
**������� ����
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{

    RCC_PLLCmd(ENABLE);                                                                                    //ʹ��PLL

    while(1)
    {
        if(RCC->CR & 0x02000000)                                                                             //��ѯPLL ready
        {
            break;
        }
    }

    FLASH_SetLatency(FLASH_Latency_2);                                                                     //ʱ�ӵȴ�״̬

    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);                                                                    //ͨ��PA8 pin �۲�Ƶ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                                                             //ѡ��PLLʱ����Ϊϵͳʱ��
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
    /*PLL ����*/
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);
    SystemClkPll(pllmul);
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
    /*PLL ����*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul);                                                         //ѡ���ڲ�ʱ��Դ��Ϊ��Ƶʱ�ӵĻ���
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**������Ϣ ��LSI_clk(void)
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 40KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSI_clk(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //mco  pa8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // ���츴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*open lsi on*/
    RCC_LSICmd(ENABLE);

    while(1)
    {
        if(RCC->CSR & 0x00000002)                                                                            //��ѯ�ڲ���Ƶʱ��ready
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSI);                                                                            //ͨ��PA8 PIN �۲�Ƶ��
}


/********************************************************************************************************
**������Ϣ ��LSE_clk(void)
**�������� ���ⲿ��Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 32.768KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSE_clk(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /*MCO PA8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // ���츴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);                                                                           //ʹ��PWR_CR�е�DBPbit

    RCC_LSEConfig(RCC_LSE_ON);

    while(1)
    {
        /*��ѯ�ⲿ32.768KHz��Ƶʱ��ready*/
        if(RCC->BDCR & 0x00000002)
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSE);                                                                            //ͨ��PA8 pin �۲�Ƶ��
}

/********************************************************************************************************
**������Ϣ ��HSE_HSI_Change(void)
**�������� ���ⲿʱ�����ڲ�ʱ��֮����л�
**������� ����
**������� ����
********************************************************************************************************/

void HSE_HSI_Change(void)
{
    RCC_ClockSecuritySystemCmd(ENABLE);                                                                    //CSS on
    SystemClk_HSEInit();
    RCC->CR &= 0xfffeffff;                                                                                 //close HESON
}
