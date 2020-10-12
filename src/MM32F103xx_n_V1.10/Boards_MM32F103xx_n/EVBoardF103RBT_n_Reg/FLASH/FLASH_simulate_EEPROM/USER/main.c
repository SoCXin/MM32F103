#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "key.h"
#include "wrflash.h"

//要写入到MCU	 FLASH的字符串数组
const u8 TEXT_Buffer[] = {"MM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 	//数组长度
#define FLASH_SAVE_ADDR  0X08005000 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

int main(void)
{
    u8 key;
    u16 i = 0;
    u8 datatemp[SIZE];
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(9600);	 //串口初始化为9600
    LED_Init();		  	 //初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "FLASH EEPROM TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 100, 2, 1, "WK_UP:Write KEY4:Read");
    while(1)
    {
        key = KEY_Scan(0);
        if(key == WKUP_PRES) //WK_UP按下,写入 FLASH
        {
            putStr(110, 120, 2, 1, "Start Write FLASH....");
            ONCHIP_FLASH_Write(FLASH_SAVE_ADDR, (u16*)TEXT_Buffer, SIZE);
            putStr(110, 120, 2, 1, "FLASH Write Finished!");
        }
        if(key == KEY4_PRES) //KEY0按下,读取字符串并显示
        {
            putStr(110, 120, 2, 1, "Start Read FLASH.... ");
            ONCHIP_FLASH_Read(FLASH_SAVE_ADDR, (u16*)datatemp, SIZE);
            putStr(110, 120, 2, 1, "The Data Readed Is:  ");
            putStr(110, 140, 2, 1, datatemp);
        }
        i++;
        delay_ms(10);
        if(i == 20)
        {
            LED1 = !LED1; //提示系统正在运行
            i = 0;
        }
    }

}


