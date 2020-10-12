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
**������Ϣ ��PVD_IRQHandler(void)
**�������� ����ѹ������������õ���ֵ��ѹ�󣬽�����жϣ���תLED
**������� ����
**������� ����
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED��ת */
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ���ı�VCC��ѹֵ���ж�LED�������
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{

    delay_init();	    	 //��ʱ������ʼ��

    LED_Init();   //LED��ʼ��

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Configure EXTI Line to generate an interrupt on falling edge */
    EXTI_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the PVD Level to 3.0V */
    PWR_PVDLevelConfig(PWR_PVDLevel_3V0);

    /* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);

    while(1)              //����ѭ��
    {

    }
}

/********************************************************************************************************
**������Ϣ ��EXTI_Configuration(void)
**�������� ��PVD�ж����ó�ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
    falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;// PVD���ӵ��ж���16��
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ʹ���ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//��ѹ�������½�Խ���趨��ֵʱ�������жϡ�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;// ʹ���ж���
    EXTI_Init(&EXTI_InitStructure);// ��ʼ
}

/********************************************************************************************************
**������Ϣ ��NVIC_Configuration(void)
**�������� ��PVD�ж����ȼ���ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж����ȼ�����

    /* Enable the PVD Interrupt */ //����PVD�ж�
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
