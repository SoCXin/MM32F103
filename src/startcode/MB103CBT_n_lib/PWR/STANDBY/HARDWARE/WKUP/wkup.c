/**
******************************************************************************
* @file    wkup.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the wkup firmware functions.
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
#include "wkup.h"
#include "led.h"
#include "delay.h"
/********************************************************************************************************
**������Ϣ ��Sys_Standby(void)
**�������� ���������ģʽ
**������� ����
**������� ����
********************************************************************************************************/
void Sys_Standby(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
    RCC->APB2RSTR |= 0X01FC; //��λ����IO��
    PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
    PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ

}

/********************************************************************************************************
**������Ϣ ��Check_WKUP(void)
**�������� ���жϰ����Ƿ���
**������� ����
**������� ����
********************************************************************************************************/
u8 Check_WKUP(void)
{
    u8 t = 0;	//��¼���µ�ʱ��
    LED1 = 0; //����DS0
    LED2 = 0; //����DS0
    LED3 = 0; //����DS0
    LED4 = 0;
    while(1)
    {
        if(WKUP_KD)
        {
            t++;			//�Ѿ�������
            delay_ms(30);
            if(t >= 100)		//���³���3����
            {
                LED1 = 0;	 	//����DS0
                LED2 = 0;	 	//����DS0
                LED3 = 0;	 	//����DS0
                LED4 = 0;
                return 1; 	//����3s������
            }
        }
        else
        {
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;
            return 0; //���²���3��
        }
    }
}
/********************************************************************************************************
**������Ϣ ��EXTI0_IRQHandler(void)
**�������� ���ⲿ�ж�0�����������жϱ�־λ
**������� ����
**������� ����
********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ
}


/********************************************************************************************************
**������Ϣ ��WKUP_Init(void)
**�������� ���ⲿ�жϻ��Ѵ�����ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void WKUP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA.0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
    //ʹ���ⲿ�жϷ�ʽ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    if(Check_WKUP() == 0) Sys_Standby();  //���ǿ���,�������ģʽ
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













