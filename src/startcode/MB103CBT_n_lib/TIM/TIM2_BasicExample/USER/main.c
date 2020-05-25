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
#include "tim2.h"
extern u32 SystemCoreClock;

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/

unsigned int ucTim2Flag;
int main(void)
{
    unsigned int uiCnt = 0;
    ucTim2Flag = 0;

    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200
    /*PCLK1倍频后(96MHz)96分频,计数器值为1000,即1ms中断一次*/
    Tim2_UPCount_test(SystemCoreClock / 1000000 - 1, 1000 - 1);
    while(1)                                                                                               //无限循环
    {
        if(ucTim2Flag > 999)
        {
            ucTim2Flag = 0;

            /* 打印当前uiCnt数值 */
            printf("Time=0x%xs\r\n", uiCnt);
            uiCnt++;
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
