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
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "wkup.h"

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    u32 t = 0;
    char str[20];
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(9600);	 	//串口初始化为9600
    LED_Init();				//初始化与LED连接的硬件接口
    WKUP_Init();			//初始化WK_UP按键，同时检测是否正常开机？
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1,  "Wakeup TEST");
    putStr(110, 60, 2, 1,  "@MindMotion");
    putStr(110, 80, 2, 1,  "2016/09/02");
    while(1)
    {
        LED2 = !LED2;
        sprintf(str, "%d", t);
        putStr(110, 100, 2, 1, str);
        t++;
        delay_ms(250);
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
