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
#include "wkup.h"
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，将PA0和GND短接
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{

    delay_init();	    	                           //延时函数初始?
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    uart_initwBaudRate(115200);	 	                 //串口初始化为115200
    printf("请按下wake up按键3秒不放\r\n");
    LED_Init();		  		                           //初始化与LED连接的硬件接口
    WKUP_Init();			                             //初始化WK_UP按键，同时检测是否正常开机？
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(250);//延时250ms
        printf("已从standby模式唤醒\r\n");
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

