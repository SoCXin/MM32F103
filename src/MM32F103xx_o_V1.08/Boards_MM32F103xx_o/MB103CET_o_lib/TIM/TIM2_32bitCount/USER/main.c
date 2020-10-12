#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

char printBuf[100];
/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0; 

extern u32 SystemCoreClock;	
void delay_init(void);
void uart_initwBaudRate(u32 bound);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);
void Tim2_UPCount_test(u16 Prescaler,u32 Period);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������Ҫ����32λ��ʱ�����ܣ�����һֱ��ӡ���TIM2 CNTֵ
**������� ��void 
**������� ��void 
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
**������Ϣ ��void TIM2_IRQHandler(void)                  
**�������� ����ʱ��2�жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    /*�����ֶ�����жϱ�־λ*/
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /*ȫ�ֱ�־*/
    ucTim2Flag++;       
}

/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test(u16 Prescaler,u32 Period)                 
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler,u32 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    NVIC_InitTypeDef NVIC_StructInit;
    
    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK1(PCLK1δ��Ƶʱ����Ƶ,������PCLK1��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /*ARR�Ĵ���ֵ*/
    TIM_StructInit.TIM_Period=Period;   
    /*Ԥ��Ƶֵ*/    
    TIM_StructInit.TIM_Prescaler=Prescaler;                                     
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/  
    /*������Ƶֵ*/    
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;   
    /*����ģʽ*/    
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                          
    TIM_StructInit.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM2, &TIM_StructInit);
    
    /*����ʱ��2�����ж�*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM2, ENABLE);	
    
    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    
    /*���ö�ʱ��2�ж�ͨ�������ȼ� */
    NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_StructInit.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_StructInit.NVIC_IRQChannelSubPriority=1;
    
    NVIC_Init(&NVIC_StructInit);
    
    
}
/********************************************************************************************************
**������Ϣ ��Tim2_UPStatusOVCheck_test(void)                       
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim2_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
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

