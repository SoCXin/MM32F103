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
**函数信息 ：int main (void)                          
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
int main(void)
{
    unsigned int uiCnt=0;                                                     
    uart_initwBaudRate(9600);	                                              
                                                                              
    /*PCLK2时钟(96MHz)9600分频,计数器值为10000,即1s溢出一次*/                 
    Tim1_UPCount_test(SystemCoreClock/10000-1,10000-1);                       
    while(1)                                                                  
    {
        for(uiCnt=0; ;uiCnt++)
        {
            /*等待定时器溢出*/
            Tim1_UPStatusOVCheck_test();
            
            /*打印当前数值*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"0x%x\r\n",uiCnt));  
        }
    }	
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
**函数信息 ：void Tim1_UPCount_test1(u16 Period,u16 Prescaler)                     
**功能描述 ：配置定时器1向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void Tim1_UPCount_test(u16 Prescaler,u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    
    /*使能TIM1时钟,默认时钟源为PCLK2(PCLK2未分频时不倍频,否则由PCLK2倍频输出),可选其它时钟源*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    /*ARR寄存器值*/
    TIM_StructInit.TIM_Period=Period; 
    /*预分频值*/    
    TIM_StructInit.TIM_Prescaler=Prescaler;                                     
    /*数字滤波器采样频率,不影响定时器时钟*/
    /*采样分频值*/
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;  
    /*计数模式*/    
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Down;                        
    TIM_StructInit.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM1, &TIM_StructInit);
    
    TIM_Cmd(TIM1, ENABLE);	
    
    /*更新定时器时会产生更新时间,清除标志位*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}


/********************************************************************************************************
**函数信息 ：Tim1_UPStatusOVCheck_test1(void)                       
**功能描述 ：等待定时器溢出
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Tim1_UPStatusOVCheck_test(void)
{
    /*等待事件更新标志位*/
    while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==RESET);
    /*清事件标志位*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
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

