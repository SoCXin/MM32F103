#include "bkp.h"
#include "led.h"
#include "delay.h"



/********************************************************************************************************
**函数信息 ：TAMPER_IRQHandler(void)
**功能描述 ：判断PC13是否产生侵入检测，PC13引脚一旦发生电平改变，就会进入中断
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void TAMPER_IRQHandler(void)
{
    if((BKP->CSR & 0x200) != 0)
    {
        /* Check if Backup registers are cleared */
        if(CheckBackupReg() != 0) //判断BKP数据是否已经擦除（若LED3被点亮，则说明数据已擦除）
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
        /* Clear Tamper pin Event(TE) pending flag */
        BKP->CSR |= 0x0001;

        /* Clear Tamper pin interrupt pending bit */
        BKP->CSR |= 0x0002;

        /* Disable Tamper pin */
        BKP->CR &= ~(0x1);

        /* Enable Tamper pin */
        BKP->CR |= 0x1;
    }
}
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
void BKP_DATA(void)
{
    u8 i;
    MY_NVIC_Init(1, 1, TAMPER_IRQn, 0);
    /* Enable PWR and BKP clock */
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;
    /* Enable write access to Backup domain */
    PWR->CR |= 0x00000100;
    /* Disable Tamper pin */
    BKP->CR &= ~(0x1);

    /* Disable Tamper interrupt */
    BKP->CSR &= ~(0X2);

    /* Tamper pin active on low level */
    BKP->CR |= 1 << 1;

    /* Clear Tamper pin Event(TE) pending flag */
    BKP->CSR |= 0x0001;

    /* Enable Tamper interrupt */
    BKP->CSR |= 0X4;

    /* Enable Tamper pin */
    BKP->CR |= 0x1;

    WriteToBackupReg();

    /* Check if the written data are correct */
    i = CheckBackupReg();

    if(i == 0)
    {
        LED1 = 0; //数据读写正确
    }
    else
    {
        /* Turn on LED2 */
        LED2 = 0; //数据读写错误
    }
}





