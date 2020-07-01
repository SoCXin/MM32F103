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
**������Ϣ ��SPIM_CSLow(SPI_TypeDef* SPIx)
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ�
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    //Spi cs assign to this pin,select
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, ENABLE);
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
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, DISABLE);
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
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))	//���ռĴ����ǿձ�־λ
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

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI1 clk enable

    SPIM_CSHigh(SPIx);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi1_cs  pa4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi1_sck  pa5
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi1_mosi  pa7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi1_miso  pa6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//DMA����λ��
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;//SPI����λ��

    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //�������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ�����ȴ���
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE); 									//Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    SPIM_TXEn(SPIx);//SPI����ʹ��
    SPIM_RXEn(SPIx);//SPI����ʹ��
}

/********************************************************************************************************
**������Ϣ ��SPI_DMA_TX_Init
**�������� : SPI����DMA��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void SPI_DMA_TX_Init()
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->TXREG); //SPI�������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)txdata;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//���ݴ�memory������
    DMA_InitStructure.DMA_BufferSize = 8;//�������ݸ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ���ı�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//memory��ַ��������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//��ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);//SPI RXΪDMAͨ��5
}

/********************************************************************************************************
**������Ϣ ��SPI_master_DMA_TX
**�������� : ͨ��DMA����ģʽ������Է�����
**������� ����
**������� ����
**������ע ����Ҫ��MISO&MOSI�̽�
********************************************************************************************************/
void SPI_master_DMA_TX()
{
    unsigned int i;

    printf("SPI DMA TEST\r\n");
    for(i = 0; i < 8; i++)
    {
        rxdata[i] = 0x0;
    }

    SPIM_Init(SPI2, 2); //SPIʱ��Ϊ36MHz
    SPI_DMA_TX_Init();
    SPI_DMACmd(SPI2, SPI_DMAReq_EN, ENABLE); //ʹ��SPI DMA
    DMA_Cmd(DMA1_Channel5, ENABLE);  //ʹ��UART1 TX DMA1 ��ָʾ��ͨ��
    SPIM_CSLow(SPI2);  					//Spi cs assign to this pin,select

    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));//ͨ��3���ݴ�����ɱ�־λ
    DMA_ClearFlag(DMA1_FLAG_TC5);//���ͨ��3���ݴ�����ɱ�־λ

    SPI_RxBytes(SPI2, 8); //����8������
    for(i = 0; i < 8; i++)
    {
        rxdata[i] = SPIMReadWriteByte(SPI2, 0x00); //����64�����壬��ȡMOSI���͵�����
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





