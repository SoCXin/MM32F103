#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];

void UartPortInit(UART_TypeDef* UARTx);
void Uart1RxTest(UART_TypeDef* UARTx);
unsigned char inbyte(UART_TypeDef* UARTx);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，串口发送数据，并等待上位机发送信息，然后再打印出来
**输入参数 ：
**输出参数 ： 
**    备注 ：这里的UART程序不使用SYSTEM里的串口程序,而是HARDWARE里的程序
********************************************************************************************************/
int main(void)
{
    UartPortInit(UART1);  		                                                
                                                                                
    while(1)              		                                                
    {  
        /*UART1的接收，在串口软件中输入字符，可以通过打印验证接收的数据是否正确*/        
        Uart1RxTest(UART1);		                                                      
    }
}


/********************************************************************************************************
**函数信息 ：UartPortInit(UART_TypeDef* UARTx)                      
**功能描述 ：初始化串口
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：无
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
    
    /*波特率9600*/
    UART_InitStructure.UART_BaudRate =  9600; 		
    /*数据位*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;		
    /*停止位*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;				            
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    /*输入输出模式*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;		            
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None; 	
    UART_Init(UARTx, &UART_InitStructure);
    /*UART 模块使能*/
    UART_Cmd(UARTx, ENABLE);  							                        
    
    if(UARTx==UART1)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
        /*uart1_tx  pa9*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);                    
           
        /*uart1_rx  pa10*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);   
        
        /*uart2_rx  pa3*/                                                                    
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*上拉输入*/        
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
        /*复用推挽输出*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                   
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_7);
        
        /*uart3_rx  pc11*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_8);
        
        /*uart4_rx  pc11*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_8);                   
                              
        /*uart5_rx  pd2*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                
        GPIO_Init(GPIOC, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_8);                    
          
        /*uart6_rx  pc7*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		               
        GPIO_Init(GPIOE, &GPIO_InitStructure);                                  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_8);                    
             
        /*uart7_rx  pe7*/        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  			                
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
        /*上拉输入*/        
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
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                
        GPIO_Init(GPIOE, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_8);    
          
        /*uart8_rx  pe0*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 			                    
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
        /*上拉输入*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			                
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_8);        
    }

	UartSendGroup((u8*)printBuf, sprintf(printBuf,"请发送小于等于1个字节的数据！\r\n"));
    
}


/********************************************************************************************************
**函数信息 ：void Uart1RxTest(UART_TypeDef* UARTx)               
**功能描述 ：串口接收函数测试
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：无
********************************************************************************************************/

void Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    if(temp!=0)
    {
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"您输入的数据为：%c\r\n",temp));		
    }			
}

/********************************************************************************************************
**函数信息 ：unsigned char inbyte(UART_TypeDef* UARTx)                
**功能描述 ：串口接收函数
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：unsigned char 串口接收返回的字节
********************************************************************************************************/
unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;
    
    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN))
        {
            /*清除接受中断位*/
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
**函数信息 ：void UartSendByte(u8 dat)      
**功能描述 ：UART发送数据
**输入参数 ：dat
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData( UART1, dat);
    while(!UART_GetFlagStatus(UART1,UART_FLAG_TXEPT));
}


/********************************************************************************************************
**函数信息 ：void UartSendGroup(u8* buf,u16 len)     
**功能描述 ：UART发送数据
**输入参数 ：buf,len
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UartSendGroup(u8* buf,u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

