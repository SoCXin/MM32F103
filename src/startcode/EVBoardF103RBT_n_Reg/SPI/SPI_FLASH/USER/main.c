#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "spi.h"
#include "lcdc.h"
#include "led.h"
#include "key.h"

unsigned char tmpdata[] = {"MM32 DK103 SPI TEST"};
#define TX_SIZE sizeof(tmpdata)
#define RX_SIZE  TX_SIZE
unsigned char rxtmpdata[TX_SIZE];
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��SPI_Flash��д�Լ���������
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{
    u8 key;
    u8 i = 0;

    unsigned char str[20];
    delay_init();	    	 	//��ʱ������ʼ��
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_initwBaudRate(9600);	 		//���ڳ�ʼ��Ϊ9600
    KEY_Init();				//������ʼ��
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "SPI TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(50, 100, 2, 1, "KEY4:Write KEY3:Erase Wakeup:Read");

    SPIM_Init();//4��Ƶ

    while(SPIM_ReadID())
    {
        putStr(110, 130, 2, 1, "S25FL008A Check Failed!");
        delay_ms(500);
        putStr(110, 130, 2, 1, "Clear S25FL008A Over      ");
        delay_ms(500);
        LED1 = !LED1; //DS0��˸
    }
    putStr(110, 130, 2, 1, "S25FL008A Ready!");

    while(1)              		//����ѭ��
    {
        key = KEY_Scan(0);
        if(key == KEY3_PRES)
        {
            putStr(110, 150, 2, 1, "Start clear S25FL008A....");
            SPIM_SectorErase(0);
            putStr(110, 150, 2, 1, "Clear S25FL008A Over      ");
            data_clear(rxtmpdata, RX_SIZE);
            putStr(110, 150, 2, 1, "                          ");
        }
        else if(key == KEY4_PRES)
        {
            putStr(110, 150, 2, 1, "Start Write S25FL008A....");
            SPIM_PageProgram(0, tmpdata, TX_SIZE);
            putStr(110, 150, 2, 1, "S25FL008A Write Over!    ");
        }
        else	if(key == WKUP_PRES)
        {
            putStr(110, 150, 2, 1, "Start Read S25FL008A.... ");
            SPIM_PageRead(0, rxtmpdata, RX_SIZE); 			//��д��ȥ��һҳ256�ֽڶ�����
            putStr(110, 150, 2, 1, "The Data Readed Is:      ");
            putStr(110, 150, 2, 1, "                        ");
            putStr(110, 150, 2, 1, rxtmpdata);
        }
        delay_ms(10);
        i++;
        if(i == 20)
        {
            i = 0;
            LED2 = !LED2; //DS0��˸
        }
    }
}

