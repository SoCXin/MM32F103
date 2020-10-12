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
#include "malloc.h"
#include "lcdc.h"
#include "key.h"

int main(void)
{
    u8 key;
    u8 i = 0;
    u8 *p = 0;
    u8 *tp = 0;
    u8 paddr[18];			//���P Addr:+p��ַ��ASCIIֵ
    char str[20];
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    uart_initwBaudRate(9600);	 //���ڳ�ʼ��Ϊ9600
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "MALLOC TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(10, 100, 2, 1, "KEY4:Free KEY3:Write Data WK_UP:Malloc");
    putStr(110, 120, 2, 1, "SRAM USED:   %");
    while(1)
    {
        key = KEY_Scan(0); //��֧������
        switch(key)
        {
            case WKUP_PRES:
                p = mymalloc(2048);	//����2K�ֽ�
                if(p != NULL)sprintf((char*)p, "Memory Malloc Test%03d", i); //��pд��һЩ����
                break;
            case KEY3_PRES:		//KEY1����
                if(p != NULL)
                {
                    sprintf((char*)p, "Memory Malloc Test%03d", i); //������ʾ����
                    putStr(110, 160, 2, 1, p);
                }
                break;
            case KEY4_PRES:
                myfree(p);	//�ͷ��ڴ�
                p = 0;		//ָ��յ�ַ
                break;
        }
        if(tp != p)
        {
            tp = p;
            sprintf((char*)paddr, "P Addr:0X%08X", (u32)tp);
            putStr(110, 140, 2, 1, paddr);
            if(p)
                putStr(110, 160, 2, 1, p);
        }
        delay_ms(10);
        i++;
        if((i % 20) == 0) //DS0��˸.
        {
            sprintf(str, "%d", mem_perused());
            putStr(190, 120, 2, 1, "   " );
            putStr(190, 120, 2, 1, str );
            LED1 = !LED1;
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