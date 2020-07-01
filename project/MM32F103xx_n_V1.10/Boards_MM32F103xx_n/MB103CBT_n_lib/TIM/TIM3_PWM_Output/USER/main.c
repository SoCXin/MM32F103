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
#include "pwm.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：uart_printf("0x%x\r\n",Uart1RxTest(UART1));   uart_printf("0x%x",sizeof(u32));
********************************************************************************************************/
int main(void)
{
    u8 dir = 1;
    u16 led0pwmval = 0;

    delay_init();	    	 //延时函数初始化
    LED_Init();
    TIM3_PWM_Init(899, 0);

    while(1)                                                                                               //无限循环
    {
        delay_ms(4);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval > 500)dir = 0;
        if(led0pwmval == 0)dir = 1;
        TIM_SetCompare1(TIM3, led0pwmval);
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


