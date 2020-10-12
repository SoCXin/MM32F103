#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void uart_initwBaudRate(u32 bound);
void I2CMasterTest(I2C_TypeDef *I2Cx);
void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned int uiI2C_speed);
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr);
void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data );
void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data );
void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char temp);
void I2CRXGroup(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf);
void UartSendGroup(u8* buf,u16 len);
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx);
void I2CRXFullCheck(I2C_TypeDef *I2Cx);

#define FLASH_DEVICE_ADDR 0xa0
char printBuf[100];

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴��ڴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{

    uart_initwBaudRate(9600);
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
**������Ϣ ��I2CMasterTest(I2C_TypeDef *I2Cx)          
**�������� ��I2C���շ������ݲ��Գ���
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CMasterTest(I2C_TypeDef *I2Cx)
{
    unsigned int i;
    unsigned char tx_buffer0[9] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22,0};
    unsigned char rx_buffer0[8] ;
    
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"i2c test start ...\r\n"));	
    
    I2CInitMasterMode(I2Cx, 100000);  
    I2CSetDeviceAddr(I2Cx,FLASH_DEVICE_ADDR);
    I2CMasterWrite(I2Cx,0, 9, tx_buffer0);
    /*e2prom ���������ʱ*/
    i = 80000;while(i--);	
    
    I2CMasterRead(I2Cx,0, 8, rx_buffer0);
    
    for(i = 0; i <8 ;i ++)
    {
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"rx_buffer0[%d]: %x \r\n", i,rx_buffer0[i]));
    }
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"i2c test over !!!\r\n"));
}


/********************************************************************************************************
**������Ϣ ��void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned int uiI2C_speed)  //unit is Khz                    
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
    if(I2Cx == I2C1)
    {
        /*I2C1��ӳ��IO��*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;                 
    }
    if(I2Cx == I2C2)
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
**������Ϣ ��void I2CRelase(I2C_TypeDef *I2Cx,unsigned char deviceaddr)           
**�������� ��ѡ��ӻ���ַ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2;deviceaddr:�ӻ���ַ
**������� ����
********************************************************************************************************/
void I2CRelase(I2C_TypeDef *I2Cx)  
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    u8 i;
    u16 j;
    j= 2000;while(j--);
    if(I2Cx == I2C1)
    {
        /*I2C1��ӳ��IO��*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;                 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIOB->BSRR = 1<<8;
        for (i= 0;i<10;i++)
        {
            GPIOB->BRR = 1<<8;
            j= 100;while(j--);
            GPIOB->BSRR = 1<<8;
            j= 100;while(j--);
        }
        GPIOB->BRR = 1<<9;
        j= 100;while(j--);
        GPIOB->BSRR = 1<<9;		

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if(I2Cx == I2C2)
    {
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;  
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);	

        GPIOB->BSRR = 1<<10;
        for (i= 0;i<10;i++)
        {
            GPIOB->BRR = 1<<10;
            j= 100;while(j--);
            GPIOB->BSRR = 1<<10;
            j= 100;while(j--);
        }

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }   
}

/********************************************************************************************************
**������Ϣ ��void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )            
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM��ַ),
			 unsigned short tx_count(���ݳ���),unsigned char *tx_data(����bufָ��)
**������� ����
*********************************************************************************************************/
void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    unsigned short i;
    unsigned char *p;
    p = tx_data;
    
    I2CTXByte(I2Cx, mem_byte_addr);
    for(i = 0; i < tx_count; i++)
    {
        /*��������*/
        I2CTXByte(I2Cx, *p); 
        p++;
    }    
    I2C_GenerateSTOP( I2Cx, ENABLE);    
}


/********************************************************************************************************
**������Ϣ ��void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char temp)              
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��temp
**������� ����
********************************************************************************************************/
 void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char temp)
{
    I2C_SendData(I2Cx, temp);
    while(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE) == 0);
    
}

/********************************************************************************************************
**������Ϣ ��void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )           
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM��ַ),
			 unsigned char rx_count(���ݳ���),unsigned char *rx_data(����bufָ��)
**������� ����
********************************************************************************************************/
void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    /*д�豸��ַ*/
    I2CMasterWrite(I2Cx, mem_byte_addr, 0, rx_data);                            
    I2CRXGroup(I2Cx,rx_count,rx_data); 

    I2C_GenerateSTOP( I2Cx, ENABLE);  
    /*���stop�ź�*/
//	while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);                         
    I2CRelase(I2Cx);
}


/********************************************************************************************************
**������Ϣ ��void I2CRXGroup(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf)            
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2),u16 rx_count(���ݸ���),u8 *data_buf(��������ָ��)
**������� ��temp
********************************************************************************************************/
void I2CRXGroup(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf)
{
    int i,data_cnt = 0;
    char flag = 0;
    for(i=0;i<rx_count;i++)         
    {	
        while(1)
        {
        /*RX FIFOδ����д�뷢��ָ��*/
            if((I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFNF) == 1) && (flag == 0)) 
            {
                I2C_ReadCmd(I2Cx);	
                data_cnt++;
                if(data_cnt == rx_count)
                {
                    flag = 1;
                }				

            }
            if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_RFNE))
            {
                data_buf[i] = I2C_ReceiveData(I2Cx);
                break;
            }		
        }
    }
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
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL))
        {
            break;
        }	
    }
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
