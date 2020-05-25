#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "key.h"
#include "dma.h"
#include "lcdc.h"

u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

int main(void)
{
    u16 i;
    u8 t = 0;
    char str[20];
    float pro = 0;			//����
    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    printf("\r\nDMA DATA:\r\n");
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 100); //DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "DMA TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) //���ASCII�ַ�������
    {
        if(t >= TEXT_LENTH) //���뻻�з�
        {
            SendBuff[i++] = 0x0d;
            SendBuff[i] = 0x0a;
            t = 0;
        }
        else SendBuff[i] = TEXT_TO_SEND[t++]; //����TEXT_TO_SEND���
    }
    while(1)
    {
        t = KEY_Scan(0);
        if(t == WKUP_PRES) //KEY0����
        {
            putStr(110, 100, 2, 1, "Start Transimit....");
            putStr(135, 120, 2, 1, " %");
            UART1->GCR |= 1 << 1;       //ѡ�񴮿�1��DMA��ʽ����
            UART1->GCR |= 1 << 4;       //ʹ�ܴ���1�ķ���
            MYDMA_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡
            //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
            //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
            while(1)
            {
                if(DMA1->ISR & (1 << 13)) //�ȴ�ͨ��4�������
                {
                    DMA1->IFCR |= 1 << 13; //���ͨ��4������ɱ�־
                    break;
                }
                pro = DMA1_Channel4->CNDTR; //�õ���ǰ��ʣ����ٸ�����
                pro = 1 - pro / ((TEXT_LENTH + 2) * 100); //�õ��ٷֱ�
                pro *= 100;    //����100��
                sprintf(str, "%.1f", pro);
                putStr (110, 120, 2, 1, str);
            }
            putStr (110, 120, 2, 1, "100 ");
            putStr(110, 140, 2, 1, "Transimit Finished!");
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
