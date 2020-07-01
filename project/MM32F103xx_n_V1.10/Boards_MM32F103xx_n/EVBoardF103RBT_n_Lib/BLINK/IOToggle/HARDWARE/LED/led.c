/**
******************************************************************************
* @file    led.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the led firmware functions.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
******************************************************************************
*/
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////
//������
//LED��������
//////////////////////////////////////////////////////////////////////////////////
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOA,GPIOC�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LD2-->PA.8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
    GPIO_SetBits(GPIOA, GPIO_Pin_8);						 //PA,8 �����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LD1-->PC.13 �˿�����, �������
    GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
    GPIO_SetBits(GPIOC, GPIO_Pin_13); 						 //PA.15 �����

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

