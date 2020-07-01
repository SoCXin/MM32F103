#include "sys.h"
#include "led.h"


//初始化PA8和PD2为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{
    RCC->APB2ENR |= RCC_APB2RSTR_IOPCRST; //RCC->APB2ENR|=1<<2;  //使能GPIOC时钟
    GPIOC->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;     //GPIOA->CRH|=0X00000003;//PA8 推挽输出
    GPIOC->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_MODE9;     //GPIOA->CRH|=0X00000003;//PA8 推挽输出

    GPIOC->ODR |= GPIO_ODR_ODR6 | GPIO_ODR_ODR7 | GPIO_ODR_ODR8 | GPIO_ODR_ODR9;    // GPIOA->ODR|=1<<8;   //PA8 输出高

}

/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率
返回  值：无
**************************************************************************/
void Led_Flash(u16 time)
{
    static int temp;
    if(++temp == time)	LED0 = ~LED0, temp = 0;
}
