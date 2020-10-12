/**
******************************************************************************
* @file    delay.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the delay firmware functions.
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

#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//������
//����delay_us,delay_ms


static u8  fac_us = 0;							//us��ʱ������
static u16 fac_ms = 0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
extern u32 SystemCoreClock;


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
    fac_us = SystemCoreClock / 8000000;				//Ϊϵͳʱ�ӵ�1/8
    fac_ms = (u16)fac_us * 1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����
}


//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; 					//ʱ�����
    SysTick->VAL = 0x00;        					//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL = 0X00;      					 //��ռ�����
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;							//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL = 0X00;       					//��ռ�����
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






































