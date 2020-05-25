/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
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
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "led.h"
u8 DST_Buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //初始化并清空接收中断
/* Private function prototypes -----------------------------------------------*/
void DMA_Configuration(void);

u8 TestStatus = 0; //DMA传输完成标志位
/********************************************************************************************************
**函数信息 ：DMA1_Channel5_IRQHandler(void)
**功能描述 ：DMA通道5传输完成中断
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL5);

        /* Check the received buffer */
        TestStatus = 1;
        LED2 = !LED2;

    }
}
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，需要使用串口助手发送10个数据，然后串口再将接受到的数据打印出来
**输入参数 ：
**输出参数 ：
**函数备注 ：串口助手不可以选择发送新行（串口助手的配置选择）
********************************************************************************************************/
int main(void)
{
    //GPIO口测试，闪烁LED灯
    u8 i;

    LED_Init();//LED灯闪烁测试GPIO
    LED1 = 0;
    uart_initwBaudRate(115200);
    printf("Please enter 10 numbers!\r\n");
    DMA_Configuration();//串口上位机软件不以选择发送新行
    LED1 = 1;

    while(1)
    {
        if(TestStatus == 1)
        {
            TestStatus = 0;
            for(i = 0; i < 10; i++)
                printf("DST_Buffer[%d]==%d\r\n", i, (DST_Buffer[i] - 0x30));
            printf("\r\n");
        }
    }
}
/********************************************************************************************************
**函数信息 ：DMA_Configuration(void)
**功能描述 ：UART接收DMA配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* UARTy_DMA1_Channel Config */
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (UART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the UARTy_DMA1_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable UARTy_DMA1_Channel Transfer complete interrupt */
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE); //????1?DMA??
    /* UARTy_DMA1_Channel enable */
    DMA_Cmd(DMA1_Channel5, ENABLE);
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
