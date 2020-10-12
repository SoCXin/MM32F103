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

extern void SPI_master_DMA_RX(void);


/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴������ִ�ӡSPI�Է����յ�����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{

    //spi�Ĳ���
    //��Ҫ��MISO��MOSI�̽�
    //ע�⣺�ı�SPI1��SP2�������϶�Ӧ��·ñҲӦ�ı�
    uart_initwBaudRate(115200);	 		//���ڳ�ʼ��Ϊ115200
    SPI_master_DMA_RX();   //��Ҫ�̽�PB14��PB15

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

