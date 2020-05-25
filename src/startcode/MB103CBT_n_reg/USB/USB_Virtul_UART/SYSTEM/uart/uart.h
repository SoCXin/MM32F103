#ifndef __UART_H
#define __UART_H
#include "sys.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

#define UART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


/** @defgroup UART_Interrupt_definition
  * @{
  */


#define UART_IT_ERR            ((uint16_t)0x0020)
#define UART_IT_PE             ((uint16_t)0x0010)
#define UART_OVER_ERR          ((uint16_t)0x0008)
#define UART_TIMEOUT_ERR       ((uint16_t)0x0004)
#define UART_IT_RXIEN          ((uint16_t)0x0002)
#define UART_IT_TXIEN          ((uint16_t)0x0001)




/** @defgroup UART_Flags
  * @{
  */


#define UART_FLAG_TXEMPTY                   ((uint16_t)0x0008)
#define UART_FLAG_TXFULL                    ((uint16_t)0x0004)
#define UART_FLAG_RXAVL                     ((uint16_t)0x0002)
#define UART_FLAG_TXEPT                     ((uint16_t)0x0001)


extern u8  UART_RX_BUF[UART_REC_LEN]; //���ջ���,���UART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 UART_RX_STA;         		//����״̬���
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 pclk2, u32 bound);
void uart_sendArray(unsigned char *dataBuf, unsigned int len);

#endif
















