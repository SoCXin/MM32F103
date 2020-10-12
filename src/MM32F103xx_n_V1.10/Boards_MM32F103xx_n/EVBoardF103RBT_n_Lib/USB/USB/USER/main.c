/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main functions.
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
#include "main.h"
#include "key.h"
#include "led.h"
#include "delay.h"

/*main.c文件内函数声明*/
void DelayMs(u32 ulMs);                                                                                  //非精确延时函数
void NVIC_Configuration(void);
void SysInit(void);
void GPIO_Configuration(void);
void SetUSBSysClockTo48M(void);
#define UART_PRINTF

int main(void)
{
    u8 key_read = 0;
    SysInit();
    GPIO_Configuration();
    KEY_Init();
    LED_Init();
    usb_test();
    LED1 = 0;
    while(1)                                                                                               //无限循环
    {
        LED1 = 0;
        if(!GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_10))
        {
            LED1 = 1;
            keyBoard_ctrl(0x00, 0x00, 4, 0x00, 0x00, 0x00, 0x00);
        }
    }
}
/********************************************************************************************************
**函数信息 ：void DelayMs(u32 ulMs)
**功能描述 ：非精确延时
**输入参数 ：u32 ulMs 毫秒
**输出参数 ：无
**备    注 ：系统时钟为72MHz时,参数为1000时约延时1s钟,当系统更改可适当更改j的值
用于一些延时要求不高的场合
********************************************************************************************************/
void DelayMs(u32 ulMs)
{
    u32 i;
    u16 j;
    for(i = ulMs; i > 0; i--)
    {
        for(j = 4700; j > 0; j--);
    }
}
/**
* @brief
* @param  None
* @retval None
*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*将PA1配置为模拟输入*/
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**
* @brief  Configures Vector Table base location.
* @param  None
* @retval None
*/
void NVIC_Configuration(void)
{
    /*配置中断优先级模式,0位抢占优先级,4位子优先级*/
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}
/********************************************************************************************************
**函数信息 ：SysInit(void)
**功能描述 ：完成系统初始化
**输入参数 ：无
**输出参数 ：无
**    备注 ：系统时钟初始化 96MHz=AHP=PCLK2=2*PCLK1
完成串口初始化 波特率9600,无奇偶校验,无硬件流控制,1位停止位
********************************************************************************************************/
void SysInit(void)
{
    SetUSBSysClockTo48M();
    NVIC_Configuration();

    /*初始化串口1,波特率为9600,无奇偶校验,无硬件流控制,1位停止位*/
    UartInit(UART1);
    uart_printf("UART OK!\r\n");
}


void SetUSBSysClockTo48M(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    RCC_DeInit();
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    }
    while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        /* Flash 0 wait state ,bit0~2*/
        FLASH->ACR &= ~0x07;
        FLASH->ACR |= 0x02;
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        /*  PLL configuration:  = (HSE ) * (5+1) = 48MHz */
        RCC->CFGR &= (uint32_t)0xFFFCFFFF;
        RCC->CR &= (uint32_t)0x000FFFFF;

        RCC->CFGR |= (uint32_t ) RCC_CFGR_PLLSRC ;
        RCC->CR |= 0x14000000;//pll = 6/1
        //RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        //RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);

        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* Wait till PLL is ready */
        while((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }

        /* Select PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
        {
        }
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
          configuration. User can add here some code to deal with this error */
    }
    SystemCoreClock = 48000000;

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
