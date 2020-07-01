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

extern void SPIM_Test(SPI_TypeDef* SPIx);

/********************************************************************************************************
**������Ϣ ��DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)
**�������� :�Աȷ�������������������޶Դ�
**������� ��*p1,*p2,count
**������� ����
********************************************************************************************************/
void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {
        if(p1[i] != p2[i])
            printf("tx[%d]=0x%x,rx[%d]=0x%x\r\n", i, p1[i], i, p2[i]);
    }
}

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴������ִ�ӡ256ҳ����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{

    //spi�Ĳ���
    //ע�⣺�ı�SPI1��SP2�������϶�Ӧ��·ñҲӦ�ı�
    delay_init();	    	 	//��ʱ������ʼ��
    uart_initwBaudRate(115200);	 		//���ڳ�ʼ��Ϊ115200
    SPIM_Test(SPI2);			//ֻ���޸Ĳ���
    while(1)              		//����ѭ��
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
