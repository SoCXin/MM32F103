/**
  ******************************************************************************
  * @file    rcc_test.c
  * @brief   配置系统时钟
  *
  ******************************************************************************
  */
#include "rcc_test.h"


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
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // 推挽复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_DeInit();
    /*HSE on*/
    /*CR寄存器BIT16位（HSEON位）置1，作用是连接外部时钟HSE作为系统时钟*/
    RCC_HSEConfig(RCC_HSE_ON);
    while(1)
    {
        if(RCC_WaitForHSEStartUp() != 0)
        {
            break;
        }
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);                                                                //选择外部时钟作为系统时钟
    RCC_MCOConfig(RCC_MCO_SYSCLK);                                                                         //通过PA8 pin 观察频率
}

/********************************************************************************************************
**函数信息 ：SystemClk_HSIInit (void)
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void SystemClk_HSIInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //MCO  PA8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // 推挽复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*HSI on*/
    /*CR寄存器BIT0位（HSION位）置1，作用是连接内部时钟HSI作为系统时钟*/
    RCC_HSICmd(ENABLE);

    RCC_MCOConfig(RCC_MCO_SYSCLK);                                                                         //通过PA8 pin 观察频率
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);                                                                //选择内部时钟作为系统时钟
}


/********************************************************************************************************
**函数信息 ：SystemClkPll(unsigned int pllmul)
**功能描述 ：倍频频率作为系统工作时钟
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16
**输出参数 ：无
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{

    RCC_PLLCmd(ENABLE);                                                                                    //使能PLL

    while(1)
    {
        if(RCC->CR & 0x02000000)                                                                             //查询PLL ready
        {
            break;
        }
    }

    FLASH_SetLatency(FLASH_Latency_2);                                                                     //时钟等待状态

    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);                                                                    //通过PA8 pin 观察频率
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                                                             //选择PLL时钟作为系统时钟
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
    /*PLL 配置*/
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**函数信息 ：HSE_Pll_test(unsigned int pllmul)
**功能描述 ：外部晶振作为时钟源
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16
**输出参数 ：无
********************************************************************************************************/

void HSI_Pll_test(unsigned int pllmul)
{
    SystemClk_HSIInit();
    /*PLL 配置*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul);                                                         //选择内部时钟源作为倍频时钟的基础
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**函数信息 ：LSI_clk(void)
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 40KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void LSI_clk(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                                                             //mco  pa8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // 推挽复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*open lsi on*/
    RCC_LSICmd(ENABLE);

    while(1)
    {
        if(RCC->CSR & 0x00000002)                                                                            //查询内部低频时钟ready
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSI);                                                                            //通过PA8 PIN 观察频率
}


/********************************************************************************************************
**函数信息 ：LSE_clk(void)
**功能描述 ：外部低频时钟并通过示波器观察PA8 pin 32.768KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void LSE_clk(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /*MCO PA8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                        // 推挽复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);                                                                           //使能PWR_CR中的DBPbit

    RCC_LSEConfig(RCC_LSE_ON);

    while(1)
    {
        /*查询外部32.768KHz低频时钟ready*/
        if(RCC->BDCR & 0x00000002)
        {
            break;
        }
    }

    RCC_MCOConfig(RCC_MCO_LSE);                                                                            //通过PA8 pin 观察频率
}

/********************************************************************************************************
**函数信息 ：HSE_HSI_Change(void)
**功能描述 ：外部时钟与内部时钟之间的切换
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void HSE_HSI_Change(void)
{
    RCC_ClockSecuritySystemCmd(ENABLE);                                                                    //CSS on
    SystemClk_HSEInit();
    RCC->CR &= 0xfffeffff;                                                                                 //close HESON
}
