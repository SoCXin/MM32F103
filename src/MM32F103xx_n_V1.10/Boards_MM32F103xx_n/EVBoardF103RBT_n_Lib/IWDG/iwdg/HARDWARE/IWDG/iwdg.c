/**
******************************************************************************
* @file    iwdg.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the iwdg functions.
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
#include "iwdg.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//看门狗 驱动代码
/********************************************************************************************************
**函数信息 ：PVU_CheckStatus(void)
**功能描述 ：检查独立看门狗预分频位状态
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*检查预分频位状态,为RESET才可改变预分频值*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET)
        {
            break;
        }
    }
}
/********************************************************************************************************
**函数信息 ：RVU_CheckStatus(void)
**功能描述 ：检查独立看门狗重载标志
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*检查重载标志状态*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET)
        {
            break;
        }
    }
}
void IWDG_Init(u8 prer, u16 rlr)
{
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /*设置时钟预分频*/
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(prer);

    /*设置重载寄存器值*/
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(rlr & 0xfff);

    /*装载并使能计数器*/
    IWDG_ReloadCounter();
    IWDG_Enable();

}
//喂独立看门狗
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
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

