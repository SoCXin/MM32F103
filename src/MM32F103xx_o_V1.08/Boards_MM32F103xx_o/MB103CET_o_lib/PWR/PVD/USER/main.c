#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void EXTI_Configuration(void);
void NVIC_Configuration(void);
void LED_Init(void);

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
**函数信息 ：int main (void)                          
**功能描述 ：改变VCC电压值，判断LED亮灭情况
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{    
    LED_Init();   				                                               
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Configure EXTI Line to generate an interrupt on falling edge */
    EXTI_Configuration();
    
    /* NVIC configuration */
    NVIC_Configuration();
    
    /* Configure the PVD Level to 3.0V */
    PWR_PVDLevelConfig(PWR_PVDLevel_3V0);
    
    /* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);
    
    while(1)             		        
    {
        
    }
}

/********************************************************************************************************
**函数信息 ：LED_Init(void)                        
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
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

/********************************************************************************************************
**函数信息 ：PVD_IRQHandler(void)                      
**功能描述 ：电压超过或低于设置的阈值电压后，进入该中断，翻转LED
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED翻转 */
        LED1_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}

/********************************************************************************************************
**函数信息 ：EXTI_Configuration(void)              
**功能描述 ：PVD中断配置初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16); 
    /*PVD连接到中断线16上*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;	
    /*使用中断模式 */    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*电压上升或下降越过设定阀值时都产生中断*/    
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
    /*使能中断线*/    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;					                
    EXTI_Init(&EXTI_InitStructure);						              
}

/********************************************************************************************************
**函数信息 ：NVIC_Configuration(void)              
**功能描述 ：PVD中断优先级初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                            
    
    /* Enable the PVD Interrupt */ 					                           
    NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

