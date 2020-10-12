/**
******************************************************************************
* @file    spi.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the spi functions.
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

#include "spi.h"

/********************************************************************************************************
**������Ϣ ��SPIM_CSLow(SPI_TypeDef* SPIx)
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ�
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    //Spi cs assign to this pin,select
    if(SPIx == SPI1)
    {
        GPIO_ResetBits( GPIOA, GPIO_Pin_4 );
    }
    else
    {
        GPIO_ResetBits( GPIOB, GPIO_Pin_12 );
    }
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
    if(SPIx == SPI1)
    {
        GPIO_SetBits( GPIOA, GPIO_Pin_4 );
    }
    else
    {
        GPIO_SetBits( GPIOB, GPIO_Pin_12 );
    }
}

/********************************************************************************************************
**������Ϣ ��SPIM_TXEn(SPI_TypeDef* SPIx)
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
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
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
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
**������Ϣ ��SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  tx_data
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while (1)
    {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT))
        {
            break;
        }
    }
    while (1)
    {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))
        {
            return SPI_ReceiveData(SPIx);
        }
    }
}

/********************************************************************************************************
**������Ϣ ��SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    if(SPIx == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI1 clk enable
        SPIM_CSHigh(SPIx);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   		//spi1_cs  pa4
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   		//spi1_sck  pa5
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   		//spi1_mosi  pa7
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  		//spi1_miso  pa6
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);

    }

    if(SPIx == SPI2)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//SPI2 clk enable
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// ���⸴�����
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_sck  pb13
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_mosi  pb15
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi2_miso  pb14
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
        GPIO_Init(GPIOB, &GPIO_InitStructure);

    }

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//DMA���ݿ��
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;//SPI���40MHz��72M���2��Ƶ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE); 									//Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)
**�������� :��ȡID
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
u8 SPIM_ReadID(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    unsigned int i;

    SPIM_CSLow(SPIx);								//Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, RDID);

    for(i = 0; i < 3; i++)
    {
        temp = SPIMReadWriteByte( SPIx, 0x01);
    }
    SPIM_CSHigh(SPIx);   							//Spi cs release
    if(temp != 0xff)
        return 0;
    else
        return 1;
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable(SPI_TypeDef* SPIx)
**�������� :д����ʹ��
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte( SPIx, WREN);
    SPIM_CSHigh(SPIx); 								//Spi cs release
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
**������Ϣ ��SPIM_WriteEnable(SPI_TypeDef* SPIx)
**�������� :д����ʧ��
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_WriteDisable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx);
    SPIMReadWriteByte( SPIx, WRDI);
    SPIM_CSHigh(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
**�������� :��ȡ256ҳ����
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  address  ;*p ;number
**������� ����
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

    for(i = 0; i < number; i++)
    {
        *p = SPIMReadWriteByte(SPIx, 0x00);
        p++;
    }


    SPIM_CSHigh(SPIx);  							//Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
**�������� :����256ҳ����
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  address;*p;number
**������� ����
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
**������Ϣ ��SPIM_SectorErase(SPI_TypeDef* SPIx)
**�������� :��������
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ��unsigned long address,ɾ��ָ��sector�ĵ�ַ each sector = 64Kbytes
**������� ����
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
**������Ϣ ��SPIM_BlockErase(SPI_TypeDef* SPIx)
**�������� :��������
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_BlockErase(SPI_TypeDef* SPIx)
{
    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select

    SPIMReadWriteByte( SPIx, BE);

    SPIM_CSHigh(SPIx);  							//Spi cs release

    SPIM_checkStatus(SPIx);
}
/********************************************************************************************************
**������Ϣ ��SPIM_Close(SPI_TypeDef* SPIx)
**�������� : CS�ź����ߣ�ֹͣͨ��
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_Close(SPI_TypeDef* SPIx)
{
    SPIM_CSHigh(SPIx);  							//Spi cs release
    SPIMReadWriteByte(SPIx, 0x01);
}


/********************************************************************************************************
**������Ϣ ��data_clear(u8 *data,u8 size)
**�������� : ��������
**������� ��*data������;size�����ݴ�С
**������� ����
********************************************************************************************************/
void data_clear(u8 *data, u8 size)
{
    for(size = size; size > 0; size--)
    {
        *data = 0;
        data++;
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



