/**
******************************************************************************
* @file    can.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the can firmware functions.
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
#include "can.h"
#include "HAL_can.h"
u8 flag = 0;
/********************************************************************************************************
**������Ϣ ��void CANM_Init(void)
**�������� ��CAN��ʼ��
**������� ��None
**������� ��None
**    ��ע ��
********************************************************************************************************/
void CANM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//����CAN1 IO��
#if 1
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);//CAN1��ӳ�䵽PB8,PB9
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; //CAN RX PB8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9; //CAN TX PB9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);//CAN1��ӳ�䵽PA11,PA12
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11; //CAN RX PA11
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12; //CAN TX PA12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}
/********************************************************************************************************
**������Ϣ ��void CAN_Config(u32 CAN_Pre,CAN_Mode ID,u32 idCode1,u32 idCode2,u32 mask1,u32 mask2)
**�������� ��CAN����
**������� ��CAN_Pre:CANͨ������
CAN_250K  CAN_500K
fMode:CANͨ��ģʽ
StandardFrame_SingleFilter=0:��׼֡�����˲���ģʽ
StandardFrame_SingleFilter=1:��չ֡�����˲���ģʽ
ExtendedFrame_SingleFilter=2:��׼֡��˫�˲���ģʽ
ExtendedFrame_DoubleFilter=3:��չ֡��˫�˲���ģʽ
idCode1�����մ���1
idCode2�����մ���2
mask1�����մ�������λ1
mask2�����մ�������λ2
**������� ��None
**    ��ע ��
********************************************************************************************************/
void CAN_Config(u32 CAN_Pre, CAN_Mode ID, u32 idCode1, u32 idCode2, u32 mask1, u32 mask2)
{
    CAN_Peli_InitTypeDef          CAN_Peli_InitStructure;
    RCC_ClocksTypeDef             RCC_Clocks;
    u32 idCodeTemp1, idMaskTemp1;
    u32 idCodeTemp2, idMaskTemp2;
    CAN_Peli_FilterInitTypeDef    CAN_Peli_FilterInitStructure;

    CAN_ResetMode_Cmd(CAN1, ENABLE); // ���븴λģʽ
    CAN_Mode_Cmd(CAN1, CAN_PELIMode);//CAN����Peliģʽ

    RCC_GetClocksFreq(&RCC_Clocks);

    CAN_Peli_StructInit(&CAN_Peli_InitStructure);

    CAN_AutoCfg_BaudParam( &CAN_Peli_InitStructure, RCC_Clocks.PCLK1_Frequency, CAN_Pre);

    CAN_Peli_InitStructure.SAM = RESET;//������
    CAN_Peli_InitStructure.STM = DISABLE;//DISABLE;//�Բ�ģʽ
    CAN_Peli_InitStructure.LOM = DISABLE;//ֻ��ģʽ
    //	CAN_Peli_InitStructure.EWLR = 0x96;
    CAN_Peli_Init(&CAN_Peli_InitStructure);

    switch(ID)
    {
        case StandardFrame_SingleFilter:
            idCodeTemp1 = idCode1 << (3 + 18);
            idMaskTemp1 = mask1 << (3 + 18);
            CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Singal;
            CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1 >> 16) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp1 >> 8) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp1 >> 0) & 0xff;

            CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1 >> 16) & 0xff) | 0x1f;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = ((idMaskTemp1 >> 8) & 0xff) | 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp1 >> 0) & 0xff) | 0xff;
            break;
        case ExtendedFrame_SingleFilter:
            idCodeTemp1 = idCode1 << 3;
            idMaskTemp1 = mask1 << 3;
            CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Singal;
            CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1 >> 16) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp1 >> 8) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId3 = idCodeTemp1 & 0xff;

            CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1 >> 16) & 0xff);
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = ((idMaskTemp1 >> 8) & 0xff);
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp1 >> 0) & 0xff);
            break;
        case StandardFrame_DoubleFilter:
            idCodeTemp1 = idCode1 << (3 + 18);
            idMaskTemp1 = mask1 << (3 + 18);
            idCodeTemp2 = idCode2 << (3 + 18);
            idMaskTemp2 = mask2 << (3 + 18);
            CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Double;
            CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1 >> 16) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp2 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp2 >> 16) & 0xff;

            CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1 >> 16) & 0xff) | 0x1f;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = (idMaskTemp2 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp2 >> 16) & 0xff) | 0x1f;
            break;
        case ExtendedFrame_DoubleFilter:
            idCodeTemp1 = idCode1 << (3);
            idMaskTemp1 = mask1 << (3);
            idCodeTemp2 = idCode2 << (3);
            idMaskTemp2 = mask2 << (3);
            CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Double;
            CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1 >> 16) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp2 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp2 >> 16) & 0xff;

            CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = (idMaskTemp1 >> 16) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = (idMaskTemp2 >> 24) & 0xff;
            CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = (idMaskTemp2 >> 16) & 0xff;
            break;
        default:
            break;
    }
    CAN_Peli_FilterInit(&CAN_Peli_FilterInitStructure);
    CAN_Peli_ITConfig(CAN_IT_RI, ENABLE);
    CAN_ResetMode_Cmd(CAN1, DISABLE); //�˳���λģʽ,���빤��ģʽ
}
/********************************************************************************************************
**������Ϣ ��void Send_CANFrame(CanTxMsg* TxMessage)
**�������� ��CAN���ͱ���
**������� ��CanTxMsg* TxMessage
**������� ��None
**    ��ע ��
********************************************************************************************************/
void Send_CANFrame(CanTxMsg* TxMessage)
{
    CanPeliTxMsg     CanPeliTxMsgStructure;
    uint32_t ID = 0;
    u32 i;

    if(TxMessage->CANIDtype)  //��չ֡
    {
        ID = TxMessage->CANID << 3;

        CanPeliTxMsgStructure.FF   = 0x01;
        CanPeliTxMsgStructure.IDLL = (ID >> 0) & 0xff;
        CanPeliTxMsgStructure.IDLH = (ID >> 8) & 0xff;
        CanPeliTxMsgStructure.IDHL = (ID >> 16) & 0xff;
        CanPeliTxMsgStructure.IDHH = (ID >> 24) & 0xff;
    }
    else											//��׼֡
    {
        ID = TxMessage->CANID << 21;

        CanPeliTxMsgStructure.FF   = 0x00;
        CanPeliTxMsgStructure.IDHL = (ID >> 16) & 0xff;
        CanPeliTxMsgStructure.IDHH = (ID >> 24) & 0xff;
    }

    CanPeliTxMsgStructure.DLC  = TxMessage->DLC;
    CanPeliTxMsgStructure.RTR  = TxMessage->RTR;

    for(i = 0; i < 8; i ++ )
    {
        CanPeliTxMsgStructure.Data[i] = *(TxMessage->Data + i);
    }

    CAN_Peli_Transmit(&CanPeliTxMsgStructure);
}
/********************************************************************************************************
**������Ϣ ��void CAN1_RX1_IRQHandler(void)
**�������� ��CAN�����жϷ�����
**������� ��None
**������� ��None
**    ��ע ��
********************************************************************************************************/
void CAN1_RX1_IRQHandler(void)
{

    u32 CAN_IR_STA;

    CAN_IR_STA = CAN1_PELI->IR;
    if(CAN_IR_STA & CAN_IT_RI)
    {
        CAN_Peli_Receive(&CanPeliRxMsgStructure);//���ճɹ�������
        flag = 1;
    }
    if(CAN_IR_STA & CAN_IT_BEI)    //����
    {
    }
    if(CAN_IR_STA & CAN_IT_DOI  )  //���ջ�������
    {
        CAN1_PELI->CMR  |= 0x08;
        CAN1_PELI->CMR  |= 0x04;
    }
    if(CAN_IR_STA & CAN_IT_ALI  )  // �ٲö�ʧ
    {
    }
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
