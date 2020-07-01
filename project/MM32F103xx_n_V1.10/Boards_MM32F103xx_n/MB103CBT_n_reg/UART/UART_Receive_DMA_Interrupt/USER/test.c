#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dma.h"

u8 DST_Buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //UART DMA���ܵ����ݴ������
u8 TestStatus = 0; //DMA������ɱ�־λ

/********************************************************************************************************
**������Ϣ ��DMA1_Channel5_IRQHandler(void)
**�������� ��DMAͨ��5��������ж�
**������� ����
**������� ����
********************************************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA1->ISR & 0x00020000)
    {
        DMA1->IFCR |= 0x00010000;
        /* Check the received buffer */
        TestStatus = 1;
        LED2 = !LED2;
    }
}


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
        if(TestStatus == 1)
        {
            TestStatus = 0;
            for(i = 0; i < 10; i++)
                printf("DST_Buffer[%d]==%d\r\n", i, (DST_Buffer[i] - 0x30));
            printf("\r\n");
        }
    }
}


