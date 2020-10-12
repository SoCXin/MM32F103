#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void DelayMs(u32 ulMs);                                                                          
void NVIC_Configuration(void);
void SysInit(void);
extern u32 SystemCoreClock;	
void uart_initwBaudRate(u32 bound);
void Tim1_UPCount_test(u16 Prescaler,u16 Period);
void Tim1_UPStatusOVCheck_test(void);
void UartSendGroup(u8* buf,u16 len);

char printBuf[100];

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{
    unsigned int uiCnt=0;                                                     
    uart_initwBaudRate(9600);	                                              
                                                                              
    /*PCLK2ʱ��(96MHz)9600��Ƶ,������ֵΪ10000,��1s���һ��*/                 
    Tim1_UPCount_test(SystemCoreClock/10000-1,10000-1);                       
    while(1)                                                                  
    {
        for(uiCnt=0; ;uiCnt++)
        {
            /*�ȴ���ʱ�����*/
            Tim1_UPStatusOVCheck_test();
            
            /*��ӡ��ǰ��ֵ*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"0x%x\r\n",uiCnt));  
        }
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
**������Ϣ ��void Tim1_UPCount_test1(u16 Period,u16 Prescaler)                     
**�������� �����ö�ʱ��1���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim1_UPCount_test(u16 Prescaler,u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    
    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK2(PCLK2δ��Ƶʱ����Ƶ,������PCLK2��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    /*ARR�Ĵ���ֵ*/
    TIM_StructInit.TIM_Period=Period; 
    /*Ԥ��Ƶֵ*/    
    TIM_StructInit.TIM_Prescaler=Prescaler;                                     
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    /*������Ƶֵ*/
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;  
    /*����ģʽ*/    
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Down;                        
    TIM_StructInit.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM1, &TIM_StructInit);
    
    TIM_Cmd(TIM1, ENABLE);	
    
    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}


/********************************************************************************************************
**������Ϣ ��Tim1_UPStatusOVCheck_test1(void)                       
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim1_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
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

