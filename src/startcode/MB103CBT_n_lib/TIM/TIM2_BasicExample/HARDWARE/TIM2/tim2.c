/**
******************************************************************************
* @file    tim2.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the tim2 firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "tim2.h"
/********************************************************************************************************
**������Ϣ ��void TIM2_IRQHandler(void)
**�������� ����ʱ��2�жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    /*�����ֶ�����жϱ�־λ*/
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /*ȫ�ֱ�־*/
    ucTim2Flag++;


}

/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test1(u16 Period,u16 Prescaler)
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ
Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    NVIC_InitTypeDef NVIC_StructInit;

    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK1(PCLK1δ��Ƶʱ����Ƶ,������PCLK1��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    TIM_StructInit.TIM_Period = Period;                                                                    //ARR�Ĵ���ֵ
    TIM_StructInit.TIM_Prescaler = Prescaler;                                                              //Ԥ��Ƶֵ
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                                                       //������Ƶֵ
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                                                   //����ģʽ
    TIM_StructInit.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_StructInit);

    /*����ʱ��2�����ж�*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);

    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* ���ö�ʱ��2�ж�ͨ�������ȼ� */
    NVIC_StructInit.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_StructInit.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_StructInit.NVIC_IRQChannelSubPriority = 1;

    NVIC_Init(&NVIC_StructInit);


}
/********************************************************************************************************
**������Ϣ ��Tim2_UPStatusOVCheck_test1(void)
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim2_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
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
