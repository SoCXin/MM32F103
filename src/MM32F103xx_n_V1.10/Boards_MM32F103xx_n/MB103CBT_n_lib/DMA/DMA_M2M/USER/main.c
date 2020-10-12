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

extern void SystemClk_HSEInit(void);
extern void DMA_m8tom8_test(void);
extern void DMA_m8tom16_test(void);
extern void DMA_m8tom32_test(void);
extern void DMA_m16tom8_test(void);
extern void DMA_m16tom16_test(void);
extern void DMA_m16tom32_test(void);

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，串口助手打印DMA传输数据
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{
    //DMA的测试
    uart_initwBaudRate(115200);	 //串口初始化为115200

    //DMA_m8tom8_test();
    //DMA_m8tom16_test();
    //DMA_m8tom32_test();
    DMA_m16tom8_test();
    //DMA_m16tom16_test();
    //DMA_m16tom32_test();


    while(1)              //无限循环
    {

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


