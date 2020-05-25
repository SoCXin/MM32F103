/**
******************************************************************************
* @file    iwdg.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the iwdg firmware functions.
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
#include "iwdg.h"
/********************************************************************************************************
**������Ϣ ��PVU_CheckStatus(void)
**�������� �����������Ź�Ԥ��Ƶλ״̬
**������� ����
**������� ����
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*���Ԥ��Ƶλ״̬,ΪRESET�ſɸı�Ԥ��Ƶֵ*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET)
        {
            break;
        }
    }
}
/********************************************************************************************************
**������Ϣ ��RVU_CheckStatus(void)
**�������� �����������Ź����ر�־
**������� ����
**������� ����
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*������ر�־״̬*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET)
        {
            break;
        }
    }
}
/********************************************************************************************************
**������Ϣ ��Write_Iwdg_PR(void)
**�������� �������������Ź�
**������� ��IWDG_Prescaler ��ѡIWDG_Prescaler_X, XΪ4,8,16,32,64,128,256,��Ӧ��Ƶֵ��Xȡֵ��ͬ
Reload<=0xfff,Ϊ����������ֵ
**������� ����
**    ��ע ����λʱ��������LSI 40KHzΪ�ο�
Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload)
{
    /*�����ڲ�����ʱ��,�ȴ�ʱ�Ӿ���*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /*����ʱ��Ԥ��Ƶ*/
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);

    /*�������ؼĴ���ֵ*/
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    /*װ�ز�ʹ�ܼ�����*/
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**������Ϣ ��void Write_Iwdg_RL(void)
**�������� ��ι������
**������� ��
**������� ����
********************************************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
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
