/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
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
#include "dma.h"


u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴��ڴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    u16 i;
    u8 t = 0;

    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
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

    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE); //ʹ��uart1 DMA
    MYDMA_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡
    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
    while(1)
    {
        if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET) //�ȴ�ͨ��4�������
        {
            DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
            break;
        }
    }

    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);
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
