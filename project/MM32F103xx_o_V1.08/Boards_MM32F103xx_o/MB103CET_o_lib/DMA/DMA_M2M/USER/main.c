#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"	  

void SystemClk_HSEInit(void);
void DMA_m8tom8_test(void);
void DMA_m8tom16_test(void);
void DMA_m8tom32_test(void);
void DMA_m16tom8_test(void);
void DMA_m16tom16_test(void);
void DMA_m16tom32_test(void);
void uart_initwBaudRate(u32 bound);
void DMAcheckStatus(uint32_t DMA_FLAG);
void UartSendGroup(u8* buf,u16 len);

unsigned char dma1Flag = 0x0;
unsigned char dma2Flag = 0x0;
char printBuf[100];

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴������ִ�ӡDMA��������
**������� 
**������� ��
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(9600);	      
    
//    DMA_m8tom8_test();
//    DMA_m8tom16_test();
//    DMA_m8tom32_test();
    DMA_m16tom8_test();
//    DMA_m16tom16_test();
//    DMA_m16tom32_test();
       
    while(1)              		
    {
        
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
**������Ϣ ��DMA_m8tom8_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��8λ���ݰᵽ�洢������һ����ַ������8λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m8tom8_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)(0x20003000+i)) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);				
    
    dma2Flag = 0x0;
    
    DMA_Cmd(DMA1_Channel2, ENABLE);
    
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    
    
    DMA_Cmd(DMA1_Channel2, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = *((volatile unsigned char *)(0x20004000+i));
        
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA_m8tom16_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��8λ���ݰᵽ�洢������һ����ַ������16λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m8tom16_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)(0x20003000+i)) = i +1;
        
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    
    DMA_DeInit(DMA1_Channel1);
    
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);				
    dma1Flag = 0x0;
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    while(1)
    {
        if(dma1Flag)
        {
            dma1Flag = 0x0;
            break;
        }
    }
    
    DMA_Cmd(DMA1_Channel1, DISABLE);
    for(i=0;i<128;i++)
    {
        
        temp = *((volatile unsigned char *)(0x20004000+i));
        
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA_m8tom32_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��8λ���ݰᵽ�洢������һ����ַ������32λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m8tom32_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)(0x20003000+i)) = i +1;
        
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    
    DMA_DeInit(DMA1_Channel3);
    
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC3);
    
    DMA_Cmd(DMA1_Channel3, DISABLE);
    for(i=0;i<256;i++)
    {
        
        temp = *((volatile unsigned char *)(0x20004000+i));

		UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA_m16tom8_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��16λ���ݰᵽ�洢������һ����ַ������8λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m16tom8_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)(0x20003000+(i*2))) = 0xb1b0+i;
        
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel4);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    
    
    DMA_Cmd(DMA1_Channel4, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC4);
    
    DMA_Cmd(DMA1_Channel4, DISABLE);
    for(i=0;i<32;i++)
    {
        
        temp = *((volatile unsigned char *)(0x20004000+i));
        
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA_m16tom16_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��16λ���ݰᵽ�洢������һ����ַ������16λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m16tom16_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)(0x20003000+(i*2))) = 0xa1a0+i;
        
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    
    DMA_DeInit(DMA1_Channel5);
    
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    
    
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC5);
    
    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = *((volatile unsigned short *)(0x20004000+(i*2)));
        
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA_m16tom32_test()          
**�������� : �Ѵ洢���е�ĳһ��ַ��ʼ��64��16λ���ݰᵽ�洢������һ����ַ������32λ��ʽ�洢
**������� ����
**������� ����
********************************************************************************************************/
void DMA_m16tom32_test()
{
    unsigned int i;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)(0x20003000+(i*2))) = 0xc1c0+i;
        
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    
    DMA_DeInit(DMA1_Channel6);
    
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x20004000;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel6, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC6);
    
    DMA_Cmd(DMA1_Channel6, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = *((volatile unsigned int *)(0x20004000+(i*4)));
        
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp%d=0x%x\r\n",i,temp));
    }
}

/********************************************************************************************************
**������Ϣ ��DMA1_Channel1_IRQHandler()     
**�������� : DMA1ͨ��1���жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void DMA1_Channel1_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        dma1Flag = 0x1;
    }
}

/********************************************************************************************************
**������Ϣ ��DMA1_Channel2_IRQHandler()     
**�������� : DMA1ͨ��2���жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void DMA1_Channel2_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        dma2Flag = 0x1;
    }
    
}

/********************************************************************************************************
**������Ϣ ��DMAcheckStatus(uint32_t DMA_FLAG)     
**�������� : ��ѯDMA�ı�־λ
**������� ��uint32_t DMA_FLAG��DMA��״̬��־λ
**������� ����
********************************************************************************************************/
void DMAcheckStatus(uint32_t DMA_FLAG)
{
    while(1)
    {
        if(DMA_GetFlagStatus(DMA_FLAG))
        {
            DMA_ClearFlag(DMA_FLAG);
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)       
**�������� : DMAͨ��ʧ��
**������� ��DMA_Channel_TypeDef* DMAy_Channelx��ѡ��ͨ��
**������� ����
********************************************************************************************************/
void DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
{
    //disable DMA_EN
    DMA_Cmd(DMAy_Channelx, DISABLE);
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

