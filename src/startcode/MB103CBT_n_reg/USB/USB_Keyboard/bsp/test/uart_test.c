/**
  ******************************************************************************
  * @file    uart_test.c
  * @brief   配置串口功能
  *
  ******************************************************************************
  */
#include "uart_test.h"
/********************************************************************************************************
**函数信息 ：UartInit(UART_TypeDef* UARTx)
**功能描述 ：初始化串口
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：无
********************************************************************************************************/

void UartInit(UART_TypeDef* UARTx)
{
    UART_InitTypeDef       UART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    if(UARTx == UART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;                                                          //uart1_tx  pa9
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                     // 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                         //uart1_rx  pa10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                       //上拉输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(UARTx == UART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;                                                         //uart1_tx  pa2
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;                                                         //uart1_rx  pa3
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //上拉输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(UARTx == UART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                        //uart3_tx  pc10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;                                                        //uart3_rx  pc11
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //上拉输入
        GPIO_Init(GPIOC, &GPIO_InitStructure);

    }

    UART_InitStructure.UART_BaudRate =  115200;                                                          //115200;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_Init(UARTx, &UART_InitStructure);

    /*UART 模块使能*/
    UART_Cmd(UARTx, ENABLE);

    /*清除中断标志*/
    UART_ClearITPendingBit(UART1, 0xff);
    UART_ClearITPendingBit(UART2, 0xff);
}

/********************************************************************************************************
**函数信息 ：uart_send(UART_TypeDef* UARTx,char c)
**功能描述 ：串口发送字节
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输入参数 ：char c ,串口发送的字节
**输出参数 ：无
********************************************************************************************************/

void uart_send(UART_TypeDef* UARTx, char c)
{
    UART_SendData(UARTx, (uint16_t)c);

    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_TXIEN))

        {
            UART_ClearITPendingBit(UARTx, UART_IT_TXIEN);
            break;
        }

    }
}

/********************************************************************************************************
**函数信息 ：unsigned char inbyte(UART_TypeDef* UARTx)
**功能描述 ：串口接收函数
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：unsigned char 串口接收返回的字节
********************************************************************************************************/

unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;

    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN))
        {
            /*清除接收中断位*/
            UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);
            break;
        }
    }
    temp = (uint8_t)UART_ReceiveData(UARTx);
    return temp;
}


/********************************************************************************************************
**函数信息 ：void Uart1RxTest(UART_TypeDef* UARTx)
**功能描述 ：串口接收函数测试
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：无
********************************************************************************************************/

unsigned int Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    return temp;
}


/********************************************************************************************************
**函数信息 ：void outbyte(char c)
**功能描述 ：串口发送函数测试，通过电脑超级终端或者串口软件打印出来
**输入参数 ：char c 串口发送的字节
**输出参数 ：无
********************************************************************************************************/

void outbyte(char c)
{
    /* 如果调用UART2、UART3,更换该函数的第一个参数即可 */
    uart_send(UART1, c);
}



