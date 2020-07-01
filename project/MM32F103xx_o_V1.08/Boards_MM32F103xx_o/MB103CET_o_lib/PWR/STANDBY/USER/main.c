#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0; 
char printBuf[100];

void delay_init(void);
void uart_initwBaudRate(u32 bound);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void LED_Init(void);
void WKUP_Init(void);
u8 Check_WKUP(void);
void Sys_Standby(void);

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
**�������� �������󣬽�PA0��GND�̽�
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{ 
    /*��ʱ������ʼ��*/
    delay_init();	    
    /*�����ж����ȼ�����2*/    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    /*���ڳ�ʼ��Ϊ9600*/    
    uart_initwBaudRate(9600);	 						                        
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"�밴��wake up����3�벻��\r\n"));
    /*��ʼ����LED���ӵ�Ӳ���ӿ�*/
    LED_Init();		  									                        
    /*��ʼ��WK_UP������ͬʱ����Ƿ���������*/
    WKUP_Init();										                        
    while(1)                                                                    
    {                                                                           
        LED1_TOGGLE();                                                          
        LED3_TOGGLE();                                                          
        LED4_TOGGLE();
        /*��ʱ250ms*/        
        delay_ms(250);									                        
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"�Ѵ�standbyģʽ����\r\n"));
    }											    
}	

/********************************************************************************************************
**������Ϣ ��void delay_init(void)                         
**�������� ����ʼ���ӳٺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void delay_init(void)
{
    /*ѡ���ⲿʱ��  HCLK/8*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /*Ϊϵͳʱ�ӵ�1/8*/    
    fac_us=SystemCoreClock/8000000;				                               
    /*��OS��,����ÿ��ms��Ҫ��systickʱ���� */
    fac_ms=(u16)fac_us*1000;					                              
}


/********************************************************************************************************
**������Ϣ ��void delay_ms(u16 nms)    
**�������� ����ʱnms
**������� ��nms
**������� ��
**    ��ע ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nms<=0xffffff*8*1000/SYSCLK,72M������,nms<=1864 
********************************************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    /*ʱ�����(SysTick->LOADΪ24bit)*/    
    SysTick->LOAD=(u32)nms*fac_ms;
    /*��ռ�����*/    
    SysTick->VAL =0x00;	
    /*��ʼ����*/    
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
    do{
        temp=SysTick->CTRL;
    }
    /*�ȴ�ʱ�䵽��*/
    while((temp&0x01)&&!(temp&(1<<16)));		                            
    /*�رռ�����*/
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
    /*��ռ�����*/    
    SysTick->VAL =0X00;       					                               
}

/********************************************************************************************************
**������Ϣ ��void uart_initwBaudRate(u32 bound)     
**�������� ��UART��ʼ��
**������� ��bound
**������� ��
**    ��ע ��
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
    /*GPIO�˿�����*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /*UART1_TX   GPIOA.9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*�����������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                        
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    
    /*UART1_RX  GPIOA.10��ʼ��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    /*��������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	                 
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    
    /*UART ��ʼ������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                   

    /*���ڲ�����*/
    UART_InitStructure.UART_BaudRate = bound;
    /*�ֳ�Ϊ8λ���ݸ�ʽ*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /*һ��ֹͣλ*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /*����żУ��λ*/    
    UART_InitStructure.UART_Parity = UART_Parity_No;
    /*��Ӳ������������*/    
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    /*�շ�ģʽ*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                
    /*��ʼ������1*/
    UART_Init(UART1, &UART_InitStructure); 	
    /*�������ڽ����ж�*/    
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    /*ʹ�ܴ���1*/    
    UART_Cmd(UART1, ENABLE);                    			                    
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
**������Ϣ ��WKUP_Init(void)                         
**�������� ���ⲿ�жϻ��Ѵ�����ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void WKUP_Init(void)
{	
    GPIO_InitTypeDef  GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /*ʹ��GPIOAʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);					    
        
    /*PA.0*/    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	
    /*��������*/    
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;							    
    GPIO_Init(GPIOA, &GPIO_InitStructure);									  
    /*ʹ���ⲿ�жϷ�ʽ*/  
    /*�ж���0����GPIOA.0*/    
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);			    
             
    /*���ð������е��ⲿ��·*/    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    /*�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����*/    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*�����ش���*/    
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  				    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /* ��ʼ���ⲿ�ж�*/    
    EXTI_Init(&EXTI_InitStructure);											    
                     
    /*ʹ�ܰ������ڵ��ⲿ�ж�ͨ��*/                     
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
    /*��ռ���ȼ�2��*/    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    /*�����ȼ�2��*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 		
    /*ʹ���ⲿ�ж�ͨ��*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    /*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���*/    
    NVIC_Init(&NVIC_InitStructure); 										    
                                      
    /*���ǿ���,�������ģʽ*/                                      
    if(Check_WKUP()==0) Sys_Standby();    									    
}

/********************************************************************************************************
**������Ϣ ��Check_WKUP(void)                         
**�������� ���жϰ����Ƿ���
**������� ����
**������� ����
********************************************************************************************************/
u8 Check_WKUP(void) 
{
    /*��¼���µ�ʱ��*/
    u8 t=0;	
    LED1_ON();
    LED3_ON();
    LED4_ON();
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            /*�Ѿ�������*/
            t++;					                                             
            delay_ms(30); 
            /*���³���3����*/            
            if(t>=100)				                                            
            {                                                                   
                LED1_ON();                                                      
                LED3_ON();                                                      
                LED4_ON();  
                /*����3s������*/                
                return 1; 			                                            
            }                                                                   
        }else                                                                   
        {                                                                       
            LED1_OFF();                                                         
            LED3_OFF();                                                         
            LED4_OFF();  
            /*���²���3��*/            
            return 0; 				                                            
        }                                                                       
    }
} 

/********************************************************************************************************
**������Ϣ ��Sys_Standby(void)                         
**�������� ���������ģʽ
**������� ����
**������� ����
********************************************************************************************************/
void Sys_Standby(void)
{  
    /*ʹ��PWR����ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	 
    /*��λ����IO��*/    
    RCC->APB2RSTR|=0X01FC;
    /*ʹ�ܻ��ѹܽŹ���*/    
    PWR_WakeUpPinCmd(ENABLE);  
    /*���������STANDBY��ģʽ*/    
    PWR_EnterSTANDBYMode();	  							                        
    
}

/********************************************************************************************************
**������Ϣ ��void UartSendByte(u8 dat)      
**�������� ��UART��������
**������� ��dat
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData( UART1, dat);
    while(!UART_GetFlagStatus(UART1,UART_FLAG_TXEPT));
}


/********************************************************************************************************
**������Ϣ ��void UartSendGroup(u8* buf,u16 len)     
**�������� ��UART��������
**������� ��buf,len
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendGroup(u8* buf,u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

