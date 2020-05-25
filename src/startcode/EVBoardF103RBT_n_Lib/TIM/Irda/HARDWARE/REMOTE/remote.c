/**
******************************************************************************
* @file    remote.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the remote functions.
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
#include "remote.h"
#include "delay.h"
#include "uart.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//����ң�ؽ������� ����
//////////////////////////////////////////////////////////////////////////////////

//����ң�س�ʼ��
//����IO�Լ���ʱ��4�����벶��
void Remote_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //ʹ��GPIOC&AFIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//TIM2 ʱ��ʹ��

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);//I2C1��ӳ��


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA1 ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_9);	//��ʼ��GPIOA1


    TIM_TimeBaseStructure.TIM_Period = 10000; //�趨�������Զ���װֵ ���10ms���
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1); 	//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // ѡ������� IC2ӳ�䵽TI5��
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��

    TIM_Cmd(TIM3, ENABLE ); 	//ʹ�ܶ�ʱ��5


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig( TIM3, TIM_IT_Update | TIM_IT_CC4, ENABLE); //��������ж� ,����CC2IE�����ж�
}


//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����
//[4]:����������Ƿ��Ѿ�������
//[3:0]:�����ʱ��
u8 	RmtSta = 0;
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec = 0;	//������յ�������
u8  RmtCnt = 0;	//�������µĴ���
//��ʱ��5�жϷ������
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(RmtSta & 0x80) //�ϴ������ݱ����յ���
        {
            RmtSta &= ~0X10;						//ȡ���������Ѿ���������
            if((RmtSta & 0X0F) == 0X00)RmtSta |= 1 << 6; //����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
            if((RmtSta & 0X0F) < 14)RmtSta++;
            else
            {
                RmtSta &= ~(1 << 7); //���������ʶ
                RmtSta &= 0XF0;	//��ռ�����
            }
        }
    }
    if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
        if(RDATA)//�����ز���
        {

            TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
            TIM_SetCounter(TIM3, 0);	   	//��ն�ʱ��ֵ
            RmtSta |= 0X10;					//����������Ѿ�������
        }
        else  //�½��ز���
        {
            Dval = TIM_GetCapture4(TIM3); //��ȡCCR1Ҳ������CC1IF��־λ
            TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); //CC4P=0	����Ϊ�����ز���

            if(RmtSta & 0X10)					//���һ�θߵ�ƽ����
            {
                if(RmtSta & 0X80) //���յ���������
                {

                    if(Dval > 300 && Dval < 800)			//560Ϊ��׼ֵ,560us
                    {
                        RmtRec <<= 1;	//����һλ.
                        RmtRec |= 0;	//���յ�0
                    }
                    else if(Dval > 1400 && Dval < 1800)	//1680Ϊ��׼ֵ,1680us
                    {
                        RmtRec <<= 1;	//����һλ.
                        RmtRec |= 1;	//���յ�1
                    }
                    else if(Dval > 2200 && Dval < 2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
                    {
                        RmtCnt++; 		//������������1��
                        RmtSta &= 0XF0;	//��ռ�ʱ��
                    }
                }
                else if(Dval > 4200 && Dval < 4700)		//4500Ϊ��׼ֵ4.5ms
                {
                    RmtSta |= 1 << 7;	//��ǳɹ����յ���������
                    RmtCnt = 0;		//�����������������
                }
            }
            RmtSta &= ~(1 << 4);
        }
    }
    TIM_ClearFlag(TIM3, TIM_IT_Update | TIM_IT_CC4);
}

//����������
//����ֵ:
//	 0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void)
{
    u8 sta = 0;
    u8 t1, t2;
    if(RmtSta & (1 << 6)) //�õ�һ��������������Ϣ��
    {
        t1 = RmtRec >> 24;			//�õ���ַ��
        t2 = (RmtRec >> 16) & 0xff;	//�õ���ַ����
        if((t1 == (u8)~t2) && t1 == REMOTE_ID) //����ң��ʶ����(ID)����ַ
        {
            t1 = RmtRec >> 8;
            t2 = RmtRec;
            if(t1 == (u8)~t2)sta = t1; //��ֵ��ȷ
        }
        if((sta == 0) || ((RmtSta & 0X80) == 0)) //�������ݴ���/ң���Ѿ�û�а�����
        {
            RmtSta &= ~(1 << 6); //������յ���Ч������ʶ
            RmtCnt = 0;		//�����������������
        }
    }
    return sta;
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

