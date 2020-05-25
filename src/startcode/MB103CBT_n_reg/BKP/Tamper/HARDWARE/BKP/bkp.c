#include "bkp.h"
#include "led.h"
#include "delay.h"



/********************************************************************************************************
**������Ϣ ��TAMPER_IRQHandler(void)
**�������� ���ж�PC13�Ƿ���������⣬PC13����һ��������ƽ�ı䣬�ͻ�����ж�
**������� ����
**������� ����
********************************************************************************************************/
void TAMPER_IRQHandler(void)
{
    if((BKP->CSR & 0x200) != 0)
    {
        /* Check if Backup registers are cleared */
        if(CheckBackupReg() != 0) //�ж�BKP�����Ƿ��Ѿ���������LED3����������˵�������Ѳ�����
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
**������Ϣ ��CheckBackupReg(u16 FirstBackupData)
**�������� : �ж�д�����Ƿ���ȷ
**������� ��FirstBackupData
**������� ��д����index+1��д�ɹ���0
********************************************************************************************************/
u8 CheckBackupReg(void)
{
    if(BKP->DR1 == 0x55)
        return 0;
    else
        return 1;
}

/********************************************************************************************************
**������Ϣ ��WriteToBackupReg()
**�������� : ��BKP���ݼĴ�����д����
**������� ��FirstBackupData
**������� ����
********************************************************************************************************/
void WriteToBackupReg(void)
{
    BKP->DR1 = 0x55;
}
/********************************************************************************************************
**������Ϣ ��BKP_DATA(void)
**�������� : BKP���ݶ�д���ԣ��ж�д�Ͷ��������Ƿ�һ��
**������� ��FirstBackupData
**������� ��i
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
        LED1 = 0; //���ݶ�д��ȷ
    }
    else
    {
        /* Turn on LED2 */
        LED2 = 0; //���ݶ�д����
    }
}





