#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];

void UartPortInit(UART_TypeDef* UARTx);
void Uart1RxTest(UART_TypeDef* UARTx);
unsigned char inbyte(UART_TypeDef* UARTx);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴��ڷ������ݣ����ȴ���λ��������Ϣ��Ȼ���ٴ�ӡ����
**������� ��
**������� �� 
**    ��ע �������UART����ʹ��SYSTEM��Ĵ��ڳ���,����HARDWARE��ĳ���
********************************************************************************************************/
int main(void)
{
    UartPortInit(UART1);  		                                                
                                                                                
    while(1)              		                                                
    {  
        /*UART1�Ľ��գ��ڴ�������������ַ�������ͨ����ӡ��֤���յ������Ƿ���ȷ*/        
        Uart1RxTest(UART1);		                                                      
    }
}


/********************************************************************************************************
**������Ϣ ��UartPortInit(UART_TypeDef* UARTx)                      
**�������� ����ʼ������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
********************************************************************************************************/
void UartPortInit(UART_TypeDef* UARTx)
{
    UART_InitTypeDef       UART_InitStructure;  
    GPIO_InitTypeDef       GPIO_InitStructure;   
    
    switch (*(uint32_t*)&UARTx)
    {
    case UART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
        break;
	case UART2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
        break;
	case UART3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART3, ENABLE);
        break;
	case UART4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        break;
	case UART5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        break;
	case UART6_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART6, ENABLE);
        break;
	case UART7_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
        break;
	case UART8_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
        break;
    default:
        break;
    }
    
    /*������9600*/
    UART_InitStructure.UART_BaudRate =  9600; 		
    /*����λ*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;		
    /*ֹͣλ*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;				            
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    /*�������ģʽ*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;		            
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None; 	
    UART_Init(UARTx, &UART_InitStructure);
    /*UART ģ��ʹ��*/
    UART_Cmd(UARTx, ENABLE);  							                        
    
    if(UARTx==UART1)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
        /*uart1_tx  pa9*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);                    
           
        /*uart1_rx  pa10*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			                   
        GPIO_Init(GPIOA, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
        
    }	
    
    if(UARTx==UART2)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
        /*uart2_tx  pa2*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);   
        
        /*uart2_rx  pa3*/                                                                    
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			               
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
    }	
    
    if(UARTx==UART3)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        /*uart3_tx  pc10*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*�����������*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                   
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_7);
        
        /*uart3_rx  pc11*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			              
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_7);                   
    }	                                                                        
                                                                                
    if(UARTx==UART4)                                                            
    {                                                                           
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
        /*uart4_tx  pc10*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_8);
        
        /*uart4_rx  pc11*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			               
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_8);                   
    }                                                                           
                                                                                
    if(UARTx==UART5)                                                            
    {                                                                           
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
        /*uart5_tx  pc12*/       
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_8);                   
                              
        /*uart5_rx  pd2*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			               
        GPIO_Init(GPIOD, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_8);        
    }
    
    if(UARTx==UART6)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
        /*uart6_tx  pc6*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_8);                    
          
        /*uart6_rx  pc7*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			               
        GPIO_Init(GPIOC, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_8);            
    }   
      
    if(UARTx==UART7)    
    {   
        /*uart7_tx  pe8*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOE, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_8);                    
             
        /*uart7_rx  pe7*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			               
        GPIO_Init(GPIOE, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_8);            
    }   
      
    if(UARTx==UART8)    
    {   
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   
        /*uart8_tx  pe1*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                
        GPIO_Init(GPIOE, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_8);    
          
        /*uart8_rx  pe0*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 			                    
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			                
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_8);        
    }

	UartSendGroup((u8*)printBuf, sprintf(printBuf,"�뷢��С�ڵ���1���ֽڵ����ݣ�\r\n"));
    
}


/********************************************************************************************************
**������Ϣ ��void Uart1RxTest(UART_TypeDef* UARTx)               
**�������� �����ڽ��պ�������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
********************************************************************************************************/

void Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    if(temp!=0)
    {
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"�����������Ϊ��%c\r\n",temp));		
    }			
}

/********************************************************************************************************
**������Ϣ ��unsigned char inbyte(UART_TypeDef* UARTx)                
**�������� �����ڽ��պ���
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��unsigned char ���ڽ��շ��ص��ֽ�
********************************************************************************************************/
unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;
    
    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN))
        {
            /*��������ж�λ*/
            UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);                       
            break;
        }
    }
    temp = (uint8_t)UART_ReceiveData(UARTx);
    if(temp==0xd)
    {
        return 0;		
    }
    return temp;
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

