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
extern void I2CMasterTest(I2C_TypeDef *I2Cx);
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴��ڴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{

    delay_init();	    	 			//��ʱ������ʼ��
    uart_initwBaudRate(115200);	 				//���ڳ�ʼ��Ϊ115200
    I2CMasterTest(I2C1);				//��·ʹ��I2C����ΪPB8,PB9,��I2C1����IO��

    while(1)              				//����ѭ��
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

