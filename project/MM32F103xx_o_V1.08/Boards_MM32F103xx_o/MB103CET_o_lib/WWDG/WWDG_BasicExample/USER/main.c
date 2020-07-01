#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0;
char printBuf[100];

void delay_init(void);
void Wwdg_reset_ON(unsigned char ucTcnt,unsigned char ucWcnt);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void uart_initwBaudRate(u32 bound);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{  
    /*��ʱ������ʼ��*/    
    delay_init();	
    /*���ڳ�ʼ��Ϊ9600*/    
    uart_initwBaudRate(9600);				                                    
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"uart ok!\r\n"));
    
    /*���ڿ��Ź���,΢�������뼶��λ,��ι������WWDG_SetCounter()����*/
    /*���ڿ��Ź�FUN_2,��ʱ����ι��������λ*/
    Wwdg_reset_ON(0x7e,0x7f);              	                                    
                                                                               
    while(1)                                                                   
    {
        /*�޸�λ���������ѭ��,ϵͳһֱ��ӡ�������� */       
        WWDG_SetCounter(0x7e);
        delay_ms(1);
    }	
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
**������Ϣ ��Wwdg_reset_test()                     
**�������� ��ʹ�ô��ڿ��Ź�
**������� ��unsigned char ucTcn ���Ź��ݼ�������ֵ unsigned char ucWcnt����ֵ
**������� ����
**    ��ע ��ϵͳ����ʱ���Ź�Ϊʧ��״̬,���Ź�ʧ�ܺ����ֶ��ر�,��λ���Ź�ʧ�� 
********************************************************************************************************/
void Wwdg_reset_ON(unsigned char ucTcnt,unsigned char ucWcnt)
{
    /*ʧ�ܴ��ڿ��Ź�ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    
    /*WWDG_Prescaler_x,xΪ1,2,4,8,WWDG counter clock = (PCLK1/4096)/x */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);
    
    /*Ϊ���Ź��Լ���������ֵ,Ϊ7λ��������ֵΪ0x40~0x7f֮��*/
    WWDG_Enable(ucTcnt);
}

/********************************************************************************************************
**������Ϣ ��Wwdg_irq_test()                      
**�������� �����ڿ��Ź��ж�����
**������� ����
**������� ����
********************************************************************************************************/
void Wwdg_irq_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetWindowValue(0x70);
    WWDG_EnableIT();
    /*�˴�Ϊ����,����ֵ����С�ڴ���ֵ,��������������λ*/
    WWDG_Enable(0x66);		
}

/********************************************************************************************************
**������Ϣ ��void WWDG_IRQHandler (void)                   
**�������� �����ڿ��Ź��жϺ���
**������� ����
**������� ����
**    ��ע �����ݼ�����������0x40ʱ�����ж�,������������С��0x7f����0x40,���ӽ���0x40���ܻ�Ƶ��������ж�           
********************************************************************************************************/
void WWDG_IRQHandler (void)
{	
    if(WWDG_GetFlagStatus())
    {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);                                                                               
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
