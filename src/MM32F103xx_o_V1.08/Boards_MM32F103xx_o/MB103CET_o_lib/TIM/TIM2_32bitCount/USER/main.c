#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];
/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0; 

extern u32 SystemCoreClock;	
void delay_init(void);
void uart_initwBaudRate(u32 bound);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void Tim2_UPCount_test(u16 Prescaler,u32 Period);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：本例主要测试32位定时器功能，串口一直打印输出TIM2 CNT值
**输入参数 ：void 
**输出参数 ：void 
********************************************************************************************************/
unsigned int ucTim2Flag;
u32 getCCRvalue = 0;
u32 getCNTvalue = 0;
  
int main(void)
{
    delay_init();
    uart_initwBaudRate(9600);	 
    /*Test write ARR 32bit*/    
    Tim2_UPCount_test(SystemCoreClock/0x1FFFF,0x1FFFF);  
    /*Test write CCRx 32bit*/    
    TIM_SetCompare1(TIM2, 0x10000);                                             
    getCCRvalue = TIM_GetCapture1(TIM2);                                        
                                                                                
    while(1)                                                                   
    {
        delay_ms(100);
        getCNTvalue = TIM_GetCounter(TIM2);
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"TIM2 Value: 0x%X  \n",getCNTvalue));
    } 
}


/********************************************************************************************************
**函数信息 ：void TIM2_IRQHandler(void)                  
**功能描述 ：定时器2中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    /*必须手动清除中断标志位*/
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /*全局标志*/
    ucTim2Flag++;       
}

/********************************************************************************************************
**函数信息 ：void Tim2_UPCount_test(u16 Prescaler,u32 Period)                 
**功能描述 ：配置定时器2向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler,u32 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    NVIC_InitTypeDef NVIC_StructInit;
    
    /*使能TIM1时钟,默认时钟源为PCLK1(PCLK1未分频时不倍频,否则由PCLK1倍频输出),可选其它时钟源*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /*ARR寄存器值*/
    TIM_StructInit.TIM_Period=Period;   
    /*预分频值*/    
    TIM_StructInit.TIM_Prescaler=Prescaler;                                     
    /*数字滤波器采样频率,不影响定时器时钟*/  
    /*采样分频值*/    
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;   
    /*计数模式*/    
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                          
    TIM_StructInit.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM2, &TIM_StructInit);
    
    /*允许定时器2更新中断*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM2, ENABLE);	
    
    /*更新定时器时会产生更新时间,清除标志位*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    
    /*配置定时器2中断通道及优先级 */
    NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_StructInit.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_StructInit.NVIC_IRQChannelSubPriority=1;
    
    NVIC_Init(&NVIC_StructInit);
    
    
}
/********************************************************************************************************
**函数信息 ：Tim2_UPStatusOVCheck_test(void)                       
**功能描述 ：等待定时器溢出
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Tim2_UPStatusOVCheck_test(void)
{
    /*等待事件更新标志位*/
    while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET);
    /*清事件标志位*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
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

