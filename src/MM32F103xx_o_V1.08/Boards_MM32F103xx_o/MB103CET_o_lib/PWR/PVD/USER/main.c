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
**������Ϣ ��int main (void)                          
**�������� ���ı�VCC��ѹֵ���ж�LED�������
**������� ��
**������� ��
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
**������Ϣ ��PVD_IRQHandler(void)                      
**�������� ����ѹ������������õ���ֵ��ѹ�󣬽�����жϣ���תLED
**������� ����
**������� ����
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED��ת */
        LED1_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}

/********************************************************************************************************
**������Ϣ ��EXTI_Configuration(void)              
**�������� ��PVD�ж����ó�ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16); 
    /*PVD���ӵ��ж���16��*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;	
    /*ʹ���ж�ģʽ */    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*��ѹ�������½�Խ���趨��ֵʱ�������ж�*/    
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
    /*ʹ���ж���*/    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;					                
    EXTI_Init(&EXTI_InitStructure);						              
}

/********************************************************************************************************
**������Ϣ ��NVIC_Configuration(void)              
**�������� ��PVD�ж����ȼ���ʼ��
**������� ����
**������� ����
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

