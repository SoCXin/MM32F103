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

#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "spi.h"
#include "lcdc.h"
#include "led.h"
#include "key.h"

unsigned char tmpdata[] = {"MM32 EVB SPI TEST!"};
#define TX_SIZE sizeof(tmpdata)
#define RX_SIZE  TX_SIZE
unsigned char rxtmpdata[TX_SIZE];
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：SPI_Flash读写以及擦除功能
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{
    u8 key;
    u8 i = 0;
    unsigned char str[20];
    delay_init();	    	 	//延时函数初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    uart_initwBaudRate(9600);	 		//串口初始化为9600
    KEY_Init();				//按键初始化
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "SPI TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(50, 100, 2, 1, "KEY4:Write KEY3:Erase Wakeup:Read");

    SPIM_Init(SPI1, 0x4); //4分频

    while(SPIM_ReadID(SPI1))
    {
        putStr(110, 130, 2, 1, "S25FL008A Check Failed!");
        delay_ms(500);
        putStr(110, 130, 2, 1, "Clear S25FL008A Over      ");
        delay_ms(500);
        LED1 = !LED1; //DS0闪烁
    }
    putStr(110, 130, 2, 1, "S25FL008A Ready!");

    while(1)              		//无限循环
    {
        key = KEY_Scan(0);
        if(key == KEY3_PRES)
        {
            putStr(110, 150, 2, 1, "Start clear S25FL008A....");
            SPIM_SectorErase(SPI1, 0);
            putStr(110, 150, 2, 1, "Clear S25FL008A Over      ");
            data_clear(rxtmpdata, RX_SIZE);
            putStr(110, 150, 2, 1, "                          ");
        }
        else if(key == KEY4_PRES)
        {
            putStr(110, 150, 2, 1, "Start Write S25FL008A....");
            SPIM_PageProgram(SPI1, 0, tmpdata, TX_SIZE);
            putStr(110, 150, 2, 1, "S25FL008A Write Over!    ");
        }
        else	if(key == WKUP_PRES)
        {
            putStr(110, 150, 2, 1, "Start Read S25FL008A.... ");
            SPIM_PageRead(SPI1, 0, rxtmpdata, RX_SIZE); 			//把写进去的一页256字节读出来
            putStr(110, 150, 2, 1, "The Data Readed Is:      ");
            putStr(110, 150, 2, 1, "                        ");
            putStr(110, 150, 2, 1, rxtmpdata);
        }
        delay_ms(10);
        i++;
        if(i == 20)
        {
            i = 0;
            LED2 = !LED2; //DS0闪烁
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
















