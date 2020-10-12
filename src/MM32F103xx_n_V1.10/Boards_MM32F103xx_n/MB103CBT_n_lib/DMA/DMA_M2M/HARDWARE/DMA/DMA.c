/**
******************************************************************************
* @file    dma.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the dma firmware functions.
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
#include "uart.h"


unsigned char dma1Flag = 0x0;
unsigned char dma2Flag = 0x0;

/********************************************************************************************************
**函数信息 ：DMA1_Channel1_IRQHandler()
**功能描述 : DMA1通道1的中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel1_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        dma1Flag = 0x1;
    }
}

/********************************************************************************************************
**函数信息 ：DMA1_Channel2_IRQHandler()
**功能描述 : DMA1通道2的中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel2_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        dma2Flag = 0x1;
    }

}

/********************************************************************************************************
**函数信息 ：DMAcheckStatus(uint32_t DMA_FLAG)
**功能描述 : 查询DMA的标志位
**输入参数 ：uint32_t DMA_FLAG，DMA的状态标志位
**输出参数 ：无
********************************************************************************************************/
void DMAcheckStatus(uint32_t DMA_FLAG)
{
    while(1)
    {
        if(DMA_GetFlagStatus(DMA_FLAG))
        {
            DMA_ClearFlag(DMA_FLAG);
            break;
        }
    }
}

/********************************************************************************************************
**函数信息 ：DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
**功能描述 : DMA通道失能
**输入参数 ：DMA_Channel_TypeDef* DMAy_Channelx，选择通道
**输出参数 ：无
********************************************************************************************************/
void DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
{
    //disable DMA_EN
    DMA_Cmd(DMAy_Channelx, DISABLE);
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom8_test()
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以8位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void dma_m8tom8_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned char *)(0x20003000 + i)) = i + 1;
        //*((volatile unsigned int *)(0x20003000+i*4)) = 0x55;
    }

    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

    dma2Flag = 0x0;

    DMA_Cmd(DMA1_Channel2, ENABLE);

    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }


    DMA_Cmd(DMA1_Channel2, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = *((volatile unsigned char *)(0x20004000 + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom16_test()
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以16位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void dma_m8tom16_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned char *)(0x20003000 + i)) = i + 1;

    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


    DMA_DeInit(DMA1_Channel1);


    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    dma1Flag = 0x0;

    DMA_Cmd(DMA1_Channel1, ENABLE);

    while(1)
    {
        if(dma1Flag)
        {
            dma1Flag = 0x0;
            break;
        }
    }

    DMA_Cmd(DMA1_Channel1, DISABLE);
    for(i = 0; i < 128; i++)
    {

        temp = *((volatile unsigned char *)(0x20004000 + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom32_test()
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以32位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m8tom32_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned char *)(0x20003000 + i)) = i + 1;

    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


    DMA_DeInit(DMA1_Channel3);


    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC3);

    DMA_Cmd(DMA1_Channel3, DISABLE);
    for(i = 0; i < 256; i++)
    {

        temp = *((volatile unsigned char *)(0x20004000 + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom8_test()
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以8位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom8_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned short *)(0x20003000 + (i * 2))) = 0xb1b0 + i;

    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel4);

    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);


    DMA_Cmd(DMA1_Channel4, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC4);

    DMA_Cmd(DMA1_Channel4, DISABLE);
    for(i = 0; i < 32; i++)
    {

        temp = *((volatile unsigned char *)(0x20004000 + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom16_test()
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以16位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom16_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned short *)(0x20003000 + (i * 2))) = 0xa1a0 + i;

    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


    DMA_DeInit(DMA1_Channel5);


    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);


    DMA_Cmd(DMA1_Channel5, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC5);

    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = *((volatile unsigned short *)(0x20004000 + (i * 2)));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom32_test()
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以32位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom32_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    for(i = 0; i < 64; i++)
    {
        *((volatile unsigned short *)(0x20003000 + (i * 2))) = 0xc1c0 + i;

    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


    DMA_DeInit(DMA1_Channel6);


    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel6, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC6);

    DMA_Cmd(DMA1_Channel6, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = *((volatile unsigned int *)(0x20004000 + (i * 4)));

        printf("temp%d=0x%x\r\n", i, temp);
    }
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
