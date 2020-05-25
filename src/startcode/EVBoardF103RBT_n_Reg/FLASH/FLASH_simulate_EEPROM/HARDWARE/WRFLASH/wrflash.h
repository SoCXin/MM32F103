#ifndef _WRFLASH_H_
#define _WRLASH_H_
#include "sys.h"

//�û������Լ�����Ҫ����
#define ONCHIP_FLASH_SIZE  	128	 			//��ѡONCHIP_32��FLASH������С(��λΪK)
#define ONCHIP_FLASH_WREN  	1              		//ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define ONCHIP_FLASH_BASE  0x08000000 			//ONCHIP_32 FLASH����ʼ��ַ
//FLASH������ֵ
#define FLASH_KEY1               0X45670123
#define FLASH_KEY2               0XCDEF89AB

void ONCHIP_FLASH_Unlock(void);					  	//FLASH����
void ONCHIP_FLASH_Lock(void);					  	//FLASH����
u8 ONCHIP_FLASH_GetStatus(void);				  	//���״̬
u8 ONCHIP_FLASH_WaitDone(u16 time);				  	//�ȴ���������
u8 ONCHIP_FLASH_ErasePage(u32 paddr);			  	//����ҳ
u8 ONCHIP_FLASH_WriteHalfWord(u32 faddr, u16 dat);	//д�����
u16 ONCHIP_FLASH_ReadHalfWord(u32 faddr);		  	//��������
void ONCHIP_FLASH_WriteLenByte(u32 WriteAddr, u32 DataToWrite, u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 ONCHIP_FLASH_ReadLenByte(u32 ReadAddr, u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void ONCHIP_FLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void ONCHIP_FLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr, u16 WriteData);
#endif

















