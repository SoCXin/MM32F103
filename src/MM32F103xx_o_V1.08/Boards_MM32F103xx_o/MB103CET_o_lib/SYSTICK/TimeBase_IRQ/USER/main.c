#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

static __IO uint32_t TimingDelay;
void LED_Init(void);

void Delay(__IO uint32_t nTime);
extern u32 SystemCoreClock;

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	

/********************************************************************************************************
**函数信息 ：TimingDelay_Decrement(void)                    
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}

/********************************************************************************************************
**函数信息 ：SysTick_Handler(void)                    
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/********************************************************************************************************
**函数信息 ：Delay(__IO uint32_t nTime)                     
**功能描述 ：程序应用调用延时
**输入参数 ：nTime：延时
**输出参数 ：无
********************************************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;
    
    while(TimingDelay != 0);
}

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动频率1s
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{	
    
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);	                                 
    
    LED_Init();		  						                                  
    while(1)
    {
        LED1_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        Delay(500);
    }
}

/********************************************************************************************************
**函数信息 ：int LED_Init (void)                          
**功能描述 ：LED初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_OFF();
    LED3_OFF();
    LED4_OFF();
}

