/**
******************************************************************************
* @file    malloc.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the malloc inc file for the library.
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
#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//�ڴ���� ��������
//////////////////////////////////////////////////////////////////////////////////



#ifndef NULL
#define NULL 0
#endif

//�ڴ�����趨.
#define MEM_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE			14*1024  						//�������ڴ� 14K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//�ڴ���С


//�ڴ���������
struct _m_mallco_dev
{
    void (*init)(void);				//��ʼ��
    u8 (*perused)(void);		  	//�ڴ�ʹ����
    u8 	*membase;					//�ڴ��
    u16 *memmap; 					//�ڴ����״̬��
    u8  memrdy; 					//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��

void mymemset(void *s, u8 c, u32 count);	//�����ڴ�
void mymemcpy(void *des, void *src, u32 n); //�����ڴ�
void mem_init(void);					 //�ڴ�����ʼ������(��/�ڲ�����)
u32 mem_malloc(u32 size);		 		//�ڴ����(�ڲ�����)
u8 mem_free(u32 offset);		 		//�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);					//���ڴ�ʹ����(��/�ڲ�����)
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);  				//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);				//�ڴ����(�ⲿ����)
void *myrealloc(void *ptr, u32 size);	//���·����ڴ�(�ⲿ����)
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










