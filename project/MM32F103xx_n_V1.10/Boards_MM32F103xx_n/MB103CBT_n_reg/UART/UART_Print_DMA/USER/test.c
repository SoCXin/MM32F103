#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dma.h"

u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

int main(void)
{
    u16 i;
    u8 t = 0;

    uart_init(SystemCoreClock / 1000000, 115200);	 	//���ڳ�ʼ��Ϊ115200
    delay_init();	   	 	//��ʱ��ʼ��
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
    //��ʾ��ʾ��Ϣ
    printf("\r\nDMA DATA:\r\n");

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

    UART1->GCR |= 1 << 1;       //ѡ�񴮿�1��DMA��ʽ����
    UART1->GCR |= 1 << 4;       //ʹ�ܴ���1�ķ���
    MYDMA_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡
    while(1)
    {
        if(DMA1->ISR & (1 << 13)) //�ȴ�ͨ��4�������
        {
            DMA1->IFCR |= 1 << 13; //���ͨ��4������ɱ�־
            break;
        }
    }
    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);

}


