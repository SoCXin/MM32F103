/**
******************************************************************************
* @file    usbapp.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the usbapp functions.
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
#include "sys.h"
#include "HAL_device.h"
#include "printf.h"
/********************************************************************************************************
**������Ϣ �� usb_gpio
**�������� �� USB IO��ʼ��
**������� ��
**������� ��
********************************************************************************************************/
void usb_gpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 					//ģ������
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   					//���ڴ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}
/********************************************************************************************************
**������Ϣ �� usb_test
**�������� �� USB�����ʼ��
**������� ��
**������� ��
********************************************************************************************************/
void usb_test(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE); 				//USB CLK EN

    uart_printf("This is a USB Demo \r\n");

    usb_gpio();
#ifdef USB_INTR_MODE
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;		//����USB�ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    usb_init();
    uart_printf("This is %x \r\n", *(u32*)0x40010804);
}




#define CLICK_NO		0
#define CLICK_LEFT		1
#define CLICK_RIGHT		2
#define CLICK_MID		3
/********************************************************************************************************
**������Ϣ �� mouse_ctrl
**�������� �� ��걨������
**������� �� char clickEnt(������¼�)
char x(x�����ƶ����λ��)
char y(y�����ƶ����λ��)
char rol(�����ƶ����λ��)
**������� ��
********************************************************************************************************/
void mouse_ctrl(char clickEnt, char x, char y, char rol)
{
    write_mreg32(USB_EPn_FIFO(EPIN), clickEnt);					//�����¼�
    write_mreg32(USB_EPn_FIFO(EPIN), x);							//x��������ƶ�λ�� (-128~127)
    write_mreg32(USB_EPn_FIFO(EPIN), y);							//y��������ƶ�λ�� (-128~127)
    write_mreg32(USB_EPn_FIFO(EPIN), rol);						//��������ƶ�λ�� (-128~127)
    write_mreg32(USB_EPn_CTRL(EPIN), 0x84);						//send 4 bytes packet
}


#define KEY_NONE 	0x00
#define KEY_UP    	0x1A
#define KEY_DOWN  	0x16
#define KEY_LEFT  	0x04
#define KEY_RIGHT 	0x07
#define KEY_A		0x18
#define KEY_B		0x0C
#define KEY_C		0x0D
#define KEY_D		0x0E

#define KEY_L_CTRL	0x01
#define KEY_L_SHIFT	0x02
#define KEY_L_ALT	0x04
#define KEY_L_WIN	0x08
#define KEY_R_CTRL	0x10
#define KEY_R_SHIFT	0x20
#define KEY_R_ALT	0x40
#define KEY_R_WIN	0x80
/********************************************************************************************************
**������Ϣ �� keyBoard_value
**�������� �� ���̱�������
**������� �� char spcKey(���ܼ�)
nomKeyx(���ð�����ֵ)
**������� ��
********************************************************************************************************/
void keyBoard_value(char spcKey, char nomKey0, char nomKey1, char nomKey2,
                    char nomKey3, char nomKey4, char nomKey5)
{
    write_mreg32(USB_EPn_FIFO(EPIN), spcKey);					//���ܼ�
    write_mreg32(USB_EPn_FIFO(EPIN), 0x00);						//����
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey0);					//���ð�����ֵ,����1Ϊ0x59 ������������
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey1);					//���ð�����ֵ
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey2);					//���ð�����ֵ
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey3);
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey4);
    write_mreg32(USB_EPn_FIFO(EPIN), nomKey5);
    write_mreg32(USB_EPn_CTRL(EPIN), 0x88);						//send 4 bytes packet
}
/********************************************************************************************************
**������Ϣ �� keyBoard_ctrl
**�������� �� ���̱������ݷ��ظ�����
**������� �� char spcKey(���ܼ�)
nomKeyx(���ð�����ֵ)
**������� ��
********************************************************************************************************/
void keyBoard_ctrl(char spcKey, char nomKey0, char nomKey1, char nomKey2,
                   char nomKey3, char nomKey4, char nomKey5)
{
    keyBoard_value(spcKey, nomKey0, nomKey1, nomKey2, nomKey3, nomKey4, nomKey5);	//��ֵд�뻺��Ĵ���
    while(read_mreg32(USB_EPn_CTRL(EPIN)) & 0x80);							//�ȴ��������
    keyBoard_value(KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE); //���ֵ
    while(read_mreg32(USB_EPn_CTRL(EPIN)) & 0x80);
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
