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


int main(void)
{
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	//初始化与LED连接的硬件接口
    while(1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_8); //LED0输出低
        GPIO_SetBits(GPIOC, GPIO_Pin_13); //LED1输出高
        delay_ms(300);
        GPIO_SetBits(GPIOA, GPIO_Pin_8); //LED0输出高
        GPIO_ResetBits(GPIOC, GPIO_Pin_13); //LED1输出低
        delay_ms(300);
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

