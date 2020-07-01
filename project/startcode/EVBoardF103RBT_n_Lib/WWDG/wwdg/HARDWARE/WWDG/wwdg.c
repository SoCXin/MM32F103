/**
******************************************************************************
* @file    wwdg.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the wwdg functions.
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
#include "wwdg.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//���Ź� ��������



//����WWDG������������ֵ,Ĭ��Ϊ���.
u8 WWDG_CNT = 0x7f;

void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDGʱ��ʹ��

    WWDG_SetPrescaler(fprer);////����IWDGԤ��Ƶֵ

    WWDG_SetWindowValue(wr);//���ô���ֵ

    WWDG_Enable(tr);	 //ʹ�ܿ��Ź� ,	���� counter .

    WWDG_ClearFlag();

    WWDG_NVIC_Init();//��ʼ�����ڿ��Ź� NVIC

    WWDG_EnableIT(); //�������ڿ��Ź��ж�
}
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);
}
//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

void WWDG_IRQHandler(void)
{
    // Update WWDG counter
    WWDG_SetCounter(0x7F);	  //�������˾��,���ڿ��Ź���������λ
    // Clear EWI flag */
    WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ
    // Toggle GPIO_Led pin 7 */
    LED2 = !LED2;
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