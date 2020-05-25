/**
******************************************************************************
* @file    spi.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the spi firmware functions.
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
#include "spi.h"
#include "uart.h"

unsigned char rxdata[8];
unsigned char txdata[8] = {0x55, 0xaa, 0xbb, 0xdd, 0x33, 0x77, 0x22, 0x11};


/********************************************************************************************************
**函数信息 ：SPIM_CSLow(SPI_TypeDef* SPIx)
**功能描述 :为选定的SPI 软件重置内部NSS 管脚
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    //Spi cs assign to this pin,select
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, ENABLE);
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
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, DISABLE);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXEn(SPI_TypeDef* SPIx)
**功能描述 :关闭 SPI 在双向模式下的数据传输方向
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
**功能描述 :关闭 SPI 在双向模式下的数据传输方向
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
**函数信息 ：SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)
**功能描述 : 通过外设 SPIx 收发数据 ,用于全双工模式(同时收发)
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  tx_data
**输出参数 ：无
********************************************************************************************************/
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while (1)
    {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))	//接收寄存器非空标志位
        {
            return SPI_ReceiveData(SPIx);
        }
    }
}



/********************************************************************************************************
**函数信息 ：SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
**功能描述 :可修改参数初始化SPI
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;spi_baud_div
**输出参数 ：无
********************************************************************************************************/
void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI1 clk enable

    SPIM_CSHigh(SPIx);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi1_cs  pa4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi1_sck  pa5
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi1_mosi  pa7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi1_miso  pa6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//DMA数据位宽
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;//SPI数据位宽

    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //软件控制NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位数据先传输
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE); 									//Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率
    SPIM_TXEn(SPIx);//SPI发送使能
    SPIM_RXEn(SPIx);//SPI接收使能
}

/********************************************************************************************************
**函数信息 ：SPI_DMA_TX_Init
**功能描述 : SPI发送DMA初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPI_DMA_TX_Init()
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->TXREG); //SPI发送数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)txdata;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//数据从memory到外设
    DMA_InitStructure.DMA_BufferSize = 8;//发送数据个数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不改变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//memory地址递增开启
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);//SPI RX为DMA通道5
}

/********************************************************************************************************
**函数信息 ：SPI_master_DMA_TX
**功能描述 : 通过DMA发送模式，完成自发自收
**输入参数 ：无
**输出参数 ：无
**函数备注 ：需要将MISO&MOSI短接
********************************************************************************************************/
void SPI_master_DMA_TX()
{
    unsigned int i;

    printf("SPI DMA TEST\r\n");
    for(i = 0; i < 8; i++)
    {
        rxdata[i] = 0x0;
    }

    SPIM_Init(SPI2, 2); //SPI时钟为36MHz
    SPI_DMA_TX_Init();
    SPI_DMACmd(SPI2, SPI_DMAReq_EN, ENABLE); //使能SPI DMA
    DMA_Cmd(DMA1_Channel5, ENABLE);  //使能UART1 TX DMA1 所指示的通道
    SPIM_CSLow(SPI2);  					//Spi cs assign to this pin,select

    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));//通道3数据传输完成标志位
    DMA_ClearFlag(DMA1_FLAG_TC5);//清除通道3数据传输完成标志位

    SPI_RxBytes(SPI2, 8); //接收8个数据
    for(i = 0; i < 8; i++)
    {
        rxdata[i] = SPIMReadWriteByte(SPI2, 0x00); //发送64个脉冲，读取MOSI发送的数据
    }

    SPIM_CSHigh(SPI2);  				//Spi cs assign to this pin,select

    for(i = 0; i < 8; i++)
    {
        printf("rxdata[%d]=0x%x\r\n", i, rxdata[i]);
    }
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





