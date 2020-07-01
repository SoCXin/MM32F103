/**
******************************************************************************
* @file    bkp.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the bkp firmware functions.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
******************************************************************************
*/
#include "bkp.h"
#include "led.h"
#include "delay.h"
u16 BKPDataReg[10] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
};


/********************************************************************************************************
**������Ϣ ��CheckBackupReg(u16 FirstBackupData)
**�������� : �ж�д�����Ƿ���ȷ
**������� ��FirstBackupData
**������� ��д����index+1��д�ɹ���0
********************************************************************************************************/
u8 CheckBackupReg(u16 FirstBackupData)
{
    u32 index = 0;

    for (index = 0; index < 10; index++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != (FirstBackupData + (index * 0x5A)))
        {
            return (index + 1);
        }
    }

    return 0;
}

/********************************************************************************************************
**������Ϣ ��WriteToBackupReg(u16 FirstBackupData)
**�������� : ��BKP���ݼĴ�����д����
**������� ��FirstBackupData
**������� ����
********************************************************************************************************/
void WriteToBackupReg(u16 FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < 10; index++)
    {
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData + (index * 0x5A));//����Ӧ�Ĵ�����д����
    }
}

/********************************************************************************************************
**������Ϣ ��BKP_DATA(void)
**�������� : BKP���ݶ�д���ԣ��ж�д�Ͷ��������Ƿ�һ��
**������� ��FirstBackupData
**������� ��i
********************************************************************************************************/
u8 BKP_DATA(void)
{
    u8 i;
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

    WriteToBackupReg(0x3210);//��BKP�Ĵ�����д����
    i = CheckBackupReg(0x3210); //�ж�����д�����Ƿ���ȷ
    return i;
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