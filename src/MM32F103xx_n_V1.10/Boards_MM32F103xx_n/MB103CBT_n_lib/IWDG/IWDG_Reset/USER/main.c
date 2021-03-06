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
#include "iwdg.h"

/*main.c文件内函数声明*/

/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：若不在死循环中喂狗，系统一直复位，不断打印uart ok！；反之则只上电打印一次
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{

    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200
    printf("uart ok!\r\n");
    /*配置为LSI32分频,计数器初始值为0x7ff,复位时长约为1.6s*/
    Write_Iwdg_ON(IWDG_Prescaler_32, 0xf);
    while(1)//无限循环
    {
        //无复位程序进入死循环,反正系统一直打印串口数据
        Write_Iwdg_RL();
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
