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

extern void SPIM_Test(SPI_TypeDef* SPIx);

/********************************************************************************************************
**函数信息 ：DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)
**功能描述 :对比发送数据与接收数据有无对错
**输入参数 ：*p1,*p2,count
**输出参数 ：无
********************************************************************************************************/
void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {
        if(p1[i] != p2[i])
            printf("tx[%d]=0x%x,rx[%d]=0x%x\r\n", i, p1[i], i, p2[i]);
    }
}

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，串口助手打印256页数据
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{

    //spi的测试
    //注意：改变SPI1或SP2，板子上对应短路帽也应改变
    delay_init();	    	 	//延时函数初始化
    uart_initwBaudRate(115200);	 		//串口初始化为115200
    SPIM_Test(SPI2);			//只需修改参数
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
