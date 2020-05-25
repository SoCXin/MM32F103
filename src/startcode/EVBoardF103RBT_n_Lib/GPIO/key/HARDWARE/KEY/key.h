/**
******************************************************************************
* @file    key.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the key inc file for the library.
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
#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//按键驱动代码
//////////////////////////////////////////////////////////////////////////////////


//#define KEY0 PCin(5)
//#define KEY1 PAin(15)
//#define WK_UP  PAin(0)


#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//读取按键0
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 


#define KEY3_PRES	1		//KEY3  
#define KEY4_PRES	2		//KEY4 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数
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
