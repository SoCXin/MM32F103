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
#include "sleep.h"

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
int main(void)
{

    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200
    printf("sleep mode ! \r\n");
    sleep_test();

    //按下K2，被唤醒
    while(1)//无限循环
    {
        printf("Wake Up ! \r\n");
        delay_ms(1000);
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


