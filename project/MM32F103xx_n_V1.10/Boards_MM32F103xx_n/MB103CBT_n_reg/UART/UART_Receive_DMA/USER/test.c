#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dma.h"

u8 DST_Buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //UART DMA���ܵ����ݴ������


int main(void)
{
    u16 i;

    uart_init(SystemCoreClock / 1000000, 115200);	 	//���ڳ�ʼ��Ϊ115200
    delay_init();	   	 	//��ʱ��ʼ��
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    //��ʾ��ʾ��Ϣ
    printf("Please enter 10 numbers!\r\n");
    MYDMA_Config(DMA1_Channel5, (u32)&UART1->RDR, (u32)DST_Buffer, 10); //DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.

    UART1->GCR |= 1 << 1;       //ѡ�񴮿�1��DMA��ʽ����
    UART1->GCR |= 1 << 3;       //ʹ�ܴ���1��   ����
    MYDMA_Enable(DMA1_Channel5);//��ʼһ��DMA���䣡
    while(1)
    {
        if(DMA1->ISR & (1 << 17)) //�ȴ�ͨ��4�������
        {
            DMA1->IFCR |= 1 << 17; //���ͨ��4������ɱ�־
            break;
        }
    }
    for(i = 0; i < 10; i++)
    {
        printf("DST_Buffer[%d]==%d\r\n", i, (DST_Buffer[i] - 0x30));
    }
    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);

}


