/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
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
#include "delay.h"
#include "uart.h"
#include "led.h"
void EXTI_Configuration(void);
void NVIC_Configuration(void);

/********************************************************************************************************
**函数信息 ：PVD_IRQHandler(void)
**功能描述 ：电压超过或低于设置的阈值电压后，进入该中断，翻转LED
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED翻转 */
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：改变VCC电压值，判断LED亮灭情况
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{

    delay_init();	    	 //延时函数初始化

    LED_Init();   //LED初始化

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Configure EXTI Line to generate an interrupt on falling edge */
    EXTI_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the PVD Level to 3.0V */
    PWR_PVDLevelConfig(PWR_PVDLevel_3V0);

    /* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);

    while(1)              //无限循环
    {

    }
}

/********************************************************************************************************
**函数信息 ：EXTI_Configuration(void)
**功能描述 ：PVD中断配置初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
    falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;// PVD连接到中断线16上
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//使用中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//电压上升或下降越过设定阀值时都产生中断。
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;// 使能中断线
    EXTI_Init(&EXTI_InitStructure);// 初始
}

/********************************************************************************************************
**函数信息 ：NVIC_Configuration(void)
**功能描述 ：PVD中断优先级初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断优先级配置

    /* Enable the PVD Interrupt */ //设置PVD中断
    NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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
