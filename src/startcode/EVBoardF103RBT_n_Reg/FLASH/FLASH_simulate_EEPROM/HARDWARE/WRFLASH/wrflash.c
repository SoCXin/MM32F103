#include "wrflash.h"
#include "delay.h"
#include "uart.h"

void ONCHIP_FLASH_Unlock(void)
{
    FLASH->KEYR = FLASH_KEY1; //д���������.
    FLASH->KEYR = FLASH_KEY2;
}
//flash����
void ONCHIP_FLASH_Lock(void)
{
    FLASH->CR |= 1 << 7; //����
}
//�õ�FLASH״̬
u8 ONCHIP_FLASH_GetStatus(void)
{
    u32 res;
    res = FLASH->SR;
    if(res & (1 << 0))return 1;		 //æ
    else if(res & (1 << 2))return 2;	//��̴���
    else if(res & (1 << 4))return 3;	//д��������
    return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 ONCHIP_FLASH_WaitDone(u16 time)
{
    u8 res;
    do
    {
        res = ONCHIP_FLASH_GetStatus();
        if(res != 1)break; //��æ,����ȴ���,ֱ���˳�.
        delay_us(1);
        time--;
    }
    while(time);
    if(time == 0)res = 0xff; //TIMEOUT
    return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 ONCHIP_FLASH_ErasePage(u32 paddr)
{
    u8 res = 0;
    res = ONCHIP_FLASH_WaitDone(0X5FFF); //�ȴ��ϴβ�������,>20ms
    if(res == 0)
    {
        FLASH->CR |= 1 << 1; //ҳ����
        FLASH->AR = paddr; //����ҳ��ַ
        FLASH->CR |= 1 << 6; //��ʼ����
        res = ONCHIP_FLASH_WaitDone(0X5FFF); //�ȴ���������,>20ms
        if(res != 1) //��æ
        {
            FLASH->CR &= ~(1 << 1); //���ҳ������־.
        }
    }
    return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 ONCHIP_FLASH_WriteHalfWord(u32 faddr, u16 dat)
{
    u8 res;
    res = ONCHIP_FLASH_WaitDone(0XFF);
    if(res == 0) //OK
    {
        FLASH->CR |= 1 << 0; //���ʹ��
        *(vu16*)faddr = dat; //д������
        res = ONCHIP_FLASH_WaitDone(0XFF); //�ȴ��������
        if(res != 1) //�����ɹ�
        {
            FLASH->CR &= ~(1 << 0); //���PGλ.
        }
    }
    return res;
}
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ
//����ֵ:��Ӧ����.
u16 ONCHIP_FLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr;
}
#if ONCHIP_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void ONCHIP_FLASH_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u16 i;
    for(i = 0; i < NumToWrite; i++)
    {
        ONCHIP_FLASH_WriteHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2; //��ַ����2.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if ONCHIP_FLASH_SIZE<256
#define ONCHIP_SECTOR_SIZE 1024 //�ֽ�
#else
#define ONCHIP_SECTOR_SIZE	2048
#endif
u16 ONCHIP_FLASH_BUF[ONCHIP_SECTOR_SIZE / 2]; //�����2K�ֽ�



void ONCHIP_FLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u32 secpos;	   //������ַ
    u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 secremain; //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
    if(WriteAddr < ONCHIP_FLASH_BASE || (WriteAddr >= (ONCHIP_FLASH_BASE + 1024 * ONCHIP_FLASH_SIZE)))return; //�Ƿ���ַ
    ONCHIP_FLASH_Unlock();						//����
    offaddr = WriteAddr - ONCHIP_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / ONCHIP_SECTOR_SIZE;			//������ַ  0~127 for ONCHIP_32F103RBT6
    secoff = (offaddr % ONCHIP_SECTOR_SIZE) / 2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = ONCHIP_SECTOR_SIZE / 2 - secoff;		//����ʣ��ռ��С
    if(NumToWrite <= secremain)secremain = NumToWrite; //�����ڸ�������Χ
    while(1)
    {
        ONCHIP_FLASH_Read(secpos * ONCHIP_SECTOR_SIZE + ONCHIP_FLASH_BASE, ONCHIP_FLASH_BUF, ONCHIP_SECTOR_SIZE / 2); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(ONCHIP_FLASH_BUF[secoff + i] != 0XFFFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            ONCHIP_FLASH_ErasePage(secpos * ONCHIP_SECTOR_SIZE + ONCHIP_FLASH_BASE); //�����������
            for(i = 0; i < secremain; i++) //����
            {
                ONCHIP_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            ONCHIP_FLASH_Write_NoCheck(secpos * ONCHIP_SECTOR_SIZE + ONCHIP_FLASH_BASE, ONCHIP_FLASH_BUF, ONCHIP_SECTOR_SIZE / 2); //д����������
        }
        else ONCHIP_FLASH_Write_NoCheck(WriteAddr, pBuffer, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;				//������ַ��1
            secoff = 0;				//ƫ��λ��Ϊ0
            pBuffer += secremain;  	//ָ��ƫ��
            WriteAddr += secremain * 2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)
            NumToWrite -= secremain;	//�ֽ�(16λ)���ݼ�
            if(NumToWrite > (ONCHIP_SECTOR_SIZE / 2))secremain = ONCHIP_SECTOR_SIZE / 2; //��һ����������д����
            else secremain = NumToWrite; //��һ����������д����
        }
    };
    ONCHIP_FLASH_Lock();//����
}
#endif
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void ONCHIP_FLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead)
{
    u16 i;
    for(i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = ONCHIP_FLASH_ReadHalfWord(ReadAddr); //��ȡ2���ֽ�.
        ReadAddr += 2; //ƫ��2���ֽ�.
    }
}

//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr, u16 WriteData)
{
    ONCHIP_FLASH_Write(WriteAddr, &WriteData, 1); //д��һ����
}
















