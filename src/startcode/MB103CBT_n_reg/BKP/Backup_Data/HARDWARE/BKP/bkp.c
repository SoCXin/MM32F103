#include "bkp.h"
#include "led.h"
#include "delay.h"


/********************************************************************************************************
**函数信息 ：CheckBackupReg(u16 FirstBackupData)
**功能描述 : 判断写数据是否正确
**输入参数 ：FirstBackupData
**输出参数 ：写错误：index+1；写成功：0
********************************************************************************************************/
u8 CheckBackupReg(void)
{
    if(BKP->DR1 == 0x55)
        return 0;
    else
        return 1;
}

/********************************************************************************************************
**函数信息 ：WriteToBackupReg()
**功能描述 : 往BKP数据寄存器里写数据
**输入参数 ：FirstBackupData
**输出参数 ：无
********************************************************************************************************/
void WriteToBackupReg(void)
{
    BKP->DR1 = 0x55;

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
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;
    /* Enable write access to Backup domain */
    PWR->CR |= 0x00000100;

    /* Clear Tamper pin Event(TE) pending flag */
    BKP->CSR |= 0x0001;

    WriteToBackupReg();//往BKP寄存器里写数据
    i = CheckBackupReg(); //判断数据写数据是否正确
    return i;
}

