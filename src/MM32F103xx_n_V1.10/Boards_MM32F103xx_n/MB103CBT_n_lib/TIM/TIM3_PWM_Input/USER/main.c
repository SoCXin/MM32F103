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
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：使用杜邦线连接板子上的PB7与PA6(Ain4)
**输入参数 ：
**输出参数 ：
**常用函数 ：uart_printf("0x%x\r\n",Uart1RxTest(UART1));   uart_printf("0x%x",sizeof(u32));
********************************************************************************************************/
extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
extern u32 SystemCoreClock;
int main(void)
{

    //PB7作为输入捕获引脚，连接PB7与PA6
    delay_init();	    	 //延时函数初始化

    uart_initwBaudRate(115200);
    TIM3_PWM_Init(1000 - 1, SystemCoreClock / 1000000 - 1); //1KHZ周期
    TIM_SetCompare1(TIM3, 200);		//设置占空比
    TIM4_PWMINPUT_INIT(0xffff, SystemCoreClock / 1000000 - 1); //pwm输入初始化以1M的频率捕捉
    printf("请连接MiniBoard上的PB7与PA6(Ain4)\r\n");
    while(1)
    {
        delay_ms(500);
        if(CollectFlag == 1)
        {
            printf("duty  	= %d%% \r\n", duty * 100 / period);				//打印占空比
            printf("cycle 	= %dKHz\r\n", 1000 / period); //打印周期另一种叫法
            printf("period  = %dus\r\n", period);   //打印周期
            CollectFlag = 0;
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


