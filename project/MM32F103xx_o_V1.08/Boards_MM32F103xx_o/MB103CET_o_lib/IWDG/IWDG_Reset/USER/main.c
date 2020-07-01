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
**函数信息 ：int main (void)                          
**功能描述 ：若不在死循环中喂狗，系统一直复位，不断打印uart ok！；反之则只上电打印一次
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(9600);	 			                
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"uart ok!\r\n"));
    /*配置为LSI32分频,计数器初始值为0x7ff,复位时长约为1.6s*/
    Write_Iwdg_ON(IWDG_Prescaler_32,0xf);
    while(1)								
    {
       /*无复位程序进入死循环,系统一直打印串口数据*/
       Write_Iwdg_RL();     				                                    
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
**函数信息 ：Write_Iwdg_PR(void)             
**功能描述 ：启动独立看门狗
**输入参数 ：IWDG_Prescaler 可选IWDG_Prescaler_X, X为4,8,16,32,64,128,256,对应分频值与X取值相同
**输出参数 ：无
**    备注 ：Reload<=0xfff,为计数器重载值  复位时常计算已LSI 40KHz为参考  Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*启动内部低速时钟,等待时钟就绪*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    
    /*设置时钟预分频*/	
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    
    /*设置重载寄存器值*/	
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload&0xfff);	
    
    /*装载并使能计数器*/	
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**函数信息 ：PVU_CheckStatus(void)                       
**功能描述 ：检查独立看门狗预分频位状态
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*检查预分频位状态,为RESET才可改变预分频值*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)                                                       
        {
            break;
        }
    }
}

/********************************************************************************************************
**函数信息 ：RVU_CheckStatus(void)                  
**功能描述 ：检查独立看门狗重载标志
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*检查重载标志状态*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  
        {
            break;
        }
    }
}

/********************************************************************************************************
**函数信息 ：void Write_Iwdg_RL(void)           
**功能描述 ：喂狗函数
**输入参数 ：
**输出参数 ：无
********************************************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
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

