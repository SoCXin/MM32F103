/**
******************************************************************************
* @file    key.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the key functions.
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
#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//按键输入 驱动代码
//////////////////////////////////////////////////////////////////////////////////

//按键初始化函数
//PA15和PC5 设置成输入
void KEY_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOA,GPIOC时钟

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PC10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC10

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键按松开标志
    if(mode)key_up = 1; //支持连按
    if(key_up && (KEY3 == 0 || KEY4 == 0 || WK_UP == 1))
    {
        delay_ms(10);//去抖动
        key_up = 0;
        if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
    }
    else if(KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;// 无按键按下
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
