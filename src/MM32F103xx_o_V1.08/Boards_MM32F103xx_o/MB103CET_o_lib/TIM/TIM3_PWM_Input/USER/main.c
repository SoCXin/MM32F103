#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
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
**������Ϣ ��int main (void)                          
**�������� ��ʹ�öŰ������Ӱ����ϵ�PB7��PA6(Ain4)
**������� ��
**������� ��
**���ú��� ��uart_printf("0x%x\r\n",Uart1RxTest(UART1));   uart_printf("0x%x",sizeof(u32));
********************************************************************************************************/
extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
extern u32 SystemCoreClock;	

/*PB7��Ϊ���벶�����ţ�����PB7��PA6*/
int main(void)
{
    delay_init();
    uart_initwBaudRate(9600);
    
    /*1KHZ����*/
    TIM3_PWM_Init(1000-1, SystemCoreClock/1000000-1);   	
    /*����ռ�ձ�*/    
    TIM_SetCompare1(TIM3, 200);												    
    
    /*pwm�����ʼ����1M��Ƶ�ʲ�׽*/
    TIM4_PWMINPUT_INIT(0xFFFF, SystemCoreClock/1000000-1);   				    
    
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"������PB7��PA6\r\n"));
    
    while(1){ 
        if(CollectFlag==1){
            /*��ӡռ�ձ�*/
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"duty  	= %d%% \r\n",duty * 100 / period));	
            /*��ӡ������һ�ֽз�*/
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"cycle 	= %dKHz\r\n",1000 / period));	
            /*��ӡ����*/            
			UartSendGroup((u8*)printBuf, sprintf(printBuf,"period  = %dus\r\n",period));			    
            CollectFlag = 0;
        }
        delay_ms(100);
    } 
}


/********************************************************************************************************
**������Ϣ ��void TIM3_PWM_Init(u16 arr,u16 psc)                       
**�������� ��TIM3 PWM��ʼ��
**������� ��u16 arr,u16 psc
**������� ��
**���ú��� ��
********************************************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
    
    //���ø�����Ϊ���������������,���TIM3 CH1  ��PWM���岨��
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;					              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 				          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
    
    /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ*/
    TIM_TimeBaseStructure.TIM_Period = arr; 
    /*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ*/    
    TIM_TimeBaseStructure.TIM_Prescaler =psc; 		
    /*����ʱ�ӷָ�:TDTS = Tck_tim*/    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	
    /*TIM���ϼ���ģʽ*/    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    /*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				            
       
    /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1*/       
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	
    /*�Ƚ����ʹ��*/    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    /*���ô�װ�벶��ȽϼĴ���������ֵ*/    
    TIM_OCInitStructure.TIM_Pulse = 0; 		
    /*�������:TIM����Ƚϼ��Ը�*/    
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			        
    
    /*����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx*/
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  		
    /*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/    
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  	               			
           
    /*ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���*/ 
    TIM_ARRPreloadConfig(TIM3, ENABLE); 				               	        
 
    /*ʹ��TIMx����*/ 
    TIM_Cmd(TIM3, ENABLE);  							                        
}


/********************************************************************************************************
**������Ϣ ��void TIM4_PWMINPUT_INIT(u16 arr,u16 psc)                         
**�������� ��PWM�����ʼ��
**������� ��
**������� ��
**���ú��� ��
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
    
    /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ*/
    TIM_TimeBaseStructure.TIM_Period = arr; 	
    /*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/     
    TIM_TimeBaseStructure.TIM_Prescaler =psc; 		
    /*����ʱ�ӷָ�:TDTS = Tck_tim*/    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
    /*TIM���ϼ���ģʽ*/    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    /*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				            
    
    /*�����ж����ȼ�*/
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
    /*PWM��������*/
    TIM_PWMIConfig(TIM4, &TIM4_ICInitStructure);     				             
     
    /*ѡ����Ч�����*/     
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2); 
    /*����Ϊ���Ӹ�λģʽ*/    
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);  				            
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);       
    /*�ж�����*/    
    TIM_ITConfig(TIM4, TIM_IT_CC2|TIM_IT_Update, ENABLE);     
    /*����жϱ�־λ*/    
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update); 			            
    TIM_Cmd(TIM4, ENABLE);    
}

/********************************************************************************************************
**������Ϣ ��void TIM4_IRQHandler(void)                       
**�������� ��TIM4�жϺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void TIM4_IRQHandler(void)
{
    /*����1���������¼�*/
    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)			                    
    {	
        /*�ɼ�ռ�ձ�*/
        duty = TIM_GetCapture1(TIM4); 		
        /*�ɼ�����*/        
        period	=	TIM_GetCapture2(TIM4);     				                    
        CollectFlag = 1;
        
    }
    /*����жϱ�־λ*/
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update);                      
}


/********************************************************************************************************
**������Ϣ ��void delay_init(void)                         
**�������� ����ʼ���ӳٺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void delay_init(void)
{
    /*ѡ���ⲿʱ��  HCLK/8*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /*Ϊϵͳʱ�ӵ�1/8*/    
    fac_us=SystemCoreClock/8000000;				                               
    /*��OS��,����ÿ��ms��Ҫ��systickʱ���� */
    fac_ms=(u16)fac_us*1000;					                              
}


/********************************************************************************************************
**������Ϣ ��void delay_ms(u16 nms)    
**�������� ����ʱnms
**������� ��nms
**������� ��
**    ��ע ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nms<=0xffffff*8*1000/SYSCLK,72M������,nms<=1864 
********************************************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    /*ʱ�����(SysTick->LOADΪ24bit)*/    
    SysTick->LOAD=(u32)nms*fac_ms;
    /*��ռ�����*/    
    SysTick->VAL =0x00;	
    /*��ʼ����*/    
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
    do{
        temp=SysTick->CTRL;
    }
    /*�ȴ�ʱ�䵽��*/
    while((temp&0x01)&&!(temp&(1<<16)));		                            
    /*�رռ�����*/
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
    /*��ռ�����*/    
    SysTick->VAL =0X00;       					                               
}

/********************************************************************************************************
**������Ϣ ��void uart_initwBaudRate(u32 bound)     
**�������� ��UART��ʼ��
**������� ��bound
**������� ��
**    ��ע ��
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
    /*GPIO�˿�����*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /*UART1_TX   GPIOA.9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*�����������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                        
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    
    /*UART1_RX  GPIOA.10��ʼ��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    /*��������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	                 
    GPIO_Init(GPIOA, &GPIO_InitStructure);				                     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    
    /*UART ��ʼ������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                   

    /*���ڲ�����*/
    UART_InitStructure.UART_BaudRate = bound;
    /*�ֳ�Ϊ8λ���ݸ�ʽ*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /*һ��ֹͣλ*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /*����żУ��λ*/    
    UART_InitStructure.UART_Parity = UART_Parity_No;
    /*��Ӳ������������*/    
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    /*�շ�ģʽ*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                
    /*��ʼ������1*/
    UART_Init(UART1, &UART_InitStructure); 	
    /*�������ڽ����ж�*/    
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    /*ʹ�ܴ���1*/    
    UART_Cmd(UART1, ENABLE);                    			                    
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

