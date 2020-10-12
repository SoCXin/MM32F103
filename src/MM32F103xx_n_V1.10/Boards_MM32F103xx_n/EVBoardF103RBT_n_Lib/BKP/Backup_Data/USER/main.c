/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
******************************************************************************
*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "bkp.h"
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��������LED��˸
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 i;

    delay_init();	    	 //��ʱ������ʼ?

    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    i = BKP_DATA();		 //��BKPд���ݣ�����ɹ���LED����10�£�ʧ���������˸

    if(i == 0) //д������ȷ��LED��˸5��
    {
        for(i = 0; i < 10; i++)
        {
            LED1 = !LED1;
            LED2 = !LED2;

            delay_ms(1000);
        }
    }
    else//д���ݲ���ȷ��LED������˸
    {
        while(1)
        {
            LED1 = !LED1;
            LED2 = !LED2;

            delay_ms(50);
        }
    }

    while(1)
    {
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
