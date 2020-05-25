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
#include "lcdc.h"
#include "led.h"
#include "i2c.h"
#include "key.h"

u8 tmpdata[] = {"123456789"};
#define TX_SIZE sizeof(tmpdata)
#define RX_SIZE  TX_SIZE
u8 rxtmpdata[RX_SIZE];

#define FLASH_DEVICE_ADDR 0xa0

int main(void)
{
    u8 key;
    u8 i = 0;
    delay_init();	    	 			//延时函数初始化
    LED_Init();		  	 //初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化

    I2CInitMasterMode(I2C2, 400000); //初始化与I2C连接的硬件接口
    I2CSetDeviceAddr(I2C2, FLASH_DEVICE_ADDR);
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "I2C TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 100, 2, 1, "KEY4:Write Wakeup:Read");

    while(1)
    {
        key = KEY_Scan(0);
        if(key == KEY4_PRES)
        {
            putStr(110, 130, 2, 1, "Start Write EEPROM....");
            E2PROM_Write( I2C2, 0, TX_SIZE, tmpdata );
            putStr(110, 130, 2, 1, "EEPROM Write Over!    ");
        }
        else  if(key == WKUP_PRES)
        {
            putStr(110, 130, 2, 1, "Start Read EEPROM.... ");
            E2PROM_Read( I2C2, 0, RX_SIZE, rxtmpdata );
            putStr(110, 130, 2, 1, "The Data Readed Is:      ");
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











