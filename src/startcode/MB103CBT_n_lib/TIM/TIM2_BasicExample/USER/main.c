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
#include "tim2.h"
extern u32 SystemCoreClock;

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/

unsigned int ucTim2Flag;
int main(void)
{
    unsigned int uiCnt = 0;
    ucTim2Flag = 0;

    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    /*PCLK1��Ƶ��(96MHz)96��Ƶ,������ֵΪ1000,��1ms�ж�һ��*/
    Tim2_UPCount_test(SystemCoreClock / 1000000 - 1, 1000 - 1);
    while(1)                                                                                               //����ѭ��
    {
        if(ucTim2Flag > 999)
        {
            ucTim2Flag = 0;

            /* ��ӡ��ǰuiCnt��ֵ */
            printf("Time=0x%xs\r\n", uiCnt);
            uiCnt++;
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
