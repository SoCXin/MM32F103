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

unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;
extern void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count);



void SPI1_IRQHandler (void)
{
    while(1)
    {
        if(SPI_GetITStatus(SPI1, SPI_IT_TXEPT))
        {
            SPI_ClearITPendingBit(SPI1, SPI_IT_TXEPT);
            spi0_tx_flag = 1;
            break;
        }

        if(SPI_GetITStatus(SPI1, SPI_IT_RX))
        {
            SPI_ClearITPendingBit(SPI1, SPI_IT_RX);		 //clear rx interrupt
            spi0_rx_flag = 1;
            break;
        }
    }
}

void SPI2_IRQHandler (void)
{
    while(1)
    {
        if(SPI_GetITStatus(SPI2, SPI_IT_TXEPT))
        {
            SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
            spi0_tx_flag = 1;
            break;
        }

        if(SPI_GetITStatus(SPI2, SPI_IT_RX))
        {
            SPI_ClearITPendingBit(SPI2, SPI_IT_RX);		 //clear rx interrupt
            spi0_rx_flag = 1;
            break;
        }
    }
}
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
    while(1)
    {
        if(spi0_tx_flag == 1)
        {
            spi0_tx_flag = 0;
            break;
        }
    }
    while (1)
    {
        if(spi0_rx_flag == 1)
        {
            spi0_rx_flag = 0;
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
    NVIC_InitTypeDef NVIC_InitStructure;
    if(SPIx == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI1 clk enable
        SPIM_CSHigh(SPIx);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   		//spi1_cs  pa4
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   		//spi1_sck  pa5
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   		//spi1_mosi  pa7
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  		//spi1_miso  pa6
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(SPIx == SPI2)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//SPI2 clk enable
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_sck  pb13
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_mosi  pb15
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi2_miso  pb14
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
        GPIO_Init(GPIOB, &GPIO_InitStructure);

    }

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    if(SPIx == SPI1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    if(SPIx == SPI2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    SPI_ITConfig(SPIx, SPI_IT_RX | SPI_IT_TXEPT, ENABLE);
    SPI_Cmd(SPIx, ENABLE); 									//Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}

/********************************************************************************************************
**函数信息 ：SPIM_ReadID(SPI_TypeDef* SPIx)
**功能描述 :读取ID
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_ReadID(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    unsigned int i;

    SPIM_CSLow(SPIx);								//Spi cs assign to this pin,select
    SPIMReadWriteByte( SPIx, RDID);

    for(i = 0; i < 3; i++)
    {
        temp = SPIMReadWriteByte( SPIx, 0x01);
        printf("temp=0x%x\r\n", temp);
    }
    SPIM_CSHigh(SPIx);   							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_WriteEnable(SPI_TypeDef* SPIx)
**功能描述 :写数据使能
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte( SPIx, WREN);
    SPIM_CSHigh(SPIx); 								//Spi cs release
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
    SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte( SPIx, RDSR);
    while(1)
    {
        temp = SPIMReadWriteByte( SPIx, 0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh(SPIx);  							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_WriteEnable(SPI_TypeDef* SPIx)
**功能描述 :写数据失能
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteDisable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx);
    SPIMReadWriteByte( SPIx, WRDI);
    SPIM_CSHigh(SPIx);
}

/********************************************************************************************************
**函数信息 ：SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
**功能描述 :读取256页数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  address  ;*p ;number
**输出参数 ：无
********************************************************************************************************/
void SPIM_PageRead(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number) //page = 256 bytes
{
    unsigned char addr0, addr1, addr2;
    unsigned int i;
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select

    SPIMReadWriteByte( SPIx, READ);
    SPIMReadWriteByte( SPIx, addr0);
    SPIMReadWriteByte( SPIx, addr1);
    SPIMReadWriteByte( SPIx, addr2);

    for(i = 0; i < 256; i++)
    {
        rxtmpdata[i] = SPIMReadWriteByte(SPIx, 0x00);
    }


    SPIM_CSHigh(SPIx);  							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
**功能描述 :发送256页数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;  address;*p;number
**输出参数 ：无
********************************************************************************************************/
void SPIM_PageProgram(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number)
{
    unsigned int j;
    unsigned char addr0, addr1, addr2;
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable(SPIx);
    SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, PP);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);
    for(j = 0; j < number; j++)
    {
        SPIMReadWriteByte(SPIx, *(p++));
    }

    SPIM_CSHigh(SPIx);  							//Spi cs release

    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**函数信息 ：SPIM_SectorErase(SPI_TypeDef* SPIx)
**功能描述 :擦除数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输入参数 ：unsigned long address,删除指定sector的地址 each sector = 64Kbytes
**输出参数 ：无
********************************************************************************************************/
void SPIM_SectorErase(SPI_TypeDef* SPIx, unsigned long address)
{
    unsigned char addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address >> 16)) & 0xff;
    addr1 = ((unsigned char)(address >> 8)) & 0xff;
    addr2 = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select

    SPIMReadWriteByte(SPIx, SE);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);
    SPIM_CSHigh(SPIx);  							//Spi cs release

    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**函数信息 ：SPIM_BlockErase(SPI_TypeDef* SPIx)
**功能描述 :擦除数据
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_BlockErase(SPI_TypeDef* SPIx)
{
    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select

    SPIMReadWriteByte( SPIx, BE);

    SPIM_CSHigh(SPIx);  							//Spi cs release

    SPIM_checkStatus(SPIx);
}

void SPIM_Close(SPI_TypeDef* SPIx)
{
    SPIM_CSHigh(SPIx);  							//Spi cs release
    SPIMReadWriteByte(SPIx, 0x01);
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
    for(i = 0; i < 256; i++)
    {
        tmpdata[i] = i * 2;
    }
    if(SPIx == SPI1)
    {
        printf("spi1 test\r\n");
        SPIM_Init(SPIx, 0x2);

        SPIM_ReadID(SPIx);

        SPIM_SectorErase(SPIx, 0);

        SPIM_PageProgram(SPIx, 0, tmpdata, 256);

        for(i = 0; i < 256; i++)
        {
            rxtmpdata[i] = 0x0;
        }
        SPIM_PageRead(SPIx, 0, rxtmpdata, 256); 		//把写进去的一页256字节读出来

        for(i = 0; i < 10; i++)
        {
            printf("rx[%d]=0x%x\r\n", i, rxtmpdata[i]);
        }

        printf("spi1 test over\r\n");
    }

    if(SPIx == SPI2)
    {
        printf("spi2 test\r\n");
        SPIM_Init(SPIx, 0x2);

        SPIM_ReadID(SPIx);

        SPIM_SectorErase(SPIx, 0);

        SPIM_PageProgram(SPIx, 256, tmpdata, 256);

        for(i = 0; i < 256; i++)
        {
            rxtmpdata[i] = 0x0;
        }
        SPIM_PageRead(SPIx, 256, rxtmpdata, 256); 		//把写进去的一页256字节读出来

        for(i = 0; i < 10; i++)
        {
            printf("rx[%d]=0x%x\r\n", i, rxtmpdata[i]);
        }
        printf("spi2 test over\r\n");
    }


}



/*------------------------------------SPI loop------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/

/********************************************************************************************************
**函数信息 ：SPI_master_loop
**功能描述 : 自发自收
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
**函数备注 ：需要将MISO&MOSI短接
********************************************************************************************************/
void SPI_master_loop(SPI_TypeDef* SPIx)
{
    unsigned int i;
    unsigned char temp[20] = {0x55, 0xaa, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x56, 0x23, 0xff, 0xaa, 0x87, 0x89, 0x88, 0x20, 0x34, 0x38, 0xcd};
    unsigned char rxdata[20];
    for(i = 0; i < 20; i++)
    {
        rxdata[i] = 0x0;
    }

    SPIM_Init(SPIx, 0x2);

    SPIM_CSLow(SPIx);  					//Spi cs assign to this pin,select

    for(i = 0; i < 20; i++)
    {
        rxdata[i] = SPIMReadWriteByte( SPIx, temp[i]);
        printf("rxdata[%d]=0x%x\r\n", i, rxdata[i]);
    }

    SPIM_CSHigh(SPIx);  				//Spi cs assign to this pin,select
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






