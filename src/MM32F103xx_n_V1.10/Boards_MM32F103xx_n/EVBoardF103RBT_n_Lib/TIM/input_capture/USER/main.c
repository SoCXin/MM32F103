/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main functions.
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
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
#include "timer.h"

extern u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态
extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值

int main(void)
{
    u32 temp = 0;
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);
    LED_Init();		  	//初始化与LED连接的硬件接口
    TIM2_Cap_Init(0XFFFF, SystemCoreClock / 1000000 - 1);		//以1Mhz的频率计数
    while(1)
    {
        delay_ms(10);
        TIM_SetCompare1(TIM1, TIM_GetCapture1(TIM1) + 1);
        if(TIM_GetCapture1(TIM1) == 300)TIM_SetCompare1(TIM1, 0);
        if(TIM2CH1_CAPTURE_STA & 0X80) //成功捕获到了一次高电平
        {
            temp = TIM2CH1_CAPTURE_STA & 0X3F;
            temp *= 65536;					//溢出时间总和
            temp += TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
            printf("HIGH:%d us\r\n", temp);	//打印总的高点平时间
            TIM2CH1_CAPTURE_STA = 0;			//开启下一次捕获
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
