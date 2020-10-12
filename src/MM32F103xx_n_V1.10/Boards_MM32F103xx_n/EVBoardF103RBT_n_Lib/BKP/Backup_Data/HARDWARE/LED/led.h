/**
******************************************************************************
* @file    led.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the functions prototypes for the led firmware
*          library.
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
#ifndef __LED_H
#define __LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//LED驱动代码
//////////////////////////////////////////////////////////////////////////////////
#define LED2 PAout(8)	// PA8
#define LED1 PCout(13)	// PC13	

void LED_Init(void);//初始化


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
