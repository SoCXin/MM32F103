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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    uart_initwBaudRate(9600);	 //���ڳ�ʼ��Ϊ9600
    printf("\r\nDMA DATA:\r\n");
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();				//������ʼ��
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 100); //DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
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
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "DMA TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    while(1)
    {
        t = KEY_Scan(0);
        if(t == WKUP_PRES) //wakeup����
        {
            putStr(110, 100, 2, 1, "Start Transimit....");
            putStr(135, 120, 2, 1, " %");

            UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE); //????1?DMA??
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
                pro = DMA_GetCurrDataCounter(DMA1_Channel4); //�õ���ǰ��ʣ����ٸ�����
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

