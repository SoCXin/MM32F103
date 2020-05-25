/**
******************************************************************************
* @file    internal_temperature.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the internal_temperature firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "tsensor.h"
#include "lcdc.h"

//开发板范例代码3
//串口实验
int main(void)
{

    u16 average = 0;
    float temperate;
    u8 i;
    unsigned int uiADData = 0;
    unsigned char tem[20];
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    uart_initwBaudRate(115200);	 //串口初始化为115200
    LED_Init();		  	 //初始化与LED连接的硬件接口
    T_Adc_Init();		  		//ADC初始化
    initLCD();

    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "Temperature TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 120, 2, 1, "TEMP_VAL:");
    putStr(230, 120, 2, 1, "C");
    while(1)
    {
        for(i = 0; i < 10; i++)
        {
            T_Get_Adc_Average(ADC_Channel_8, &uiADData);
            average += uiADData;
            if(i == 9)
                average = average / 10;
        }
        temperate = (float)average * (3.3 / 4095);		//电压值
        temperate = (temperate - 1.5) / 0.00478 + 25;	//转换为温度值

        sprintf(tem, "%.1f", temperate);
        putStr(185, 120, 2, 1, tem);
        delay_ms(250);
        average = 0;

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
