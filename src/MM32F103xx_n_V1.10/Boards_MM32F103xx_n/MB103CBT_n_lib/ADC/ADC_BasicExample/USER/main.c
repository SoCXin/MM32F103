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
#include "adc.h"


/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{
    u16 ADCVAL;
    float fValue;
    delay_init();	    	    //��ʱ������ʼ��
    uart_initwBaudRate(115200);	        //���ڳ�ʼ��Ϊ115200


    /*����ADC1Ϊ����ת��ģʽ,ͨ��1ʹ��*/
    ADC1_SingleChannel( ADC_Channel_1);
    while(1)
    {
        ADCVAL = Get_Adc_Average(ADC_Channel_1, 5);
        fValue = ((float)ADCVAL / 4095) * 3.3;
        printf("ADC1_CH_1=%.2fV\r\n", fValue);
        delay_ms(1000);
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




