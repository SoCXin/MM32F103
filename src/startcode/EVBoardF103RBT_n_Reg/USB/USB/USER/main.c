#include "main.h"
#include "sys.h"
#include "usb.h"
#include "uart.h"
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
    SysInit();
    GPIO_Configuration();
    usb_test();
    while(1)                                                                                               //无限循环
    {
        if(!PCin(10))
        {
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
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= 0XFFFFF0FF;	//PC10设置成输入
    GPIOC->CRH |= GPIO_CRH_CNF10_1;
    GPIOC->ODR |= GPIO_ODR_ODR10;	   	//PC10上拉
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
    /*初始化串口1,波特率为115200,无奇偶校验,无硬件流控制,1位停止位*/
    uart_initwBaudRate(115200);	 //串口初始化为9600
    printf("UART OK!\r\n");
}

void RCC_DeInit(void)
{
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;
    /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits */
    RCC->CFGR &= (uint32_t)0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;
    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    /* Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;
    /* Disable all interrupts */
    RCC->CIR = 0x00000000;
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

