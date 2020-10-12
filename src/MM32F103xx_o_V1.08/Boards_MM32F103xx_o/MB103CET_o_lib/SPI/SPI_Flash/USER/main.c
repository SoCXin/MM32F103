#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"
#include "string.h"

void SPIM_Test(SPI_TypeDef* SPIx);
void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8* buf,u16 len);
void SPIM_Test(SPI_TypeDef* SPIx);
void SPIM_Init(SPI_TypeDef* SPIx,unsigned short spi_baud_div);
void SPIM_ReadID(SPI_TypeDef* SPIx);
void SPIM_CSLow(SPI_TypeDef* SPIx);
void SPIM_CSHigh(SPI_TypeDef* SPIx);
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data);
void SPIM_SectorErase(SPI_TypeDef* SPIx,unsigned long address);
void SPIM_WriteEnable(SPI_TypeDef* SPIx);
void SPIM_checkStatus(SPI_TypeDef* SPIx);
void SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number);
void SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number);
void SPIM_RXEn(SPI_TypeDef* SPIx);
void SPIM_TXEn(SPI_TypeDef* SPIx);
void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);

char printBuf[100];
unsigned char tmpdata[256];
unsigned char rxtmpdata[256];

#define READ            0x03
#define FAST_READ       0x0B
#define RDID            0x9F
#define WREN            0x06
#define WRDI            0x04
#define SE              0xD8
#define BE              0xC7
#define PP              0x02
#define RDSR            0x05
#define WRSR            0x01
#define DP              0xB9    
#define RES             0xAB

/********************************************************************************************************
**������Ϣ ��DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)                       
**�������� :�Աȷ�������������������޶Դ�
**������� ��*p1,*p2,count
**������� ����
********************************************************************************************************/
void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)
{
    unsigned int i;
    
    for(i=0;i<count;i++)
    {
        if(p1[i] != p2[i])	
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"tx[%d]=0x%x,rx[%d]=0x%x\r\n",i,p1[i],i,p2[i]));
    }
}

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬴������ִ�ӡ256ҳ����
**������� ��
**������� ��
**    ��ע ��ע�⣺�ı�SPI1��SP2�������϶�Ӧ��·ñҲӦ�ı�
********************************************************************************************************/

int main(void)
{
    /*���ڳ�ʼ��Ϊ9600*/
    uart_initwBaudRate(9600);	 	
    /*ֻ���޸Ĳ���*/    
    SPIM_Test(SPI2);			                                                
    while(1) {
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
**������Ϣ ��SPIM_Test(SPI_TypeDef* SPIx)          
**�������� :���Գ���ʹ�ô��ڴ�ӡ256ҳ����
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_Test(SPI_TypeDef* SPIx)
{
    unsigned int i;
    for(i = 0; i < 256; i++){
        tmpdata[i] = i * 2;
    }
    if(SPIx == SPI1){
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"spi1 test\r\n"));
        SPIM_Init(SPIx, 0x20);
        
        SPIM_ReadID(SPIx);
        
        SPIM_SectorErase(SPIx, 0);
        
        SPIM_PageProgram(SPIx, 0, tmpdata, 256);
        
        memset(rxtmpdata, 0x0, 256);
        
        /*��д��ȥ��һҳ256�ֽڶ�����*/
        SPIM_PageRead(SPIx, 0, rxtmpdata, 256); 		                        
        
        for(i = 0; i < 10; i++){
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"rx[%d]=0x%x\r\n", i, rxtmpdata[i]));
        }

        UartSendGroup((u8*)printBuf, sprintf(printBuf,"spi1 test over\r\n"));
    }
    
    if(SPIx==SPI2)
    {
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"spi2 test\r\n"));
        SPIM_Init(SPIx, 0x20);
        
        SPIM_ReadID(SPIx);	
        
        SPIM_SectorErase(SPIx, 0);
        
        SPIM_PageProgram(SPIx, 256, tmpdata, 256);
        
        memset(rxtmpdata, 0x0, 256);
        
        /*��д��ȥ��һҳ256�ֽڶ�����*/
        SPIM_PageRead(SPIx, 256, rxtmpdata, 256); 		                        
        
        for(i = 0; i < 10; i++){
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"rx[%d]=0x%x\r\n", i, rxtmpdata[i]));
        }
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"spi2 test over\r\n"));
    }    
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)              
**�������� : ����256ҳ����
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  address;*p;number
**������� ����
********************************************************************************************************/
void SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
{
    unsigned int j;
    unsigned char addr0,addr1,addr2;
    /*page address*/
    address = address&0xffffff00; 					                            
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    SPIM_WriteEnable(SPIx);
    /*Spi cs assign to this pin,select*/
    SPIM_CSLow(SPIx);  								                            
    SPIMReadWriteByte(SPIx,PP);
    SPIMReadWriteByte(SPIx,addr0);
    SPIMReadWriteByte(SPIx,addr1);
    SPIMReadWriteByte(SPIx,addr2);
    for(j=0;j<number;j++){
        SPIMReadWriteByte(SPIx,*(p++));
    }
    /*Spi cs release*/
    SPIM_CSHigh(SPIx);  							                            
    
    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_SectorErase(SPI_TypeDef* SPIx)             
**�������� :��������
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ��unsigned long address,ɾ��ָ��sector�ĵ�ַ each sector = 64Kbytes
**������� ����
********************************************************************************************************/
void SPIM_SectorErase(SPI_TypeDef* SPIx,unsigned long address)
{
    unsigned char addr0,addr1,addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address>>16))&0xff;
    addr1 = ((unsigned char)(address>>8))&0xff;
    addr2 = ((unsigned char)address)&0xff;
    
    SPIM_WriteEnable(SPIx);
    
    /*Spi cs assign to this pin,select*/
    SPIM_CSLow(SPIx);  								                            
                                                                                
    SPIMReadWriteByte(SPIx,SE);                                                 
    SPIMReadWriteByte(SPIx,addr0);                                              
    SPIMReadWriteByte(SPIx,addr1);                                              
    SPIMReadWriteByte(SPIx,addr2);   
    /*Spi cs release*/    
    SPIM_CSHigh(SPIx);  							                            
    
    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)               
**�������� :��ȡ256ҳ����
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  address  ;*p ;number
**������� ����
********************************************************************************************************/
void SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)  
{
    unsigned char addr0,addr1,addr2;
    unsigned int i;
    /*page address*/
    address = address&0xffffff00; 				                                
    addr0 = (unsigned char)(address>>16);                               
    addr1 = (unsigned char)(address>>8);                                
    addr2 = (unsigned char)address;                                     
         
    /*Spi cs assign to this pin,select*/    
    SPIM_CSLow(SPIx); 							                                
                                                                        
    SPIMReadWriteByte( SPIx,READ);
    SPIMReadWriteByte( SPIx,addr0);
    SPIMReadWriteByte( SPIx,addr1);
    SPIMReadWriteByte( SPIx,addr2);
    
    for(i=0;i<256;i++){
        rxtmpdata[i] = SPIMReadWriteByte(SPIx,0x00);
    }
    
    /*Spi cs release*/
    SPIM_CSHigh(SPIx);  							                            
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable(SPI_TypeDef* SPIx)                  
**�������� :д����ʹ��
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
    /*Spi cs assign to this pin,select*/
    SPIM_CSLow(SPIx); 								                            
    SPIMReadWriteByte( SPIx,WREN);	
    /*Spi cs release*/    
    SPIM_CSHigh(SPIx); 								                            
}

/********************************************************************************************************
**������Ϣ ��SSPIM_checkStatus(SPI_TypeDef* SPIx)                 
**�������� :������ݷ����Ƿ���ȷ
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_checkStatus(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    /*Spi cs assign to this pin,select*/
    SPIM_CSLow(SPIx); 								                            
    SPIMReadWriteByte( SPIx,RDSR);	                                            
    while(1){                                                                   
        temp = SPIMReadWriteByte( SPIx,0x00);	                                
        if((temp&0x01)==0x0)                                                    
            break;                                                              
    }         
    /*Spi cs release*/    
    SPIM_CSHigh(SPIx);  							                            
}

/********************************************************************************************************
**������Ϣ ��SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)            
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Init(SPI_TypeDef* SPIx,unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    SPIM_CSHigh(SPIx);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
    
    if(SPIx==SPI1){
        /*SPI1 clk enable*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                    
        
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_5);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        /*�����������*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        /*spi1_sck  pb3 ;spi1_mosi  pb5*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_5; 
        /*�����������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /*spi1_miso  pb4*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;  		                    
        /*��������*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		                       
        GPIO_Init(GPIOB, &GPIO_InitStructure);   
    }
    
    if(SPIx==SPI2){
        /*SPI2 clk enable*/
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                    
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;   		
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        /*�����������*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14; 
        /*��������*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		                    
        GPIO_Init(GPIOB, &GPIO_InitStructure);                                  
    }                                                                           
                                                                                
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                               
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                           
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;                         
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			                    
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			                    
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);
    
    /*Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����*/
    SPI_Cmd(SPIx, ENABLE); 						                                
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);	
}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)                   
**�������� : ��ȡID 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_ReadID(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    unsigned int i;
    
    /*Spi cs assign to this pin,select*/
    SPIM_CSLow(SPIx);								                            
    SPIMReadWriteByte( SPIx,RDID);				
    
    for(i=0;i<3;i++){
        temp = SPIMReadWriteByte( SPIx,0x01);
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"temp=0x%x\r\n",temp));		
    }
    /*Spi cs release*/
    SPIM_CSHigh(SPIx);   							                            
}

/********************************************************************************************************
**������Ϣ ��SPIM_CSLow(SPI_TypeDef* SPIx)                      
**�������� : Ϊѡ����SPI ��������ڲ�NSS �ܽ� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    //Spi cs assign to this pin,select	
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0,ENABLE);
}

/********************************************************************************************************
**������Ϣ ��SPIM_CSHigh(SPI_TypeDef* SPIx)                      
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
    //Spi cs release
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0,DISABLE);
}

/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)                    
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  tx_data
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);	
    while (1){
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))	{
            return SPI_ReceiveData(SPIx);
        }
    }
}

/********************************************************************************************************
**������Ϣ ��SPIM_TXEn(SPI_TypeDef* SPIx)                     
**�������� : �ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_TXDisable(SPI_TypeDef* SPIx)                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_RXEn(SPI_TypeDef* SPIx)                     
**�������� : �ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_RXDisable(SPI_TypeDef* SPIx)                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
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

