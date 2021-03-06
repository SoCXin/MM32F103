/**
******************************************************************************
* @file    wwdg.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the wwdg inc file for the library.
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
#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//看门狗 驱动代码

//////////////////////////////////////////////////////////////////////////////////

void WWDG_Init(u8 tr, u8 wr, u32 fprer); //初始化WWDG
void WWDG_Set_Counter(u8 cnt);       //设置WWDG的计数器
void WWDG_NVIC_Init(void);
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
