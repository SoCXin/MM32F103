#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*定义最大接收字节数 200*/
#define UART_REC_LEN  			200     
/*使能（1）/禁止（0）串口1接收*/
#define EN_UART1_RX 			1		                                        

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))
         
/*接收状态标记*/         
u16 UART_RX_STA=0; 
/*接收缓冲,最大UART_REC_LEN个字节*/
u8 UART_RX_BUF[UART_REC_LEN];     		                                        
char printBuf[100];                                                     
/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0;

void delay_init(void);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void uart_nvic_init(u32 bound);
void LED_Init(void);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，串口发送数据，并等待上位机发送信息，然后再打印出来
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{	
    u8 t;
    u8 len;	
    u16 times=0;
    
    /*延时函数初始化*/    
    delay_init();	    				 			                            	
    LED_Init();
    /*设置中断优先级分组2*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    /*串口初始化为9600*/    
    uart_nvic_init(9600);	 						                            
    while(1)
    {
        if(UART_RX_STA&0x8000)
        {
            /*得到此次接收到的数据长度*/            
            len=UART_RX_STA&0x3fff;						                        
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\n您发送的消息为:\r\n"));
            for(t=0;t<len;t++)
            {
                while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET);
                UART_SendData(UART1, (u8)UART_RX_BUF[t]);
            }
            /*插入换行*/
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\n\r\n"));         
            UART_RX_STA=0;
        }else
        {
            times++;
            if(times%5000==0)
            {
                UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\nMini Board 串口实验\r\n"));
            }
            if(times%200==0) UartSendGroup((u8*)printBuf, sprintf(printBuf,"请输入数据,以回车键结束\r\n")); 
            /*闪烁LED,提示系统正在运行*/
            if(times%30==0)LED1_TOGGLE();				                        
            delay_ms(5);   
        }
    }	 
}


/********************************************************************************************************
**函数信息 ：void uart_nvic_init(u32 bound)    
**功能描述 ：UART端口、中断初始化
**输入参数 ：bound
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void uart_nvic_init(u32 bound)
{
    /*GPIO端口设置*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*使能UART1，GPIOA时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);       

    /*UART1 NVIC 配置*/     
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;    
    /*抢占优先级3*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;      
    /*子优先级3*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    /*IRQ通道使能*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    /*根据指定的参数初始化VIC寄存器*/    
    NVIC_Init(&NVIC_InitStructure);					                            
    
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
}

/********************************************************************************************************
**函数信息 ：void UART1_IRQHandler(void)    
**功能描述 ：串口1中断服务程序
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UART1_IRQHandler(void)                	
{
    u8 Res;
    /*接收中断(接收到的数据必须是0x0d 0x0a结尾)*/
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET)  		                
    {
        UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        /*读取接收到的数据*/
        Res =UART_ReceiveData(UART1);					                        
        if((Res==0x0d)&&((UART_RX_STA&0X3FFF)>0))
        {
            UART_RX_STA|=0x4000;
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        /*接收到了0x0d*/
        else if((UART_RX_STA&0x4000)&&((UART_RX_STA&0X3FFF)>0))                 
        {
            if(Res==0x0a)
            {
                UART_RX_STA|=0x8000;				
            }
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else{
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
            UART_RX_STA=UART_RX_STA&0X3FFF;
            if((UART_RX_STA)>(UART_REC_LEN-1))
                /*接收数据错误,重新开始接收*/
                UART_RX_STA=0;						                            
        }       
    }    
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
    
    LED1_ON();
    LED3_ON();
    LED4_ON();
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
