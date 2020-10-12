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
#include "HAL_device.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"


extern void UartPortInit(UART_TypeDef* UARTx);
extern void Uart1RxTest(UART_TypeDef* UARTx);
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴��ڷ������ݣ����ȴ���λ��������Ϣ��Ȼ���ٴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{

    //UART�Ĳ���
    //�����UART����ʹ��SYSTEM��Ĵ��ڳ���,����HARDWARE��ĳ���
    UartPortInit(UART1);  //UART1�ķ��ͣ�����ͨ�����������ӡUART OK

    while(1)              //����ѭ��
    {

        Uart1RxTest(UART1);//UART1�Ľ��գ��ڴ�������������ַ�������ͨ����ӡ��֤���յ������Ƿ���ȷ

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

