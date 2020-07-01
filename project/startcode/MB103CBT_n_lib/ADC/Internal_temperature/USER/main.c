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
#include "tsensor.h"

//�����巶������16
//�ڲ��¶ȴ�����ʵ��

int main(void)
{
    u16 adcx;
    float temperate;
    delay_init();	    	 //��ʱ������ʼ��
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    T_Adc_Init();		  		//ADC��ʼ��

    printf("MiniBoard �ڲ��¶ȴ���������\r\n");
    while(1)
    {
        adcx = Get_Adc_Average(5);
        temperate = (float)adcx * (3.3 / 4095);		//��ѹֵ
        temperate = (temperate - 1.5) / 0.00478 + 25;	//ת��Ϊ�¶�ֵ
        printf("��ǰ���оƬ�ڲ��¶�Ϊ:%.2f��\r\n", temperate);
        delay_ms(300);
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
