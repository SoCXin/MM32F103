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
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "can.h"
CanPeliRxMsg CanPeliRxMsgStructure;
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬽��յ����Ĳ���ӡ
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    u8 i = 0;
    delay_init();
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
    CANM_Init();
    CAN_Config(CAN_250K, ExtendedFrame_DoubleFilter, 0x172, 0x325, 0x00, 0x00);		//CAN�������μĴ�������λ����Ϊ��أ���ֻ�ɽ������մ���Ĵ����е�����CAN_ID
    while(1)
    {
        if(flag)
        {
            flag = 0;
            printf("CANID:0x%x  Data", CanPeliRxMsgStructure.ID);	//���ڴ�ӡ
            for(i = 0; i < 8; i++)
            {
                printf("%x", CanPeliRxMsgStructure.Data[i]);		//��ӡCAN��������
            }
            printf("\r\n");																		//����
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


