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
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "tim1.h"

/*main.c�ļ��ں�������*/
void DelayMs(u32 ulMs);                                                                                  //�Ǿ�ȷ��ʱ����
void NVIC_Configuration(void);
void SysInit(void);
extern u32 SystemCoreClock;
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{
    unsigned int uiCnt = 0;

    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200

    /*PCLK2ʱ��(96MHz)9600��Ƶ,������ֵΪ10000,��1s���һ��*/
    Tim1_UPCount_test(SystemCoreClock / 10000 - 1, 10000 - 1);
    while(1)                                                                                               //����ѭ��
    {
        for(uiCnt = 0; ; uiCnt++)
        {
            /* �ȴ���ʱ����� */
            Tim1_UPStatusOVCheck_test();

            /* ��ӡ��ǰ��ֵ */
            printf("0x%x\r\n", uiCnt);	//��ӡ��Ҫ����ʱ��
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
