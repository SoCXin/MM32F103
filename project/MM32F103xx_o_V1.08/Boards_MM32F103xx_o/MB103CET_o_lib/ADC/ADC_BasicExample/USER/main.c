#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

u16 ADCVAL;
/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0;                                                          
extern u32 SystemCoreClock;
char printBuf[100];

void delay_init(void);
void uart_initwBaudRate(u32 bound);
void ADC1_SingleChannel(uint8_t ADC_Channel_x);
void GPIO_Configuration(void);
u16 Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times);
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x);
void delay_ms(u16 nms);
void UartSendGroup(u8* buf,u16 len);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
int main(void)
{    
    float fValue;
    delay_init();	    	    	 
    uart_initwBaudRate(9600);	     
    
    
    /*����ADC1Ϊ����ת��ģʽ,ͨ��1ʹ��*/
    ADC1_SingleChannel( ADC_Channel_1);
    while(1)
    {
        ADCVAL=Get_Adc_Average(ADC_Channel_1,5);
        fValue = ((float)ADCVAL/4095)*3.3;
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"ADC1_CH_1=%.2fV\r\n",fValue));
        delay_ms(1000);  
    }
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
**������Ϣ ��void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**�������� ������ADC1����ת��ģʽ
**������� ��ADC_Channel_x , xΪ0~8
**������� ����
**    ��ע ��
********************************************************************************************************/
void ADC1_SingleChannel(uint8_t ADC_Channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;
    
    
    GPIO_Configuration();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    /*ADC1 ͨ��1����PA1*/
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*��������ͨ��*/
    ADC_RegularChannelConfig(ADC1, 9, 0, 0); 
    /*ʹ��ѡ��ͨ��,�����������*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, 0); 
    
    ADC_Cmd(ADC1, ENABLE); 
    
}


/********************************************************************************************************
**������Ϣ ��void GPIO_Configuration(void)     
**�������� ��GPIO����
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/********************************************************************************************************
**������Ϣ ��Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)     
**�������� ����ȡ����ADC1����ֵ��ƽ��ֵ
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
{
    u32 temp_val=0;
    u8 t;
    u8 delay;
    for(t=0;t<times;t++)
    {
        temp_val+=ADC1_SingleChannel_Get(ADC_Channel_x);
        for(delay=0;delay<100;delay++);
    }
    return temp_val/times;
} 


/********************************************************************************************************
**������Ϣ ��ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)       
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
{ 
    u16 puiADData;
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    
    while(!(ADC1->ADSTA&ADC_FLAG_EOC));
    
    puiADData=ADC1->ADDATA&0xfff;
    
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /*ADCR�Ĵ�����ADSTλʧ�ܣ����ת������*/
    
    return puiADData;
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
