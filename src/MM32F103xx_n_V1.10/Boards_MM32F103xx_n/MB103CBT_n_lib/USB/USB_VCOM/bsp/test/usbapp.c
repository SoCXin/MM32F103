/**
******************************************************************************
* @file    usbapp.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the usbapp firmware functions.
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
#include "usb.h"
#include "HAL_device.h"
#include "sys.h"
#include "stdio.h"
/********************************************************************************************************
**������Ϣ �� USB_GPIO
**�������� �� USB IO��ʼ��
**������� ��
**������� ��
********************************************************************************************************/
void USB_GPIO(void)
{
    RCC->APB2ENR |= RCC_APB2RSTR_IOPARST;   //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    GPIOA->CRH &= 0XFFF00FFF;				  //��PA12&PA13���ó�ģ������
    GPIOA->CRL |= GPIO_CRL_MODE5;			  //PA5���ڲ���
    GPIOA->BRR = 0x0020;
}
/********************************************************************************************************
**������Ϣ �� usb_test
**�������� �� USB�����ʼ��
**������� ��
**������� ��
********************************************************************************************************/
void usb_test(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;					  //ʹ��USBʱ��
    printf("This is a USB Demo \r\n");
    USB_GPIO();
    MY_NVIC_Init(1, 1, USB_HP_CAN1_TX_IRQn, 2);			 //����USB�ж�
    USB_Init();
    printf("This is %x \r\n", *(u32*)0x40010804);		   //���ڲ���
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
