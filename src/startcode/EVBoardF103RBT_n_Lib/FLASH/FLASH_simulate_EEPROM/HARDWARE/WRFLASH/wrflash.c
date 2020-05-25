/**
******************************************************************************
* @file    wrflash.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the wrflash functions.
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
#include "wrflash.h"
#include "delay.h"
#include "uart.h"

//////////////////////////////////////////////////////////////////////////////////
//103������
//FLASH ��������
//////////////////////////////////////////////////////////////////////////////////

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
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
        FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2; //��ַ����2.
    }
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if ONCHIP_FLASH_SIZE<256
#define ONCHIP_FLASH_SECTOR_SIZE 1024 //�ֽ�
#else
#define ONCHIP_FLASH_SECTOR_SIZE	2048
#endif
u16 ONCHIP_FLASH_BUF[ONCHIP_FLASH_SECTOR_SIZE / 2]; //�����2K�ֽ�
void ONCHIP_FLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u32 secpos;	   //������ַ
    u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
    u16 secremain; //������ʣ���ַ(16λ�ּ���)
    u16 i;
    u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
    if(WriteAddr < ONCHIP_FLASH_BASE || (WriteAddr >= (ONCHIP_FLASH_BASE + 1024 * ONCHIP_FLASH_SIZE)))return; //�Ƿ���ַ
    FLASH_Unlock();						//����
    offaddr = WriteAddr - ONCHIP_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / ONCHIP_FLASH_SECTOR_SIZE;			//������ַ  0~127 for 103RBT6
    secoff = (offaddr % ONCHIP_FLASH_SECTOR_SIZE) / 2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = ONCHIP_FLASH_SECTOR_SIZE / 2 - secoff;		//����ʣ��ռ��С
    if(NumToWrite <= secremain)secremain = NumToWrite; //�����ڸ�������Χ
    while(1)
    {
        ONCHIP_FLASH_Read(secpos * ONCHIP_FLASH_SECTOR_SIZE + ONCHIP_FLASH_BASE, ONCHIP_FLASH_BUF, ONCHIP_FLASH_SECTOR_SIZE / 2); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(ONCHIP_FLASH_BUF[secoff + i] != 0XFFFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            FLASH_ErasePage(secpos * ONCHIP_FLASH_SECTOR_SIZE + ONCHIP_FLASH_BASE); //�����������
            for(i = 0; i < secremain; i++) //����
            {
                ONCHIP_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            ONCHIP_FLASH_Write_NoCheck(secpos * ONCHIP_FLASH_SECTOR_SIZE + ONCHIP_FLASH_BASE, ONCHIP_FLASH_BUF, ONCHIP_FLASH_SECTOR_SIZE / 2); //д����������
        }
        else ONCHIP_FLASH_Write_NoCheck(WriteAddr, pBuffer, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;				//������ַ��1
            secoff = 0;				//ƫ��λ��Ϊ0
            pBuffer += secremain;  	//ָ��ƫ��
            WriteAddr = secpos * ONCHIP_FLASH_SECTOR_SIZE + ONCHIP_FLASH_BASE; //WriteAddr+=secremain;	//д��ַƫ��
            NumToWrite -= secremain;	//�ֽ�(16λ)���ݼ�
            if(NumToWrite > (ONCHIP_FLASH_SECTOR_SIZE / 2))secremain = ONCHIP_FLASH_SECTOR_SIZE / 2; //��һ����������д����
            else secremain = NumToWrite; //��һ����������д����
        }
    };
    FLASH_Lock();//����
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr, u16 WriteData)
{
    ONCHIP_FLASH_Write(WriteAddr, &WriteData, 1); //д��һ����
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














