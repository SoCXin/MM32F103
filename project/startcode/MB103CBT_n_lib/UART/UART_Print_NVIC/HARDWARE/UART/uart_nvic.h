/**
******************************************************************************
* @file    uart_nvic.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the uart_nvic inc file for the library.
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
#ifndef __UART_NVIC_H
#define __UART_NVIC_H
#include "stdio.h"
#include "sys.h"
//������
//����1��ʼ��
#define UART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  UART_RX_BUF[UART_REC_LEN]; //���ջ���,���UART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 UART_RX_STA;         		//����״̬���
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_nvic_init(u32 bound);
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

