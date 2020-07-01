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
#include "adc.h"
#include "lcdc.h"

int main(void)
{

    char ucFlag = 0;
    char vol[20];
    char val[20];
    unsigned int uiADData = 0;
    float fValue = 0;

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    uart_initwBaudRate(115200);	 //串口初始化为115200
    LED_Init();		  	 //初始化与LED连接的硬件接口

    /*配置ADC1为单次转换模式,通道1使能*/
    ADC1_SingleChannel( ADC_Channel_1);
    initLCD();

    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "ADC TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 120, 2, 1, "ADC_CH1_VAL:");
    putStr(110, 140, 2, 1, "ADC_CH1_VOL:0.000V");

    while(1)
    {
        /*读取当前转换数据*/
        ucFlag = ADC1_SingleChannel_Get( ADC_Channel_1, &uiADData);
        if(ucFlag == 1)
        {
            ucFlag = 0;
            fValue = ((float)uiADData / 4095) * 3.3;
            sprintf(val, "%d", uiADData);
            sprintf(vol, "%.3f", fValue);
            putStr(205, 120, 2, 1, "    ");
            putStr(205, 120, 2, 1, val);
            putStr(205, 140, 2, 1, vol);
            delay_ms(300);
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
