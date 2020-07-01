/**
******************************************************************************
* @file    can.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the can inc file for the library.
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
#ifndef __CAN_H
#define __CAN_H

#include "HAL_conf.h"
#define CAN_ID_STD                   0
#define CAN_ID_EXT                   1
#define CAN_DATA_FRAME       					0
#define CAN_REMOTE_FRAME              1
#define CAN_500K                     500000
#define CAN_250K                     250000

#define TX_CANID  									 0x172

typedef enum
{
    StandardFrame_SingleFilter = 0,
    ExtendedFrame_SingleFilter = 1,
    StandardFrame_DoubleFilter = 2,
    ExtendedFrame_DoubleFilter = 3,
} CAN_Mode;

typedef struct
{
    uint32_t CANID;
    uint32_t CANIDtype;

    uint8_t RTR;

    uint8_t DLC;
    uint8_t Data[8];

} CanTxMsg;

extern CanPeliRxMsg CanPeliRxMsgStructure;
extern u8 flag;
void CANM_Init(void);
void CAN_Config(u32 CAN_Pre, CAN_Mode ID, u32 idCode1, u32 idCode2, u32 mask1, u32 mask2);
void Send_CANFrame(CanTxMsg* TxMessage);
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
