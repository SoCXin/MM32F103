/**
******************************************************************************
* @file    iwdg.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the iwdg inc file for the library.
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
#ifndef __IWDG_H
#define __IWDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//看门狗 驱动代码
//////////////////////////////////////////////////////////////////////////////////


void IWDG_Init(u8 prer, u16 rlr);
void IWDG_Feed(void);


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
