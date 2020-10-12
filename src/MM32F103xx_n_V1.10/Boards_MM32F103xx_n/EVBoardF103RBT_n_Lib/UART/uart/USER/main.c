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
#include "uart_nvic.h"
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴��ڷ������ݣ����ȴ���λ��������Ϣ��Ȼ���ٴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    uart_nvic_init(115200);	 //���ڳ�ʼ��Ϊ115200

    while(1)
    {
        if(UART_RX_STA & 0x8000)
        {
            len = UART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
            printf("\r\n�����͵���ϢΪ:\r\n");
            for(t = 0; t < len; t++)
            {
                while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET);
                UART_SendData(UART1, (u8)UART_RX_BUF[t]);
            }
            printf("\r\n\r\n");//���뻻��
            UART_RX_STA = 0;
        }
        else
        {
            times++;
            if(times % 5000 == 0)
            {
                printf("\r\n����ʵ��\r\n");
            }
            if(times % 200 == 0)printf("����������,�Իس�������\r\n");
            if(times % 30 == 0)LED1 = !LED1; //��˸LED,��ʾϵͳ��������.
            delay_ms(10);
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
