#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void I2CMasterTest(I2C_TypeDef *I2Cx);
void UartSendGroup(u8* buf,u16 len);
void uart_initwBaudRate(u32 bound);

unsigned char chI2c_Txdata[10]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa};
unsigned char chI2c_Rxdata[10]={0,0,0,0,0,0,0,0};
char printBuf[100];

#define FLASH_DEVICE_ADDR 0xa0

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{    
    uart_initwBaudRate(9600);
    /*I2C�շ����ݣ�ֻ���޸Ĳ����Ϳ�ѡ������I2C*/    
    I2CMasterTest(I2C1);				                                        
                                                                                
    while(1)              				                                     
    {
        
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
**������Ϣ ��void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned int uiI2C_speed) //unit is Khz                    
**�������� ����ʼ��I2C
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned int uiI2C_speed) 
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    if(I2Cx==I2C1)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  	
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_4);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_4);
    }
    if(I2Cx==I2C2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); 
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_4);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_4);
    }
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
    I2C_InitStructure.I2C_OwnAddress = 0;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = uiI2C_speed;    
    
    I2C_Init(I2Cx, &I2C_InitStructure);    
    I2C_Cmd(I2Cx, ENABLE);    
}

/********************************************************************************************************
**������Ϣ ��I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)          
**�������� ��ѡ��ӻ���ַ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2;deviceaddr:�ӻ���ַ
**������� ����
********************************************************************************************************/
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)  
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    if(I2Cx==I2C1)
    {
        /*I2C1��ӳ��IO��*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;                 
    }
    if(I2Cx==I2C2)
    {
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;   	
    }
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    I2C_Cmd(I2Cx,DISABLE);
    I2C_Send7bitAddress(I2Cx, deviceaddr , I2C_Direction_Transmitter);	
    I2C_Cmd(I2Cx, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**�������� ����鷢���жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
{
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE))
        {
            break;
        }
    }
}
/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**�������� ���������жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{
    
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_RFNE))
        {
            break;
        }	
    }
}
/********************************************************************************************************
**������Ϣ ��I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)              
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��cmd��temp
**������� ����
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char data)
{
    I2C_SendData(I2Cx, data);
    while(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE)==0);
}

/********************************************************************************************************
**������Ϣ ��I2CRXByte(I2C_TypeDef *I2Cx)             
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ��temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFNF) == 1)			        
        {
            I2C_ReadCmd(I2Cx);
        }
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_RFNE))
        {
            break;
        }
    }
    return  I2C_ReceiveData(I2Cx);
}


/********************************************************************************************************
**������Ϣ ��I2CRXGroup()             
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),u16 rx_count(���ݸ���),u8 *data_buf(��������ָ��)
**������� ��temp
********************************************************************************************************/
void I2CRXGroup_DMA(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf)
{
    DMA_InitTypeDef  DMA_InitStructure;
    
    /* Enable DMA1 clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    /* DMA1 channel5 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel7);
    /*I2C���ݽ��ռĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32) &(I2C1->IC_DATA_CMD);
    /*���յ������ݴ�ŵ����ݻ���ַ*/    
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)chI2c_Rxdata;	
    /*���ݴ����赽memory*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				            
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    /*����M2Mģʽ*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;				                
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    
    /*I2C����DMAʹ��*/
    I2C1->IC_DMA_CR=0x1;							                            
    DMA_Cmd(DMA1_Channel7, ENABLE);
    /* DMA1 Channel5 transfer complete test */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC7))
    {
        /*���������ʡ��*/
        I2C_ReadCmd(I2C1);							                            
    }
}

/********************************************************************************************************
**������Ϣ ��void E2PROM_Write(	I2C_TypeDef *I2Cx,\unsigned short mem_byte_addr, \ unsigned short tx_count, \ unsigned char *tx_data  )           
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM��ַ),
			 unsigned short tx_count(���ݳ���),unsigned char *tx_data(����bufָ��)
**������� ����
********************************************************************************************************/
void E2PROM_Write(	I2C_TypeDef *I2Cx,\
    unsigned short mem_byte_addr, \
        unsigned short tx_count, \
            unsigned char *tx_data  )
{
    unsigned short i;
    unsigned char *p;
    p = tx_data;
    
    /*����Ҫ�������ݵĵ�ַ*/
    I2CTXByte(I2Cx,mem_byte_addr);						                        
    for(i=0;i<tx_count;i++)                                                     
    {                                                                           
        /*��������*/      
        I2CTXByte(I2Cx,*p); 							                        
        p++;
    }
    
    I2C_GenerateSTOP(I2C1,ENABLE);
    /*���stop�ź�*/
    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);                         
}

/********************************************************************************************************
**������Ϣ ��I2CMasterRead()           
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM��ַ)
             unsigned char rx_count(���ݳ���),unsigned char *rx_data(����bufָ��)
**������� ����
********************************************************************************************************/
void  E2PROM_Read(	I2C_TypeDef *I2Cx,\
    unsigned short mem_byte_addr, \
        unsigned char rx_count, \
            unsigned char *rx_data )
{
    /*���stop�ź�*/
    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);                            
    
    I2CTXByte(I2Cx,mem_byte_addr);
    I2CRXGroup_DMA(I2Cx,rx_count,rx_data);
    I2C_GenerateSTOP(I2C1,ENABLE);
    
}

/********************************************************************************************************
**������Ϣ ��I2CMasterTest(I2C_TypeDef *I2Cx)          
**�������� ��I2C���շ������ݲ��Գ���
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CMasterTest(I2C_TypeDef *I2Cx)
{
    unsigned int i;

	UartSendGroup((u8*)printBuf, sprintf(printBuf,"i2c test start ...\r\n"));	
    
    I2CInitMasterMode(I2Cx,100000);
    I2CSetDeviceAddr(I2Cx,FLASH_DEVICE_ADDR);
    
    /*��EEPROM��ַ0д10����*/
    E2PROM_Write( I2Cx,0, 10, chI2c_Txdata );	                                
    
    /*eeprom ���������ʱ*/
    i =80000;while(i--);						                                
    
    /*��д������ݶ�����*/
    E2PROM_Read( I2Cx,0, 10, chI2c_Rxdata );	                                
    
    for(i = 0; i <10 ;i ++)
    {
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"Rx[%d]: %x \r\n", i,chI2c_Rxdata[i]));
    }
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"i2c test over !!!\r\n"));
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

