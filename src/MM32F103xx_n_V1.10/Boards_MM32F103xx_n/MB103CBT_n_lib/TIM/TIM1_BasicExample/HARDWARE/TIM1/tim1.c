/**
******************************************************************************
* @file    Tim1.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the Tim1 firmware functions.
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
#include "tim1.h"

/********************************************************************************************************
**������Ϣ ��void Tim1_UPCount_test1(u16 Period,u16 Prescaler)
**�������� �����ö�ʱ��1���ϼ���ģʽ
**������� ��Period 16λ����������ֵ
Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim1_UPCount_test(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;

    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK2(PCLK2δ��Ƶʱ����Ƶ,������PCLK2��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);


    TIM_StructInit.TIM_Period = Period;                                                                    //ARR�Ĵ���ֵ
    TIM_StructInit.TIM_Prescaler = Prescaler;                                                              //Ԥ��Ƶֵ
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                                                       //������Ƶֵ
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                                                   //����ģʽ
    TIM_StructInit.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_StructInit);

    TIM_Cmd(TIM1, ENABLE);

    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}
/********************************************************************************************************
**������Ϣ ��Tim1_UPStatusOVCheck_test1(void)
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim1_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) == RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
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
