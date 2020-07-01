/**
******************************************************************************
* @file    wrflash.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the wrflash inc file for the library.
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
#ifndef __ONCHIP_FLASH_H__
#define __ONCHIP_FLASH_H__
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//ONCHIP FLASH ��������
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define ONCHIP_FLASH_SIZE 128 	 		//��ѡONCHIP��FLASH������С(��λΪK)
#define ONCHIP_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define ONCHIP_FLASH_BASE 0x08000000 	//ONCHIP FLASH����ʼ��ַ



u16 ONCHIP_FLASH_ReadHalfWord(u32 faddr);		  //��������
void ONCHIP_FLASH_WriteLenByte(u32 WriteAddr, u32 DataToWrite, u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 ONCHIP_FLASH_ReadLenByte(u32 ReadAddr, u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void ONCHIP_FLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void ONCHIP_FLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr, u16 WriteData);
#endif


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















