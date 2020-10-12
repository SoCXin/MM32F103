/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main functions.
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
#include "uart_nvic.h"
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，串口发送数据，并等待上位机发送信息，然后再打印出来
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    uart_nvic_init(115200);	 //串口初始化为115200

    while(1)
    {
        if(UART_RX_STA & 0x8000)
        {
            len = UART_RX_STA & 0x3fff; //得到此次接收到的数据长度
            printf("\r\n您发送的消息为:\r\n");
            for(t = 0; t < len; t++)
            {
                while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET);
                UART_SendData(UART1, (u8)UART_RX_BUF[t]);
            }
            printf("\r\n\r\n");//插入换行
            UART_RX_STA = 0;
        }
        else
        {
            times++;
            if(times % 5000 == 0)
            {
                printf("\r\n串口实验\r\n");
            }
            if(times % 200 == 0)printf("请输入数据,以回车键结束\r\n");
            if(times % 30 == 0)LED1 = !LED1; //闪烁LED,提示系统正在运行.
            delay_ms(10);
        }
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
