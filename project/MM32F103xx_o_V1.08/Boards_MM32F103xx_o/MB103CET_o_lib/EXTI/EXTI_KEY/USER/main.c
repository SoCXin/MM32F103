#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void EXTIX_Init(void);
void LED_Init(void);
void KEY_Init(void);
void delay_init(void);

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����1
#define WK_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//��ȡ����4

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
**������Ϣ ��int main (void)                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/	   
int main(void)
{	
    /*��ʼ����LED���ӵ�Ӳ���ӿ�*/
    LED_Init();		
    /*��ʼ���밴�����ӵ�Ӳ���ӿ�*/    
    KEY_Init();          	                                                    
    EXTIX_Init();
    while(1)
    {
    }		 
}

/********************************************************************************************************
**������Ϣ ��void EXTIX_Init(void)                    
**�������� ���ⲿ�жϳ�ʼ������
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void EXTIX_Init(void)
{
    
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    
    /*��ʼ��������Ӧioģʽ*/
    KEY_Init();
    
    /*GPIOC.13 �ж����Լ��жϳ�ʼ������*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*�½��ش���*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���*/
    EXTI_Init(&EXTI_InitStructure);	 					                        
    
    /*GPIOB.10	  �ж����Լ��жϳ�ʼ������*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource10);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*�½��ش���*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���*/
    EXTI_Init(&EXTI_InitStructure);						                        
    
    /*GPIOB.11	  �ж����Լ��жϳ�ʼ������*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource11);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*�½��ش���*/
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /*����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���*/
    EXTI_Init(&EXTI_InitStructure);						                        
    
    /*ʹ�ܰ������ڵ��ⲿ�ж�ͨ��*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    /*�����ȼ�2*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    /*ʹ���ⲿ�ж�ͨ��*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    /*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���*/    
    NVIC_Init(&NVIC_InitStructure);  	  					                    
    
}


/********************************************************************************************************
**������Ϣ ��void EXTI15_10_IRQHandler(void)                   
**�������� ���ⲿ�жϺ���
**������� ��
**������� ��
**���ú��� ��
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
    /*���EXTI10��11��13��·����λ*/
    EXTI_ClearFlag(EXTI_Line10|EXTI_Line11|EXTI_Line13);                        
    
}


/********************************************************************************************************
**������Ϣ ��LED_Init(void)                        
**�������� ��LED��ʼ��
**������� ����
**������� ����
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
**������Ϣ ��void KEY_Init(void)    
**�������� ��������ʼ������ 
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void KEY_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    /*ʹ��GPIOA,GPIOB,GPIOCʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
    
    /*PC13��K1*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*��ʼ��GPIOC13*/    
    GPIO_Init(GPIOC, &GPIO_InitStructure);			                            
      
    /*PA0,K2��WK_UP��*/    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    /*��ʼ��GPIOA0*/    
    GPIO_Init(GPIOA, &GPIO_InitStructure);			                            
    
    /*PB10,PB11,K3,K4*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*��ʼ��GPIOB.10,11*/    
    GPIO_Init(GPIOB, &GPIO_InitStructure);				                          
}


