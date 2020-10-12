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

extern void SPI_master_DMA_RX(void);


/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，串口助手打印SPI自发自收的数据
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{

    //spi的测试
    //需要将MISO和MOSI短接
    //注意：改变SPI1或SP2，板子上对应短路帽也应改变
    uart_initwBaudRate(115200);	 		//串口初始化为115200
    SPI_master_DMA_RX();   //需要短接PB14和PB15

    while(1)              		//无限循环
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

