/**
******************************************************************************
* @file    bkp.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the bkp firmware functions.
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
#include "bkp.h"
#include "led.h"
#include "delay.h"

u16 BKPDataReg[10] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
};
/**
* @brief  This function handles Tamper interrupt request.
* @param  None
* @retval None
*/
//
/********************************************************************************************************
**������Ϣ ��IsBackupRegReset(void)
**�������� ���ж�BKP���ݼĴ������Ƿ������ݣ������ж�������
**������� ����
**������� ����
********************************************************************************************************/
uint32_t IsBackupRegReset(void)
{
    uint32_t index = 0;

    for (index = 0; index < 10; index++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != 0x0000)
        {
            return (index + 1);
        }
    }

    return 0;
}

/********************************************************************************************************
**������Ϣ ��TAMPER_IRQHandler(void)
**�������� ���ж�PC13�Ƿ���������⣬PC13����һ��������ƽ�ı䣬�ͻ�����ж�
**������� ����
**������� ����
********************************************************************************************************/
void TAMPER_IRQHandler(void)
{
    if(BKP_GetITStatus() != RESET)
    {
        /* Tamper detection event occurred */

        /* Check if Backup registers are cleared */
        if(IsBackupRegReset() == 0)//�ж�BKP�����Ƿ��Ѿ���������LED3����������˵�������Ѳ�����
        {
            /* OK, Backup registers are reset as expected */

            /* Turn on LED3 */
            LED3 = 0;
        }
        else
        {
            /* Backup registers are not reset */
            /* Turn on LED4 */
            LED4 = 0;
        }

        /* Clear Tamper pin interrupt pending bit */
        BKP_ClearITPendingBit();

        /* Clear Tamper pin Event(TE) pending flag */
        BKP_ClearFlag();

        /* Disable Tamper pin */
        BKP_TamperPinCmd(DISABLE);

        /* Enable Tamper pin */
        BKP_TamperPinCmd(ENABLE);

    }
}
//�ж�д�����Ƿ���ȷ
/********************************************************************************************************
**������Ϣ ��TAMPER_IRQHandler(void)
**�������� ���ж�PC13�Ƿ���������⣬PC13����һ��������ƽ�ı䣬�ͻ�����ж�
**������� ����
**������� ����
********************************************************************************************************/
u8 CheckBackupReg(u16 FirstBackupData)
{
    u32 index = 0;
    index = BKP->DR1;
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
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData + (index * 0x5A));
    }
}
/********************************************************************************************************
**������Ϣ ��NVIC_Configuration(void)
**�������� : �������жϳ�ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable TAMPER IRQChannel */
    NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************************************************
**������Ϣ �BKP_DATA(void)
**�������� : ���������⣬����BKP���ݼĴ���д���ݣ����ж��Ƿ���ȷ
**������� ����
**������� ����
********************************************************************************************************/
void BKP_DATA(void)
{
    NVIC_Configuration();

    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);
    /* Disable Tamper pin */
    BKP_TamperPinCmd(DISABLE);

    /* Disable Tamper interrupt */
    BKP_ITConfig(DISABLE);

    /* Tamper pin active on low level */
    BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);

    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

    /* Enable Tamper interrupt */
    BKP_ITConfig(ENABLE);

    /* Enable Tamper pin */
    BKP_TamperPinCmd(ENABLE);

    /* Write data to Backup DRx registers */
    WriteToBackupReg(0xA53C);

    /* Check if the written data are correct */
    if(CheckBackupReg(0xA53C) == 0x00)
    {
        /* Turn on LED1 */
        LED1 = 0; //���ݶ�д��ȷ
    }
    else
    {
        /* Turn on LED2 */
        LED2 = 0; //���ݶ�д����
    }

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
