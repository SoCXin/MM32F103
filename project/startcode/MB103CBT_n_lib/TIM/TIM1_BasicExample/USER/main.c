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
#include "tim1.h"

/*main.c文件内函数声明*/
void DelayMs(u32 ulMs);                                                                                  //非精确延时函数
void NVIC_Configuration(void);
void SysInit(void);
extern u32 SystemCoreClock;
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
int main(void)
{
    unsigned int uiCnt = 0;

    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200

    /*PCLK2时钟(96MHz)9600分频,计数器值为10000,即1s溢出一次*/
    Tim1_UPCount_test(SystemCoreClock / 10000 - 1, 10000 - 1);
    while(1)                                                                                               //无限循环
    {
        for(uiCnt = 0; ; uiCnt++)
        {
            /* 等待定时器溢出 */
            Tim1_UPStatusOVCheck_test();

            /* 打印当前数值 */
            printf("0x%x\r\n", uiCnt);	//打印需要消耗时间
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
