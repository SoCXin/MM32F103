/**
******************************************************************************
* @file    wwdg.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the wwdg functions.
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
#include "wwdg.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//看门狗 驱动代码



//保存WWDG计数器的设置值,默认为最大.
u8 WWDG_CNT = 0x7f;

void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDG时钟使能

    WWDG_SetPrescaler(fprer);////设置IWDG预分频值

    WWDG_SetWindowValue(wr);//设置窗口值

    WWDG_Enable(tr);	 //使能看门狗 ,	设置 counter .

    WWDG_ClearFlag();

    WWDG_NVIC_Init();//初始化窗口看门狗 NVIC

    WWDG_EnableIT(); //开启窗口看门狗中断
}
//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);
}
//窗口看门狗中断服务程序
void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占2，子优先级3，组2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //抢占2，子优先级3，组2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}

void WWDG_IRQHandler(void)
{
    // Update WWDG counter
    WWDG_SetCounter(0x7F);	  //当禁掉此句后,窗口看门狗将产生复位
    // Clear EWI flag */
    WWDG_ClearFlag();	  //清除提前唤醒中断标志位
    // Toggle GPIO_Led pin 7 */
    LED2 = !LED2;
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
