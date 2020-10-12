#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*�����������ֽ��� 200*/
#define UART_REC_LEN  			200     
/*ʹ�ܣ�1��/��ֹ��0������1����*/
#define EN_UART1_RX 			1		                                        

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))
         
/*����״̬���*/         
u16 UART_RX_STA=0; 
/*���ջ���,���UART_REC_LEN���ֽ�*/
u8 UART_RX_BUF[UART_REC_LEN];     		                                        
char printBuf[100];                                                     
/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0;

void delay_init(void);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void uart_nvic_init(u32 bound);
void LED_Init(void);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴��ڷ������ݣ����ȴ���λ��������Ϣ��Ȼ���ٴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{	
    u8 t;
    u8 len;	
    u16 times=0;
    
    /*��ʱ������ʼ��*/    
    delay_init();	    				 			                            	
    LED_Init();
    /*�����ж����ȼ�����2*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    /*���ڳ�ʼ��Ϊ9600*/    
    uart_nvic_init(9600);	 						                            
    while(1)
    {
        if(UART_RX_STA&0x8000)
        {
            /*�õ��˴ν��յ������ݳ���*/            
            len=UART_RX_STA&0x3fff;						                        
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\n�����͵���ϢΪ:\r\n"));
            for(t=0;t<len;t++)
            {
                while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET);
                UART_SendData(UART1, (u8)UART_RX_BUF[t]);
            }
            /*���뻻��*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\n\r\n"));         
            UART_RX_STA=0;
        }else
        {
            times++;
            if(times%5000==0)
            {
                UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\nMini Board ����ʵ��\r\n"));
            }
            if(times%200==0) UartSendGroup((u8*)printBuf, sprintf(printBuf,"����������,�Իس�������\r\n")); 
            /*��˸LED,��ʾϵͳ��������*/
            if(times%30==0)LED1_TOGGLE();				                        
            delay_ms(5);   
        }
    }	 
}


/********************************************************************************************************
**������Ϣ ��void uart_nvic_init(u32 bound)    
**�������� ��UART�˿ڡ��жϳ�ʼ��
**������� ��bound
**������� ��
**    ��ע ��
********************************************************************************************************/
void uart_nvic_init(u32 bound)
{
    /*GPIO�˿�����*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*ʹ��UART1��GPIOAʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);       

    /*UART1 NVIC ����*/     
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;    
    /*��ռ���ȼ�3*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;      
    /*�����ȼ�3*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    /*IRQͨ��ʹ��*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    /*����ָ���Ĳ�����ʼ��VIC�Ĵ���*/    
    NVIC_Init(&NVIC_InitStructure);					                            
    
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
}

/********************************************************************************************************
**������Ϣ ��void UART1_IRQHandler(void)    
**�������� ������1�жϷ������
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void UART1_IRQHandler(void)                	
{
    u8 Res;
    /*�����ж�(���յ������ݱ�����0x0d 0x0a��β)*/
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET)  		                
    {
        UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        /*��ȡ���յ�������*/
        Res =UART_ReceiveData(UART1);					                        
        if((Res==0x0d)&&((UART_RX_STA&0X3FFF)>0))
        {
            UART_RX_STA|=0x4000;
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        /*���յ���0x0d*/
        else if((UART_RX_STA&0x4000)&&((UART_RX_STA&0X3FFF)>0))                 
        {
            if(Res==0x0a)
            {
                UART_RX_STA|=0x8000;				
            }
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else{
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
            UART_RX_STA=UART_RX_STA&0X3FFF;
            if((UART_RX_STA)>(UART_REC_LEN-1))
                /*�������ݴ���,���¿�ʼ����*/
                UART_RX_STA=0;						                            
        }       
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
    
    LED1_ON();
    LED3_ON();
    LED4_ON();
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
