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
**函数信息 ：int main (void)                          
**功能描述 ：开机后，串口打印数据
**输入参数 ：
**输出参数 ：
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
**函数信息 ：I2CMasterTest(I2C_TypeDef *I2Cx)          
**功能描述 ：I2C接收发送数据测试程序
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
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
    /*e2prom 操作间隔延时*/
    i = 80000;while(i--);	
    
    I2CMasterRead(I2Cx,0, 8, rx_buffer0);
    
    for(i = 0; i <8 ;i ++)
    {
		UartSendGroup((u8*)printBuf, sprintf(printBuf,"rx_buffer0[%d]: %x \r\n", i,rx_buffer0[i]));
    }
	UartSendGroup((u8*)printBuf, sprintf(printBuf,"i2c test over !!!\r\n"));
}


/********************************************************************************************************
**函数信息 ：void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned int uiI2C_speed)  //unit is Khz                    
**功能描述 ：初始化I2C
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
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
**函数信息 ：I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)          
**功能描述 ：选择从机地址
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2;deviceaddr:从机地址
**输出参数 ：无
********************************************************************************************************/
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)  
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    if(I2Cx == I2C1)
    {
        /*I2C1重映射IO口*/
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
**函数信息 ：void I2CRelase(I2C_TypeDef *I2Cx,unsigned char deviceaddr)           
**功能描述 ：选择从机地址
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2;deviceaddr:从机地址
**输出参数 ：无
********************************************************************************************************/
void I2CRelase(I2C_TypeDef *I2Cx)  
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    u8 i;
    u16 j;
    j= 2000;while(j--);
    if(I2Cx == I2C1)
    {
        /*I2C1重映射IO口*/
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
**函数信息 ：void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )            
**功能描述 ：I2C发送数据
**输入参数 ：I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM地址),
			 unsigned short tx_count(数据长度),unsigned char *tx_data(数据buf指针)
**输出参数 ：无
*********************************************************************************************************/
void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    unsigned short i;
    unsigned char *p;
    p = tx_data;
    
    I2CTXByte(I2Cx, mem_byte_addr);
    for(i = 0; i < tx_count; i++)
    {
        /*发送数据*/
        I2CTXByte(I2Cx, *p); 
        p++;
    }    
    I2C_GenerateSTOP( I2Cx, ENABLE);    
}


/********************************************************************************************************
**函数信息 ：void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char temp)              
**功能描述 ：I2C发送数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2；temp
**输出参数 ：无
********************************************************************************************************/
 void I2CTXByte(I2C_TypeDef *I2Cx,unsigned char temp)
{
    I2C_SendData(I2Cx, temp);
    while(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE) == 0);
    
}

/********************************************************************************************************
**函数信息 ：void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )           
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx(I2C1,I2C2),unsigned short mem_byte_addr(E2PROM地址),
			 unsigned char rx_count(数据长度),unsigned char *rx_data(数据buf指针)
**输出参数 ：无
********************************************************************************************************/
void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    /*写设备地址*/
    I2CMasterWrite(I2Cx, mem_byte_addr, 0, rx_data);                            
    I2CRXGroup(I2Cx,rx_count,rx_data); 

    I2C_GenerateSTOP( I2Cx, ENABLE);  
    /*检测stop信号*/
//	while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);                         
    I2CRelase(I2Cx);
}


/********************************************************************************************************
**函数信息 ：void I2CRXGroup(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf)            
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx(I2C1,I2C2),u16 rx_count(数据个数),u8 *data_buf(接收数据指针)
**输出参数 ：temp
********************************************************************************************************/
void I2CRXGroup(I2C_TypeDef *I2Cx,u16 rx_count,u8 *data_buf)
{
    int i,data_cnt = 0;
    char flag = 0;
    for(i=0;i<rx_count;i++)         
    {	
        while(1)
        {
        /*RX FIFO未满就写入发送指令*/
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
**函数信息 ：I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**功能描述 ：检查发送中断标志位
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
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
**函数信息 ：I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**功能描述 ：检查接收中断标志位
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
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
