/**
******************************************************************************
* @file    exti.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the exti functions.
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
#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "uart.h"

//////////////////////////////////////////////////////////////////////////////////
//开发板
//外部中断 驱动代码
//////////////////////////////////////////////////////////////////////////////////


//外部中断初始化函数
void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //外部中断，需要使能AFIO时钟

    KEY_Init();//初始化按键对应io模式

    //GPIOC.10 中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);

    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15	  中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.0	  中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器



    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

}


void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //消抖
    if(WK_UP == 1)
    {
        LED2 = !LED2;
        LED1 = !LED1;
    }
    EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
void EXTI15_10_IRQHandler(void)
{
    delay_ms(10);   //消抖
    if(KEY3 == 0)
    {
        LED1 = !LED1;
    }
    else if(KEY4 == 0)
    {
        LED2 = !LED2;
    }
    EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line15);  //清除LINE10,15上的中断标志位
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

