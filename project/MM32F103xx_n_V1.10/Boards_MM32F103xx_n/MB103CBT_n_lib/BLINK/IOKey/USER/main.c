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
#include "key.h"
//开发板范例代码2
//按键输入实验
int main(void)
{
    u8 t = 0;
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
    while(1)
    {
        t = KEY_Scan(0);		//得到键值
        switch(t)
        {
            case KEY1_PRES://K1默认不连PC13,默认连接reset复位按键，所以按下K1会复位
                LED1 = !LED1;
                break;
            case WKUP_PRES:
                LED2 = !LED2;
                break;
            case KEY3_PRES:
                LED3 = !LED3;
                break;
            case KEY4_PRES:
                LED4 = !LED4;
                break;
            default:
                delay_ms(10);
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
