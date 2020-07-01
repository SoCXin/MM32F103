#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

u32 SysTick_Count;
u32 gDlyCnt;
char printBuf[100];
u32 timer1cnt;
u32 timer1ccr;

void InitSystick(void);
void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8* buf,u16 len);
void LED_Init(void);
void initGPIO_Timer8(void);
void initTIM8(u16 prescaler, u16 period, u16 dutyCycle);
void initNVIC_TIM8(void);
void OnTIM8(void);

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))

#define TIMPSC  0xFF
#define TIMARR  0xFFFF
#define TIMCCR  0x7FFF


/*����������TIM8�����·PWM��CH1N(PA_7) CH2N(PB14) CH3N(PB15)
 ������miniboard�� AIN5(PA_7)/MOSI(PB14)/MISO(PB15)���ӵ� LD3 �۲�LED��(��)*/

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{    
    SystemInit();
    InitSystick();
    
    uart_initwBaudRate(9600);
    LED_Init();
    UartSendGroup((u8*)printBuf, sprintf(printBuf,"TIM8 Test...\r\n"));
    
    initGPIO_Timer8();
    initTIM8(TIMPSC, TIMARR, TIMCCR);
    initNVIC_TIM8();
    OnTIM8();
    
    while(1){
        gDlyCnt = 1000;
        while(gDlyCnt);     
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"timercnt = 0x%x \r\n",timer1cnt));
    }
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
**������Ϣ ��void InitSystick()                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void InitSystick(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}


/********************************************************************************************************
**������Ϣ ��void SysTick_Handler()                         
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void SysTick_Handler()
{
    if(SysTick_Count++ > 500){
        SysTick_Count = 0;
        LED3_TOGGLE();                                                          
	}
    if(gDlyCnt > 0) gDlyCnt--;
    if(timer1ccr < TIMARR) timer1ccr ++;
    else timer1ccr = 0;
}


/********************************************************************************************************
**������Ϣ ��LED_Init(void)                        
**�������� ��LED��ʼ��
**������� ����
**������� ����
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
**������Ϣ ��void initGPIO_Timer8(void)                     
**�������� ��TIM8 GPIO��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void initGPIO_Timer8(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
    /*TIM8_CH1N*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_3);  
    /*TIM8_CH2N*/    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_3);  
    /*TIM8_CH3N*/    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_3);                       

    /*TIM8_CH1N*/                                                                          
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;                                 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*TIM8_CH2N / CH3N*/
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14 | GPIO_Pin_15;                  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/********************************************************************************************************
**������Ϣ ��void initTIM8(u16 prescaler, u16 period, u16 dutyCycle)                     
**�������� ��TIM8��ʼ��
**������� ��u16 prescaler, u16 period, u16 dutyCycle
**������� ����
********************************************************************************************************/
void initTIM8(u16 prescaler, u16 period, u16 dutyCycle)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
    TIM_StructInit.TIM_Period = period;
    TIM_StructInit.TIM_Prescaler = prescaler;
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_StructInit.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM8, &TIM_StructInit);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = dutyCycle;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM8, ENABLE);
    TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);
    
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

/********************************************************************************************************
**������Ϣ ��void initNVIC_TIM8()                  
**�������� ��TIM8 �жϳ�ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void initNVIC_TIM8(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /*Config TIM8_CC_IRQn NVIC config*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  
    /*PreemptionPriority : 1*/    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /*Enable IRQn*/
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             
    NVIC_Init(&NVIC_InitStructure);    
}

/********************************************************************************************************
**������Ϣ ��void TIM8_CC_IRQHandler()                   
**�������� ��TIM8 �жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void TIM8_CC_IRQHandler()
{
    if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET) {
        /*Clear the CC1 flag*/
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);                                
    }
    timer1cnt = TIM8->CNT;
    TIM_SetCompare1(TIM8, timer1ccr);
}

/********************************************************************************************************
**������Ϣ ��void OnTIM8(void)                     
**�������� ��
**������� ��
**������� ��
********************************************************************************************************/
void OnTIM8(void)
{
    TIM_Cmd(TIM8, ENABLE);
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

