#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0; 
char printBuf[100];

void delay_init(void);
void uart_initwBaudRate(u32 bound);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void LED_Init(void);
void WKUP_Init(void);
u8 Check_WKUP(void);
void Sys_Standby(void);

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，将PA0和GND短接
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{ 
    /*延时函数初始化*/
    delay_init();	    
    /*设置中断优先级分组2*/    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    /*串口初始化为9600*/    
    uart_initwBaudRate(9600);	 						                        
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"请按下wake up按键3秒不放\r\n"));
    /*初始化与LED连接的硬件接口*/
    LED_Init();		  									                        
    /*初始化WK_UP按键，同时检测是否正常开机*/
    WKUP_Init();										                        
    while(1)                                                                    
    {                                                                           
        LED1_TOGGLE();                                                          
        LED3_TOGGLE();                                                          
        LED4_TOGGLE();
        /*延时250ms*/        
        delay_ms(250);									                        
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"已从standby模式唤醒\r\n"));
    }											    
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
**函数信息 ：WKUP_Init(void)                         
**功能描述 ：外部中断唤醒待机初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void WKUP_Init(void)
{	
    GPIO_InitTypeDef  GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /*使能GPIOA时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);					    
        
    /*PA.0*/    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	
    /*下拉输入*/    
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;							    
    GPIO_Init(GPIOA, &GPIO_InitStructure);									  
    /*使用外部中断方式*/  
    /*中断线0连接GPIOA.0*/    
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);			    
             
    /*设置按键所有的外部线路*/    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    /*设外外部中断模式:EXTI线路为中断请求*/    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    /*上升沿触发*/    
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  				    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    /* 初始化外部中断*/    
    EXTI_Init(&EXTI_InitStructure);											    
                     
    /*使能按键所在的外部中断通道*/                     
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
    /*先占优先级2级*/    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    /*从优先级2级*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 		
    /*使能外部中断通道*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    /*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/    
    NVIC_Init(&NVIC_InitStructure); 										    
                                      
    /*不是开机,进入待机模式*/                                      
    if(Check_WKUP()==0) Sys_Standby();    									    
}

/********************************************************************************************************
**函数信息 ：Check_WKUP(void)                         
**功能描述 ：判断按键是否按下
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
u8 Check_WKUP(void) 
{
    /*记录按下的时间*/
    u8 t=0;	
    LED1_ON();
    LED3_ON();
    LED4_ON();
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            /*已经按下了*/
            t++;					                                             
            delay_ms(30); 
            /*按下超过3秒钟*/            
            if(t>=100)				                                            
            {                                                                   
                LED1_ON();                                                      
                LED3_ON();                                                      
                LED4_ON();  
                /*按下3s以上了*/                
                return 1; 			                                            
            }                                                                   
        }else                                                                   
        {                                                                       
            LED1_OFF();                                                         
            LED3_OFF();                                                         
            LED4_OFF();  
            /*按下不足3秒*/            
            return 0; 				                                            
        }                                                                       
    }
} 

/********************************************************************************************************
**函数信息 ：Sys_Standby(void)                         
**功能描述 ：进入待机模式
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Sys_Standby(void)
{  
    /*使能PWR外设时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	 
    /*复位所有IO口*/    
    RCC->APB2RSTR|=0X01FC;
    /*使能唤醒管脚功能*/    
    PWR_WakeUpPinCmd(ENABLE);  
    /*进入待命（STANDBY）模式*/    
    PWR_EnterSTANDBYMode();	  							                        
    
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

