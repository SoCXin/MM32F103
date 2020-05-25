/**
******************************************************************************
* @file    remote.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the remote inc file for the library.
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
#ifndef __RED_H
#define __RED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//红外遥控解码驱动 代码
//////////////////////////////////////////////////////////////////////////////////

#define RDATA 	PCin(9)	 	//红外数据输入脚

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0

extern u8 RmtCnt;			//按键按下的次数

void Remote_Init(void);    	//红外传感器接收头引脚初始化
u8 Remote_Scan(void);
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












