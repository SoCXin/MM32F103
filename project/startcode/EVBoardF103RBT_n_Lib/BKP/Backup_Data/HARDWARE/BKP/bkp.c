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
**函数信息 ：CheckBackupReg(u16 FirstBackupData)
**功能描述 : 判断写数据是否正确
**输入参数 ：FirstBackupData
**输出参数 ：写错误：index+1；写成功：0
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
**函数信息 ：WriteToBackupReg(u16 FirstBackupData)
**功能描述 : 往BKP数据寄存器里写数据
**输入参数 ：FirstBackupData
**输出参数 ：无
********************************************************************************************************/
void WriteToBackupReg(u16 FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < 10; index++)
    {
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData + (index * 0x5A));//往对应寄存器里写数据
    }
}

/********************************************************************************************************
**函数信息 ：BKP_DATA(void)
**功能描述 : BKP数据读写测试，判断写和读的数据是否一致
**输入参数 ：FirstBackupData
**输出参数 ：i
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

    WriteToBackupReg(0x3210);//往BKP寄存器里写数据
    i = CheckBackupReg(0x3210); //判断数据写数据是否正确
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