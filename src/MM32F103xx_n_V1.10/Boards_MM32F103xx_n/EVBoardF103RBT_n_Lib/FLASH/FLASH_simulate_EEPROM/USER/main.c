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
#include "wrflash.h"
#include "lcdc.h"
#include "key.h"

//Ҫд�뵽MCU	 FLASH���ַ�������
const u8 TEXT_Buffer[] = {"MM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 	//���鳤��
#define FLASH_SAVE_ADDR  0X08005000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
int main(void)
{
    u8 key;
    u16 i = 0;
    u8 datatemp[SIZE];

    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ9600
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    initLCD();

    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "FLASH EEPROM TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 100, 2, 1, "WK_UP:Write KEY4:Read");
    while(1)
    {
        key = KEY_Scan(0);
        if(key == WKUP_PRES) //WK_UP����,д�� FLASH
        {
            putStr(110, 120, 2, 1, "Start Write FLASH....");
            ONCHIP_FLASH_Write(FLASH_SAVE_ADDR, (u16*)TEXT_Buffer, SIZE);
            putStr(110, 120, 2, 1, "FLASH Write Finished!");
        }
        if(key == KEY4_PRES) //KEY0����,��ȡ�ַ�������ʾ
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
            LED1 = !LED1; //��ʾϵͳ��������
            i = 0;
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

