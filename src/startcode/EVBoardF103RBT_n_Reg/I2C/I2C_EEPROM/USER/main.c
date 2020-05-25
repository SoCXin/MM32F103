#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "i2c.h"
#include "key.h"

u8 tmpdata[] = {"12345678"};
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

    I2C_Master_Init();//初始化与I2C连接的硬件接口
    I2CSetDeviceAddr(FLASH_DEVICE_ADDR);
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
            E2PROM_Write(0, TX_SIZE, tmpdata );
            putStr(110, 130, 2, 1, "EEPROM Write Over!    ");
        }
        else  if(key == WKUP_PRES)
        {
            putStr(110, 130, 2, 1, "Start Read EEPROM.... ");
            E2PROM_Read(0, RX_SIZE, rxtmpdata );
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


