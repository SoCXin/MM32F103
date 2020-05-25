#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//开发板
//按键输入 驱动代码
//////////////////////////////////////////////////////////////////////////////////

//按键初始化函数
//PA15和PC10 设置成输入
void KEY_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //使能GPIOC时钟

    GPIOA->CRL &= 0XFFFFFFF0;	//PA0设置成输入	,K2(WK_UP)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;

    GPIOC->CRH &= 0XFFFFF0FF;	//PC10设置成输入,K1
    GPIOC->CRH |= GPIO_CRH_CNF10_1;
    GPIOC->ODR |= GPIO_ODR_ODR10;	   	//PC13上拉

    GPIOA->CRH &= 0X0FFFFFFF;	//PA15设置成输入
    GPIOA->CRH |= GPIO_CRH_CNF15_1;
    GPIOA->ODR |= GPIO_ODR_ODR15;	   	//PA15上拉

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


















