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
**函数信息 ：int main (void)
**功能描述 ：开机后，串口发送数据，并等待上位机发送信息，然后再打印出来
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{

    //UART的测试
    //这里的UART程序不使用SYSTEM里的串口程序,而是HARDWARE里的程序
    UartPortInit(UART1);  //UART1的发送，可以通过串口软件打印UART OK

    while(1)              //无限循环
    {

        Uart1RxTest(UART1);//UART1的接收，在串口软件中输入字符，可以通过打印验证接收的数据是否正确

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

