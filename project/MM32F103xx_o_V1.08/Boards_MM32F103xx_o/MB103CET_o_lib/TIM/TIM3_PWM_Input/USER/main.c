#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0;
char printBuf[100];
u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 0;

void delay_init(void);
void uart_initwBaudRate(u32 bound);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWMINPUT_INIT(u16 arr,u16 psc);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：使用杜邦线连接板子上的PB7与PA6(Ain4)
**输入参数 ：
**输出参数 ：
**常用函数 ：uart_printf("0x%x\r\n",Uart1RxTest(UART1));   uart_printf("0x%x",sizeof(u32));
********************************************************************************************************/
extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
extern u32 SystemCoreClock;	

/*PB7作为输入捕获引脚，连接PB7与PA6*/
int main(void)
{
    delay_init();
    uart_initwBaudRate(9600);
    
    /*1KHZ周期*/
    TIM3_PWM_Init(1000-1, SystemCoreClock/1000000-1);   	
    /*设置占空比*/    
    TIM_SetCompare1(TIM3, 200);												    
    
    /*pwm输入初始化以1M的频率捕捉*/
    TIM4_PWMINPUT_INIT(0xFFFF, SystemCoreClock/1000000-1);   				    
    
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"请连接PB7与PA6\r\n"));
    
    while(1){ 
        if(CollectFlag==1){
            /*打印占空比*/
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"duty  	= %d%% \r\n",duty * 100 / period));	
            /*打印周期另一种叫法*/
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"cycle 	= %dKHz\r\n",1000 / period));	
            /*打印周期*/            
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"period  = %dus\r\n",period));			    
            CollectFlag = 0;
        }
        delay_ms(100);
    } 
}


/********************************************************************************************************
**函数信息 ：void TIM3_PWM_Init(u16 arr,u16 psc)                       
**功能描述 ：TIM3 PWM初始化
**输入参数 ：u16 arr,u16 psc
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
    
    //设置该引脚为复用推挽输出功能,输出TIM3 CH1  的PWM脉冲波形
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;					              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 				          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
    
    /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值*/
    TIM_TimeBaseStructure.TIM_Period = arr; 
    /*设置用来作为TIMx时钟频率除数的预分频值  不分频*/    
    TIM_TimeBaseStructure.TIM_Prescaler =psc; 		
    /*设置时钟分割:TDTS = Tck_tim*/    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	
    /*TIM向上计数模式*/    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				            
       
    /*选择定时器模式:TIM脉冲宽度调制模式1*/       
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	
    /*比较输出使能*/    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    /*设置待装入捕获比较寄存器的脉冲值*/    
    TIM_OCInitStructure.TIM_Pulse = 0; 		
    /*输出极性:TIM输出比较极性高*/    
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			        
    
    /*根据TIM_OCInitStruct中指定的参数初始化外设TIMx*/
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  		
    /*使能TIMx在CCR1上的预装载寄存器*/    
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  	               			
           
    /*使能TIMx在ARR上的预装载寄存器*/ 
    TIM_ARRPreloadConfig(TIM3, ENABLE); 				               	        
 
    /*使能TIMx外设*/ 
    TIM_Cmd(TIM3, ENABLE);  							                        
}


/********************************************************************************************************
**函数信息 ：void TIM4_PWMINPUT_INIT(u16 arr,u16 psc)                         
**功能描述 ：PWM输入初始化
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void TIM4_PWMINPUT_INIT(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef 		 NVIC_InitStructure;
    TIM_ICInitTypeDef        TIM4_ICInitStructure;
    GPIO_InitTypeDef         GPIO_InitStructure;
    
    /*Open TIM4 clock*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);     		
    /*open gpioB clock*/    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  			            
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);
    
    /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值*/
    TIM_TimeBaseStructure.TIM_Period = arr; 	
    /*设置用来作为TIMx时钟频率除数的预分频值*/     
    TIM_TimeBaseStructure.TIM_Prescaler =psc; 		
    /*设置时钟分割:TDTS = Tck_tim*/    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
    /*TIM向上计数模式*/    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				            
    
    /*配置中断优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;                     
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;                   
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM4_ICInitStructure.TIM_ICFilter = 0x0; 
    
    //TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    /*PWM输入配置*/
    TIM_PWMIConfig(TIM4, &TIM4_ICInitStructure);     				             
     
    /*选择有效输入端*/     
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2); 
    /*配置为主从复位模式*/    
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);  				            
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);       
    /*中断配置*/    
    TIM_ITConfig(TIM4, TIM_IT_CC2|TIM_IT_Update, ENABLE);     
    /*清除中断标志位*/    
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update); 			            
    TIM_Cmd(TIM4, ENABLE);    
}

/********************************************************************************************************
**函数信息 ：void TIM4_IRQHandler(void)                       
**功能描述 ：TIM4中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void TIM4_IRQHandler(void)
{
    /*捕获1发生捕获事件*/
    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)			                    
    {	
        /*采集占空比*/
        duty = TIM_GetCapture1(TIM4); 		
        /*采集周期*/        
        period	=	TIM_GetCapture2(TIM4);     				                    
        CollectFlag = 1;
        
    }
    /*清除中断标志位*/
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update);                      
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

