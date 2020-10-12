/**
******************************************************************************
* @file    i2c.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the i2c firmware functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/

#include "HAL_device.h"
#include "uart.h"

#define FLASH_DEVICE_ADDR 0xa0
unsigned char chI2c_Txdata[10] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa};
unsigned char chI2c_Rxdata[10] = {0, 0, 0, 0, 0, 0, 0, 0};

/********************************************************************************************************
**函数信息 ：I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned long apb_mhz,unsigned long i2c_baud_rate) //unit is Khz
**功能描述 ：初始化I2C
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
********************************************************************************************************/
void I2CInitMasterMode(I2C_TypeDef *I2Cx, unsigned int uiI2C_speed)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    if(I2Cx == I2C1)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);//复用I2C1 IO口
        GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);//I2C1重映射
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1重映射IO口
    }
    if(I2Cx == I2C2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;
    }
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//保持总线空闲即CLK&DATA为高
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
    I2C_InitStructure.I2C_OwnAddress = 0;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = uiI2C_speed;

    I2C_Init( I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;				//需要外加上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/********************************************************************************************************
**函数信息 ：I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)
**功能描述 ：选择从机地址
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2;deviceaddr:从机地址
**输出参数 ：无
********************************************************************************************************/
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx, unsigned char deviceaddr)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    if(I2Cx == I2C1)
    {
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1重映射IO口
    }
    if(I2Cx == I2C2)
    {
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;
    }

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, deviceaddr, I2C_Direction_Transmitter);
    I2C_Cmd(I2Cx, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
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
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_RFNE))
        {
            break;
        }
    }
}
/********************************************************************************************************
**函数信息 ：I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
**功能描述 ：I2C发送数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2；cmd；temp
**输出参数 ：无
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx, unsigned char data)
{
    I2C_SendData(I2Cx, data);
    while(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE) == 0);
}
/********************************************************************************************************
**函数信息 ：I2CRXByte(I2C_TypeDef *I2Cx)
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFNF) == 1)			//修改为函数
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
**函数信息 ：I2CRXGroup()
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx(I2C1,I2C2)
u16 rx_count(数据个数)
u8 *data_buf(接收数据指针)
**输出参数 ：temp
********************************************************************************************************/
void I2CRXGroup_DMA(I2C_TypeDef *I2Cx, u16 rx_count, u8 *data_buf)
{
    DMA_InitTypeDef  DMA_InitStructure;

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* DMA1 channel5 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (I2C1->IC_DATA_CMD); //I2C数据接收寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)chI2c_Rxdata;//接收到的数据存放的数据基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//数据从外设到memory
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//不是M2M模式
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);

    I2C1->IC_DMA_CR = 0x1; //I2C接收DMA使能
    DMA_Cmd(DMA1_Channel7, ENABLE);
    /* DMA1 Channel5 transfer complete test */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC7))
    {
        I2C_ReadCmd(I2C1);//读命令，不能省略
    }
}

/********************************************************************************************************
**函数信息 ：	I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
**功能描述 ：	I2C发送数据
**输入参数 ：
I2C_TypeDef *I2Cx(I2C1,I2C2)
unsigned short mem_byte_addr(E2PROM地址)
unsigned short tx_count(数据长度)
unsigned char *tx_data(数据buf指针)
**输出参数 ：无
********************************************************************************************************/
void E2PROM_Write(	I2C_TypeDef *I2Cx, \
                    unsigned short mem_byte_addr, \
                    unsigned short tx_count, \
                    unsigned char *tx_data  )
{
    unsigned short i;
    unsigned char *p;
    p = tx_data;

    I2CTXByte(I2Cx, mem_byte_addr); //发送要发送数据的地址
    for(i = 0; i < tx_count; i++)
    {
        I2CTXByte(I2Cx, *p); //发送数据
        p++;
    }

    I2C_GenerateSTOP(I2C1, ENABLE);
    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET)) == 0); //检测stop信号
}

/********************************************************************************************************
**函数信息 ：	I2CMasterRead()
**功能描述 ：	I2C接收数据
**输入参数 ：
I2C_TypeDef *I2Cx(I2C1,I2C2)
unsigned short mem_byte_addr(E2PROM地址)
unsigned char rx_count(数据长度)
unsigned char *rx_data(数据buf指针)
**输出参数 ：无
********************************************************************************************************/
void  E2PROM_Read(	I2C_TypeDef *I2Cx, \
                    unsigned short mem_byte_addr, \
                    unsigned char rx_count, \
                    unsigned char *rx_data )
{

    I2CTXByte(I2Cx, mem_byte_addr);
    I2CRXGroup_DMA(I2Cx, rx_count, rx_data);

    while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET)) == 0); //检测stop信号
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

    printf("i2c test start ...\r\n");

    I2CInitMasterMode(I2Cx, 100000);
    I2CSetDeviceAddr(I2Cx, FLASH_DEVICE_ADDR);

    E2PROM_Write( I2Cx, 0, 10, chI2c_Txdata ); //往EEPROM地址0写10个数

    i = 80000; while(i--);						//eeprom 操作间隔延时

    E2PROM_Read( I2Cx, 0, 10, chI2c_Rxdata ); //将写入的数据读出来

    for(i = 0; i < 10 ; i ++)
    {
        printf("Rx[%d]: %x \r\n", i, chI2c_Rxdata[i]);
    }
    printf("i2c test over !!!\r\n");
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/
