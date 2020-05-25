#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "wkup.h"

extern u8 EnterStandby;

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    u32 t = 0;
    char str[20];
    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(9600);	 	//���ڳ�ʼ��Ϊ9600
    LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
    WKUP_Init();			//��ʼ��WK_UP������ͬʱ����Ƿ�����������
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1,  "Wakeup TEST");
    putStr(110, 60, 2, 1,  "@MindMotion");
    putStr(110, 80, 2, 1,  "2016/09/02");
    while(1)
    {
        if(EnterStandby == 1)
        {
            lcdFillColor(Black);
            NVIC_SystemReset();
        }
        LED2 = !LED2;
        sprintf(str, "%d", t);
        putStr(110, 100, 2, 1, str);
        t++;
        delay_ms(250);
    }
}


