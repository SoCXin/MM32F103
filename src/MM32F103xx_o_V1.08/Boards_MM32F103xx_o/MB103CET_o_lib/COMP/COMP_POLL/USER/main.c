#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];
void UartSendGroup(u8* buf,u16 len);
void UartSendAscii(char *str);
void Comp_Config(uint32_t COMP_Selection_COMPx);
unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx);
void uart_initwBaudRate(u32 bound);
void GPIO_ConfigInit(void);
void RCC_ConfigInit(void);

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    int a,i;
    uart_initwBaudRate(9600);
    RCC_ConfigInit();

    Comp_Config(COMP_Selection_COMP1);    

    while(1)
    {
        a=Comp_StatusCheck( COMP_Selection_COMP1);
        /*��ӡ��ǰ�Ƚ������״̬*/
        if(a==0)
        {
            /*��������С�ڷ�������*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"INP < INM \r\n"));
        }
        else if(a==1)
        {
            /*����������ڷ�������*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"INP > INM \r\n"));
        }
        else
        {
            /*��������*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"Param Error \r\n"));
        }
        i=1000000; while(i--);
    }	
}

/********************************************************************************************************
**������Ϣ ��void RCC_ConfigInit(void)
**�������� ��ʱ�ӳ�ʼ��
**������� ��
**������� ��
********************************************************************************************************/
void RCC_ConfigInit(void)
{
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    
}

/********************************************************************************************************
**������Ϣ ��void GPIO_ConfigInit(void)
**�������� ��GPIO����
**������� ��
**������� ��
********************************************************************************************************/
void GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

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
**������Ϣ ��void UartSendByte(u8 dat)
**�������� ��UART��������
**������� ��
**������� ��
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


/********************************************************************************************************
**������Ϣ ��void Comp_Config(uint32_t COMP_Selection_COMPx)    
**�������� ��COMP��ʼ��
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void Comp_Config(uint32_t COMP_Selection_COMPx)
{	
    COMP_InitTypeDef COMP_InitStructure;
    GPIO_ConfigInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);

    /*�Ƚ������������Ϊ 3/4 Vref*/
    COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_IO2; 	        
    /*����PA1Ϊ�Ƚ�������������*/
    COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO2;     
    COMP_InitStructure.COMP_Output = COMP_Output_None;
    COMP_InitStructure.COMP_BlankingSrce = COMP_BlankingSrce_None;
    COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
    COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
    COMP_InitStructure.COMP_Mode = COMP_Mode_MediumSpeed;

    COMP_Init(COMP_Selection_COMPx, &COMP_InitStructure);

    COMP_Cmd(COMP_Selection_COMPx, ENABLE);
}


/********************************************************************************************************
**������Ϣ ��unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx)    
**�������� ��COMP״̬���
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx)
{
    unsigned char chCapStatus=2;
    if(COMP_GetOutputLevel(COMP_Selection_COMPx)==0)
    {
        chCapStatus=0;
    }
    else
    {
        chCapStatus=1;
    }
    return chCapStatus;
}
