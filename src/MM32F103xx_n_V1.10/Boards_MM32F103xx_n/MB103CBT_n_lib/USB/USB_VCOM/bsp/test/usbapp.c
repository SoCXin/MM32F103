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
**函数信息 ： USB_GPIO
**功能描述 ： USB IO初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void USB_GPIO(void)
{
    RCC->APB2ENR |= RCC_APB2RSTR_IOPARST;   //RCC->APB2ENR|=1<<2;  //使能GPIOA时钟
    GPIOA->CRH &= 0XFFF00FFF;				  //将PA12&PA13配置成模拟输入
    GPIOA->CRL |= GPIO_CRL_MODE5;			  //PA5用于测试
    GPIOA->BRR = 0x0020;
}
/********************************************************************************************************
**函数信息 ： usb_test
**功能描述 ： USB外设初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void usb_test(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;					  //使能USB时钟
    printf("This is a USB Demo \r\n");
    USB_GPIO();
    MY_NVIC_Init(1, 1, USB_HP_CAN1_TX_IRQn, 2);			 //配置USB中断
    USB_Init();
    printf("This is %x \r\n", *(u32*)0x40010804);		   //用于测试
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
