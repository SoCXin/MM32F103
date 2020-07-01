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
#include "key.h"
//�����巶������2
//��������ʵ��
int main(void)
{
    u8 t = 0;
    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    while(1)
    {
        t = KEY_Scan(0);		//�õ���ֵ
        switch(t)
        {
            case KEY1_PRES://K1Ĭ�ϲ���PC13,Ĭ������reset��λ���������԰���K1�Ḵλ
                LED1 = !LED1;
                break;
            case WKUP_PRES:
                LED2 = !LED2;
                break;
            case KEY3_PRES:
                LED3 = !LED3;
                break;
            case KEY4_PRES:
                LED4 = !LED4;
                break;
            default:
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
