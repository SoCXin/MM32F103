#ifndef __UART_NVIC_H
#define __UART_NVIC_H
#include "stdio.h"
#include "sys.h"

#define UART_REC_LEN  			200  	//�����������ֽ��� 200

extern u8  UART_RX_BUF[UART_REC_LEN]; //���ջ���,���UART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 UART_RX_STA;         		//����״̬���
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_nvic_init(u32 bound);

#endif

















