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
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "dma.h"


u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，串口打印数据
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    u16 i;
    u8 t = 0;

    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 	//串口初始化为115200
    LED_Init();		  		//初始化与LED连接的硬件接口
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.
    printf("\r\nDMA DATA:\r\n");
    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) //填充ASCII字符集数据
    {
        if(t >= TEXT_LENTH) //加入换行符
        {
            SendBuff[i++] = 0x0d;
            SendBuff[i] = 0x0a;
            t = 0;
        }
        else SendBuff[i] = TEXT_TO_SEND[t++]; //复制TEXT_TO_SEND语句
    }

    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE); //使能uart1 DMA
    MYDMA_Enable(DMA1_Channel4);//开始一次DMA传输！
    //等待DMA传输完成，此时我们来做另外一些事，点灯
    //实际应用中，传输数据期间，可以执行另外的任务
    while(1)
    {
        if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET) //等待通道4传输完成
        {
            DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
            break;
        }
    }

    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);
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
