#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];

void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8* buf,u16 len);
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload);
void PVU_CheckStatus(void);
void RVU_CheckStatus(void);
void Write_Iwdg_RL(void);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ����������ѭ����ι����ϵͳһֱ��λ�����ϴ�ӡuart ok������֮��ֻ�ϵ��ӡһ��
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(9600);	 			                
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"uart ok!\r\n"));
    /*����ΪLSI32��Ƶ,��������ʼֵΪ0x7ff,��λʱ��ԼΪ1.6s*/
    Write_Iwdg_ON(IWDG_Prescaler_32,0xf);
    while(1)								
    {
       /*�޸�λ���������ѭ��,ϵͳһֱ��ӡ��������*/
       Write_Iwdg_RL();     				                                    
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
**������Ϣ ��Write_Iwdg_PR(void)             
**�������� �������������Ź�
**������� ��IWDG_Prescaler ��ѡIWDG_Prescaler_X, XΪ4,8,16,32,64,128,256,��Ӧ��Ƶֵ��Xȡֵ��ͬ
**������� ����
**    ��ע ��Reload<=0xfff,Ϊ����������ֵ  ��λʱ��������LSI 40KHzΪ�ο�  Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*�����ڲ�����ʱ��,�ȴ�ʱ�Ӿ���*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    
    /*����ʱ��Ԥ��Ƶ*/	
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    
    /*�������ؼĴ���ֵ*/	
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload&0xfff);	
    
    /*װ�ز�ʹ�ܼ�����*/	
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**������Ϣ ��PVU_CheckStatus(void)                       
**�������� �����������Ź�Ԥ��Ƶλ״̬
**������� ����
**������� ����
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*���Ԥ��Ƶλ״̬,ΪRESET�ſɸı�Ԥ��Ƶֵ*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)                                                       
        {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��RVU_CheckStatus(void)                  
**�������� �����������Ź����ر�־
**������� ����
**������� ����
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*������ر�־״̬*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  
        {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��void Write_Iwdg_RL(void)           
**�������� ��ι������
**������� ��
**������� ����
********************************************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
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

