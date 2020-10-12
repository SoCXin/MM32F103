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
#include "wkup.h"
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬽�PA0��GND�̽�
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{

    delay_init();	    	                           //��ʱ������ʼ?
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    uart_initwBaudRate(115200);	 	                 //���ڳ�ʼ��Ϊ115200
    printf("�밴��wake up����3�벻��\r\n");
    LED_Init();		  		                           //��ʼ����LED���ӵ�Ӳ���ӿ�
    WKUP_Init();			                             //��ʼ��WK_UP������ͬʱ����Ƿ�����������
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(250);//��ʱ250ms
        printf("�Ѵ�standbyģʽ����\r\n");
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

