#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void EXTIX_Init(void);
void LED_Init(void);
void KEY_Init(void);
void delay_init(void);

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键1
#define WK_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

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
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/	   
int main(void)
{	
    /*初始化与LED连接的硬件接口*/
    LED_Init();		
    /*初始化与按键连接的硬件接口*/    
    KEY_Init();          	                                                    
    EXTIX_Init();
    while(1)
    {
    }		 
}

/********************************************************************************************************
**函数信息 ：void EXTIX_Init(void)                    
**功能描述 ：外部中断初始化函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void EXTIX_Init(void)
{
    
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    
    /*初始化按键对应io模式*/
    KEY_Init();
    
    /*GPIOC.13 中断线以及中断初始化配置*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*下降沿触发*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器*/
    EXTI_Init(&EXTI_InitStructure);	 					                        
    
    /*GPIOB.10	  中断线以及中断初始化配置*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource10);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*下降沿触发*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器*/
    EXTI_Init(&EXTI_InitStructure);						                        
    
    /*GPIOB.11	  中断线以及中断初始化配置*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource11);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*下降沿触发*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器*/
    EXTI_Init(&EXTI_InitStructure);						                        
    
    /*使能按键所在的外部中断通道*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    /*子优先级2*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    /*使能外部中断通道*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    /*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/    
    NVIC_Init(&NVIC_InitStructure);  	  					                    
    
}


/********************************************************************************************************
**函数信息 ：void EXTI15_10_IRQHandler(void)                   
**功能描述 ：外部中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
u16 EXTI_DELAY=0;

void EXTI15_10_IRQHandler(void)
{			
    for(EXTI_DELAY=0;EXTI_DELAY<1000;EXTI_DELAY++);
    if(KEY1==0)
    {	  
        LED1_TOGGLE();
    }
    else if(KEY3==0)
    {	  
        LED3_TOGGLE();
    }	
    else if(KEY4==0)
    {	  
        LED4_TOGGLE();
    }
    /*清除EXTI10，11，13线路挂起位*/
    EXTI_ClearFlag(EXTI_Line10|EXTI_Line11|EXTI_Line13);                        
    
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
**函数信息 ：void KEY_Init(void)    
**功能描述 ：按键初始化函数 
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void KEY_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    /*使能GPIOA,GPIOB,GPIOC时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
    
    /*PC13，K1*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    /*设置成上拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*初始化GPIOC13*/    
    GPIO_Init(GPIOC, &GPIO_InitStructure);			                            
      
    /*PA0,K2（WK_UP）*/    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	
    /*设置成下拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    /*初始化GPIOA0*/    
    GPIO_Init(GPIOA, &GPIO_InitStructure);			                            
    
    /*PB10,PB11,K3,K4*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
    /*设置成上拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*初始化GPIOB.10,11*/    
    GPIO_Init(GPIOB, &GPIO_InitStructure);				                          
}


