/**
******************************************************************************
* @file    key.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the key functions.
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
#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//�������� ��������
//////////////////////////////////////////////////////////////////////////////////

//������ʼ������
//PA15��PC5 ���ó�����
void KEY_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //ʹ��GPIOA,GPIOCʱ��

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PC10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC10

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP����
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //�������ɿ���־
    if(mode)key_up = 1; //֧������
    if(key_up && (KEY3 == 0 || KEY4 == 0 || WK_UP == 1))
    {
        delay_ms(10);//ȥ����
        key_up = 0;
        if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
    }
    else if(KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;// �ް�������
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
