#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0;
char printBuf[100];

void delay_init(void);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void uart_initwBaudRate(u32 bound);
void LED_Init(void);
u8 FLASH_Program(void);

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))

#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08002800)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08003200)
uint32_t Data = 0x12345679;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

/********************************************************************************************************
**函数信息 ：main(void)                     
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
**    备注 ：下载程序前要选择全擦除
********************************************************************************************************/
int main(void)    
{ 
    u8 t; 
    delay_init();
    LED_Init();
    uart_initwBaudRate(9600);
    t=FLASH_Program();
    
    if(t==0)
        while(1)              
        {
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"program ok\r\n"));
            LED1_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(1000);
        }
    else
        while(1)
        {
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"program error\r\n"));
            delay_ms(1000);
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
**函数信息 ：u8 FLASH_Program(void)    
**功能描述 ：Program FLASH Bank1
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
u8 FLASH_Program(void)
{
         
    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH_Unlock();
    
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    
    FLASH_ErasePage(BANK1_WRITE_START_ADDR);
    
    FLASH_ClearFlag(FLASH_FLAG_EOP );	
    
    FLASHStatus = FLASH_ProgramWord(0x08001FFC, Data);
    
    FLASH_ClearFlag(FLASH_FLAG_EOP );	
    
    FLASH_Lock();
    
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"%x\r\n",(*(__IO uint32_t*) 0x08001FFC)));
    
    if((*(__IO uint32_t*) 0x08001FFC) != Data)
    {
        return 1;
    }
    return 0;
}

/********************************************************************************************************
**函数信息 ：void delay_init(void)                         
**功能描述 ：初始化延迟函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void delay_init(void)
{
    /*选择外部时钟  HCLK/8*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /*为系统时钟的1/8*/    
    fac_us=SystemCoreClock/8000000;				                               
    /*非OS下,代表每个ms需要的systick时钟数 */
    fac_ms=(u16)fac_us*1000;					                              
}


/********************************************************************************************************
**函数信息 ：void delay_ms(u16 nms)    
**功能描述 ：延时nms
**输入参数 ：nms
**输出参数 ：
**    备注 ：SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK,72M条件下,nms<=1864 
********************************************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    /*时间加载(SysTick->LOAD为24bit)*/    
    SysTick->LOAD=(u32)nms*fac_ms;
    /*清空计数器*/    
    SysTick->VAL =0x00;	
    /*开始倒数*/    
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
    do{
        temp=SysTick->CTRL;
    }
    /*等待时间到达*/
    while((temp&0x01)&&!(temp&(1<<16)));		                            
    /*关闭计数器*/
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
    /*清空计数器*/    
    SysTick->VAL =0X00;       					                               
}

/********************************************************************************************************
**函数信息 ：LED_Init(void)                        
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_OFF();
    LED3_OFF();
    LED4_OFF();
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
