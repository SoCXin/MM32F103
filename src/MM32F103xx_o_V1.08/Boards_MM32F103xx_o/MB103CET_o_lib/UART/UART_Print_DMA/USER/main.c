#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

u32 TEXT_TO_SEND[] = {'1','2','3','4','5','6','7','8','9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH+2)*100];
/*����DMAÿ�����ݴ��͵ĳ���*/
u16 DMA1_MEM_LEN;			                                                     
char printBuf[100];

void uart_initwBaudRate(u32 bound);
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴��ڴ�ӡ����(MM32link-option ��������)
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{ 
    u16 i;
    u8 t=0; 
    
    /*���ڳ�ʼ��Ϊ9600*/
    uart_initwBaudRate(9600); 
    /*DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*10*/    
    MYDMA_Config(DMA1_Channel4,(u32)&UART1->TDR,(u32)SendBuff,(TEXT_LENTH+2)*10);	
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\nDMA DATA:\r\n"));
    /*���ASCII�ַ�������*/
    for(i=0;i<(TEXT_LENTH+2)*100;i++)					                        
    { 
        /*���뻻�з�*/
        if(t>=TEXT_LENTH)								                        
        { 
            SendBuff[i++]=0x0d; 
            SendBuff[i]=0x0a; 
            t=0;
        }
        /*����TEXT_TO_SEND���*/
        else SendBuff[i]=TEXT_TO_SEND[t++];			                            
    } 
    
    /*ʹ��uart1 DMA*/
    UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE);
    /*��ʼһ��DMA���䣡*/    
    MYDMA_Enable(DMA1_Channel4);					        	                
    
    /*�ȴ�DMA������ɣ���ʱ������������һЩ�£����*/
    /*ʵ��Ӧ���У����������ڼ䣬����ִ�����������*/
    while(1) 
    { 
        /*�ȴ�ͨ��4�������*/
        if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)		                        
        { 
            /*���ͨ��4������ɱ�־*/
            DMA_ClearFlag(DMA1_FLAG_TC4);				                        
            break; 
        } 
    } 

    UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\nUART DMA TEST OK!\r\n"));	
    
    while(1);
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
**������Ϣ ��MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)                      
**�������� �����޸ĸ�����������޸����õ�DMA����
**������� ����
**������� ����
********************************************************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);			            
    
    /*��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ*/
    DMA_DeInit(DMA_CHx); 							                            
    DMA1_MEM_LEN=cndtr;
    /*DMA����ADC����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  	
    /*DMA�ڴ����ַ*/    
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  	
    /*���ݴ��䷽�򣬴��ڴ��ȡ���͵�����*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  	
    /*DMAͨ����DMA����Ĵ�С*/    
    DMA_InitStructure.DMA_BufferSize = cndtr;  		
    /*�����ַ�Ĵ�������*/    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*�ڴ��ַ�Ĵ�������*/    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*���ݿ��Ϊ8λ*/    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    /*���ݿ��Ϊ8λ*/	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    /*��������������ģʽ*/    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  		
    /*DMAͨ�� xӵ�������ȼ�*/    
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 	
    /*DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��*/    
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 	
    /*����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��UART1_Tx_DMA_Channel����ʶ�ļĴ���*/    
    DMA_Init(DMA_CHx, &DMA_InitStructure);  					                
    
}

/********************************************************************************************************
**������Ϣ ��MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)                   
**�������� ����ʼһ��DMA����
**������� ����
**������� ����
********************************************************************************************************/
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    /*�ر�UART1 TX DMA1 ��ָʾ��ͨ��*/
    DMA_Cmd(DMA_CHx, DISABLE );  	
    /*ʹ��UART1 TX DMA1 ��ָʾ��ͨ��*/     
    DMA_Cmd(DMA_CHx, ENABLE);  		                                            
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
