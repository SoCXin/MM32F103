/**
******************************************************************************
* @file    wkup.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the wkup inc file for the library.
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
#ifndef __WKUP_H
#define __WKUP_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//开发板
//待机唤醒 代码
//////////////////////////////////////////////////////////////////////////////////

#define WKUP_KD PAin(0)			//PA0 检测是否外部WK_UP按键按下

u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PA0 WKUP唤醒初始化
void Sys_Enter_Standby(void);	//系统进入待机模式
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
