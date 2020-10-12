/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
******************************************************************************
*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "bkp.h"
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，LED闪烁
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 i;

    delay_init();	    	 //延时函数初始?

    LED_Init();		  		//初始化与LED连接的硬件接口
    i = BKP_DATA();		 //往BKP写数据，如果成功则LED满闪10下，失败则快速闪烁

    if(i == 0) //写数据正确，LED闪烁5下
    {
        for(i = 0; i < 10; i++)
        {
            LED1 = !LED1;
            LED2 = !LED2;

            delay_ms(1000);
        }
    }
    else//写数据不正确，LED极速闪烁
    {
        while(1)
        {
            LED1 = !LED1;
            LED2 = !LED2;

            delay_ms(50);
        }
    }

    while(1)
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
