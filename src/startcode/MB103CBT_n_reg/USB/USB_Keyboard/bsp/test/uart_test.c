/**
  ******************************************************************************
  * @file    uart_test.c
  * @brief   ���ô��ڹ���
  *
  ******************************************************************************
  */
#include "uart_test.h"
/********************************************************************************************************
**������Ϣ ��UartInit(UART_TypeDef* UARTx)
**�������� ����ʼ������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                     // ���⸴�����
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                         //uart1_rx  pa10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                       //��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(UARTx == UART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;                                                         //uart1_tx  pa2
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // ���⸴�����
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;                                                         //uart1_rx  pa3
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(UARTx == UART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                        //uart3_tx  pc10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // ���⸴�����
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;                                                        //uart3_rx  pc11
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //��������
        GPIO_Init(GPIOC, &GPIO_InitStructure);

    }

    UART_InitStructure.UART_BaudRate =  115200;                                                          //115200;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_Init(UARTx, &UART_InitStructure);

    /*UART ģ��ʹ��*/
    UART_Cmd(UARTx, ENABLE);

    /*����жϱ�־*/
    UART_ClearITPendingBit(UART1, 0xff);
    UART_ClearITPendingBit(UART2, 0xff);
}

/********************************************************************************************************
**������Ϣ ��uart_send(UART_TypeDef* UARTx,char c)
**�������� �����ڷ����ֽ�
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��char c ,���ڷ��͵��ֽ�
**������� ����
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
**������Ϣ ��unsigned char inbyte(UART_TypeDef* UARTx)
**�������� �����ڽ��պ���
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��unsigned char ���ڽ��շ��ص��ֽ�
********************************************************************************************************/

unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;

    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN))
        {
            /*��������ж�λ*/
            UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);
            break;
        }
    }
    temp = (uint8_t)UART_ReceiveData(UARTx);
    return temp;
}


/********************************************************************************************************
**������Ϣ ��void Uart1RxTest(UART_TypeDef* UARTx)
**�������� �����ڽ��պ�������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
********************************************************************************************************/

unsigned int Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    return temp;
}


/********************************************************************************************************
**������Ϣ ��void outbyte(char c)
**�������� �����ڷ��ͺ������ԣ�ͨ�����Գ����ն˻��ߴ��������ӡ����
**������� ��char c ���ڷ��͵��ֽ�
**������� ����
********************************************************************************************************/

void outbyte(char c)
{
    /* �������UART2��UART3,�����ú����ĵ�һ���������� */
    uart_send(UART1, c);
}



