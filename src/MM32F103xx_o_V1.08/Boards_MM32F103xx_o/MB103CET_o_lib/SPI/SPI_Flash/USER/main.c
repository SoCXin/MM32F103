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
**函数信息 ：DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)                       
**功能描述 :对比发送数据与接收数据有无对错
**输入参数 ：*p1,*p2,count
**输出参数 ：无
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
**函数信息 ：int main (void)                          
**功能描述 ：开机后，串口助手打印256页数据
**输入参数 ：
**输出参数 ：
**    备注 ：注意：改变SPI1或SP2，板子上对应短路帽也应改变
********************************************************************************************************/

int main(void)
{
    /*串口初始化为9600*/
    uart_initwBaudRate(9600);	 	
    /*只需修改参数*/    
    SPIM_Test(SPI2);			                                                
    while(1) {
    }
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
**函数信息 ：SPIM_Test(SPI_TypeDef* SPIx)          
**功能描述 :测试程序，使用串口打印256页数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
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
        
        /*把写进去的一页256字节读出来*/
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
        
        /*把写进去的一页256字节读出来*/
        SPIM_PageRead(SPIx, 256, rxtmpdata, 256); 		                        
        
        for(i = 0; i < 10; i++){
            UartSendGroup((u8*)printBuf, sprintf(printBuf,"rx[%d]=0x%x\r\n", i, rxtmpdata[i]));
        }
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"spi2 test over\r\n"));
    }    
}

/********************************************************************************************************
**函数信息 ：SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)              
**功能描述 : 发送256页数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  address;*p;number
**输出参数 ：无
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
**函数信息 ：SPIM_SectorErase(SPI_TypeDef* SPIx)             
**功能描述 :擦除数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输入参数 ：unsigned long address,删除指定sector的地址 each sector = 64Kbytes
**输出参数 ：无
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
**函数信息 ：SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)               
**功能描述 :读取256页数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  address  ;*p ;number
**输出参数 ：无
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
**函数信息 ：SPIM_WriteEnable(SPI_TypeDef* SPIx)                  
**功能描述 :写数据使能
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
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
**函数信息 ：SSPIM_checkStatus(SPI_TypeDef* SPIx)                 
**功能描述 :检查数据发送是否正确
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
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
**函数信息 ：SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)            
**功能描述 :可修改参数初始化SPI
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;spi_baud_div
**输出参数 ：无
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
        /*复用推挽输出*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        /*spi1_sck  pb3 ;spi1_mosi  pb5*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_5; 
        /*复用推挽输出*/        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /*spi1_miso  pb4*/
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;  		                    
        /*上拉输入*/
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
        /*复用推挽输出*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                    
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14; 
        /*上拉输入*/        
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
    
    /*Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率*/
    SPI_Cmd(SPIx, ENABLE); 						                                
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);	
}

/********************************************************************************************************
**函数信息 ：SPIM_ReadID(SPI_TypeDef* SPIx)                   
**功能描述 : 读取ID 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
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
**函数信息 ：SPIM_CSLow(SPI_TypeDef* SPIx)                      
**功能描述 : 为选定的SPI 软件重置内部NSS 管脚 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    //Spi cs assign to this pin,select	
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0,ENABLE);
}

/********************************************************************************************************
**函数信息 ：SPIM_CSHigh(SPI_TypeDef* SPIx)                      
**功能描述 :为选定的SPI 软件配置内部NSS 管脚 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
    //Spi cs release
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0,DISABLE);
}

/********************************************************************************************************
**函数信息 ：SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)                    
**功能描述 : 通过外设 SPIx 收发数据 ,用于全双工模式(同时收发)
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  tx_data
**输出参数 ：无
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
**函数信息 ：SPIM_TXEn(SPI_TypeDef* SPIx)                     
**功能描述 : 关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXDisable(SPI_TypeDef* SPIx)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXEn(SPI_TypeDef* SPIx)                     
**功能描述 : 关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXDisable(SPI_TypeDef* SPIx)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
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

