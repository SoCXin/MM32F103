/**
******************************************************************************
* @file    uart_nvic.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the uart_nvic firmware functions.
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
#include "uart_nvic.h"




#if EN_UART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUARTx->SR�ܱ���Ī������Ĵ���
u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART_RX_STA = 0;     //����״̬���

void uart_nvic_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��UART1��GPIOAʱ��

    //UART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //UART ��ʼ������

    UART_InitStructure.UART_BaudRate = bound;//���ڲ�����
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//һ��ֹͣλ
    UART_InitStructure.UART_Parity = UART_Parity_No;//����żУ��λ
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//��Ӳ������������
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	//�շ�ģʽ

    UART_Init(UART1, &UART_InitStructure); //��ʼ������1
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);//�������ڽ����ж�
    UART_Cmd(UART1, ENABLE);                    //ʹ�ܴ���1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //UART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10


}

void UART1_IRQHandler(void)                	//����1�жϷ������
{
    u8 Res;
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        Res = UART_ReceiveData(UART1);	//��ȡ���յ�������
        if((Res == 0x0d) && ((UART_RX_STA & 0X3FFF) > 0))
        {
            UART_RX_STA |= 0x4000;
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        }
        else if((UART_RX_STA & 0x4000) && ((UART_RX_STA & 0X3FFF) > 0)) //���յ���0x0d
        {
            if(Res == 0x0a)
            {
                UART_RX_STA |= 0x8000;
            }
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        }
        else
        {
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
            UART_RX_STA = UART_RX_STA & 0X3FFF;
            if((UART_RX_STA) > (UART_REC_LEN - 1))
                UART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
        }

    }

}
#endif

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

