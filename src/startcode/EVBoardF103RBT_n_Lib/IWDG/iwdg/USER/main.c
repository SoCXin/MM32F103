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
#include "led.h"
#include "key.h"
#include "iwdg.h"


int main(void)
{
    delay_init();	    	 //延时函数初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    KEY_Init();          //按键初始化
    delay_ms(300);   	 //让人看得到灭
    IWDG_Init(4, 625);   //与分频数为64,重载值为625,溢出时间为1s
    LED1 = 0;				 //点亮LED1
    while(1)
    {
        if(KEY_Scan(0) == WKUP_PRES)
            IWDG_Feed();//如果WK_UP按下,则喂狗
        delay_ms(100);
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
