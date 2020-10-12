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
**函数信息 ：int main (void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
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
        /*打印当前比较器输出状态*/
        if(a==0)
        {
            /*正向输入小于反向输入*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"INP < INM \r\n"));
        }
        else if(a==1)
        {
            /*正向输入大于反向输入*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"INP > INM \r\n"));
        }
        else
        {
            /*参数错误*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"Param Error \r\n"));
        }
        i=1000000; while(i--);
    }	
}

/********************************************************************************************************
**函数信息 ：void RCC_ConfigInit(void)
**功能描述 ：时钟初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void RCC_ConfigInit(void)
{
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    
}

/********************************************************************************************************
**函数信息 ：void GPIO_ConfigInit(void)
**功能描述 ：GPIO配置
**输入参数 ：
**输出参数 ：
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
**函数信息 ：void uart_initwBaudRate(u32 bound)     
**功能描述 ：UART初始化
**输入参数 ：bound
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
    /*GPIO端口设置*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /*UART1_TX   GPIOA.9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*复用推挽输出*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                        
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    
    /*UART1_RX  GPIOA.10初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    /*浮空输入*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	                 
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    
    /*UART 初始化设置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                   

    /*串口波特率*/
    UART_InitStructure.UART_BaudRate = bound;
    /*字长为8位数据格式*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /*一个停止位*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /*无奇偶校验位*/    
    UART_InitStructure.UART_Parity = UART_Parity_No;
    /*无硬件数据流控制*/    
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    /*收发模式*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                
    /*初始化串口1*/
    UART_Init(UART1, &UART_InitStructure); 	
    /*开启串口接受中断*/    
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    /*使能串口1*/    
    UART_Cmd(UART1, ENABLE);                    			                    
}

/********************************************************************************************************
**函数信息 ：void UartSendByte(u8 dat)
**功能描述 ：UART发送数据
**输入参数 ：
**输出参数 ：
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


/********************************************************************************************************
**函数信息 ：void Comp_Config(uint32_t COMP_Selection_COMPx)    
**功能描述 ：COMP初始化
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void Comp_Config(uint32_t COMP_Selection_COMPx)
{	
    COMP_InitTypeDef COMP_InitStructure;
    GPIO_ConfigInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);

    /*比较器反向输入端为 3/4 Vref*/
    COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_IO2; 	        
    /*配置PA1为比较器输正向输入*/
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
**函数信息 ：unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx)    
**功能描述 ：COMP状态检查
**输入参数 ：
**输出参数 ：
**    备注 ：
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
