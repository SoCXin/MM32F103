#include "sys.h"
#include "spi.h"
#include "uart.h"



//��������������
void data_clear(u8 *data, u8 size)
{
    for(size = size; size > 0; size--)
    {
        *data = 0;
        data++;
    }
}
//���ݶԱ�
void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {
        if(p1[i] != p2[i])
            printf("tx[%d]=0x%x,rx[%d]=0x%x\r\n", i, p1[i], i, p2[i]);
    }
}
void SPIM_CSLow()
{
    PAout(4) = 0; //Ƭѡ����
}

void SPIM_CSHigh()
{
    PAout(4) = 1; //Ƭѡ����
}
void SPIM_TXEn()
{
    SPI1->GCTL |= SPI_GCTL_TXEN;//ʹ����SPI1����
}

void SPIM_TXDisable()
{
    SPI1->GCTL &= ~SPI_GCTL_TXEN;//ʧ��SPI1����
}

void SPIM_RXEn()
{
    SPI1->GCTL |= SPI_GCTL_RXEN;//ʹ����SPI1����
}

void SPIM_RXDisable()
{
    SPI1->GCTL &= ~SPI_GCTL_RXEN;//ʧ��SPI1����
}


unsigned int SPIMReadWriteByte(unsigned char tx_data)
{
    SPIMSendByte(tx_data);
    while(1)
    {
        if(SPI1->CSTAT & SPI_CSTAT_RXAVL) //SPI1������ɱ�־λ
        {
            break;
        }
    }

    return SPI1->RXREG;
}
void SPIMSendByte(unsigned char tx_data)
{
    SPI1->TXREG = tx_data;
    while(1)
    {
        if(SPI1->CSTAT & SPI_CSTAT_TXEPT) //SPI1������ɱ�־λ
        {
            break;
        }
    }
}

unsigned char SPIM_Readbyte()
{

    while(1)
    {
        if(SPI1->CSTAT & SPI_CSTAT_RXAVL) //SPI1������ɱ�־λ
        {
            break;
        }
    }

    return SPI1->RXREG;

}

void SPIM_Init()
{

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //����SPI1ʱ��
    SPIM_CSHigh();

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //����GPIOAʱ��
    GPIOA->CRL &= 0x0000FFFF; //����PA5,PA6,PA7Ϊȱʡֵ
    GPIOA->CRL |= 0xB0B30000; //PA5,PA7���츴�����
    GPIOA->CRL |= 0x08000000; //PA6��������
    GPIOA->ODR |= GPIO_ODR_ODR6;
    SPI1->GCTL |= SPI_GCTL_MM; //����ģʽ
    SPI1->CCTL |= SPI_CCTL_SPILEN; //8λ���ݿ��

    SPI1->CCTL &= ~SPI_CCTL_CPOL; //SPI_CPOL_Low
    SPI1->CCTL |= SPI_CCTL_CPHA; //SPI_CPHA_1Edge

    SPI1->GCTL &= ~SPI_GCTL_NSS_SEL; //�������NSS
    SPI1->CCTL |= 0x10; //��������ģʽ
    SPI1->SPBRG = 0x4; //2��Ƶ��������Ӧ��Ϊ72M/2=36M
    SPI1->CCTL &= ~SPI_CCTL_LSBFE; //MSB
    SPI1->GCTL |= SPI_GCTL_SPIEN; //ʹ��SPI
    SPIM_TXEn();
    SPIM_RXEn();
}

u8 SPIM_ReadID()
{
    unsigned char temp;
    unsigned int i;

    SPIM_CSLow();								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);

    for(i = 0; i < 3; i++)
    {
        temp = SPIMReadWriteByte(0x01);
    }
    SPIM_CSHigh();   							//Spi cs release
    if(temp != 0xff)
        return 0;
    else
        return 1;
}

void SPIM_WriteEnable()
{

    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);
    SPIM_CSHigh(); 								//Spi cs release
}


void SPIM_checkStatus()
{
    unsigned char temp;
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDSR);
    while(1)
    {
        temp = SPIMReadWriteByte(0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();  							//Spi cs release
}

void SPIM_WriteDisable()
{
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
}

void SPIM_PageRead(unsigned long address, unsigned char *p, unsigned int number) //page = 256 bytes
{
    unsigned char addr0, addr1, addr2;
    unsigned int i;
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow(); 								//Spi cs assign to this pin,select

    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);

    for(i = 0; i < number; i++)
    {
        *p = SPIMReadWriteByte(0x00);
        p++;
    }


    SPIM_CSHigh();  							//Spi cs release
}

void SPIM_PageProgram(unsigned long address, unsigned char *p, unsigned int number)
{
    unsigned int j;
    unsigned char addr0, addr1, addr2;
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable();
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j = 0; j < number; j++)
    {
        SPIMReadWriteByte(*(p++));
    }

    SPIM_CSHigh();  							//Spi cs release

    SPIM_checkStatus();

}

void SPIM_SectorErase(unsigned long address)
{
    unsigned char addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address >> 16)) & 0xff;
    addr1 = ((unsigned char)(address >> 8)) & 0xff;
    addr2 = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable();

    SPIM_CSLow();  								//Spi cs assign to this pin,select

    SPIMReadWriteByte(SE);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    SPIM_CSHigh();  							//Spi cs release

    SPIM_checkStatus();
}

void SPIM_BlockErase()
{
    SPIM_WriteEnable();

    SPIM_CSLow();  		//Spi cs assign to this pin,select
    SPIM_TXEn(); 			//Transmit Enable bit TXEN
    SPIMReadWriteByte(BE); 	//SPDAT = PP;CheckStatus();
    SPIM_TXDisable(); 	//disable TXEN
    SPIM_CSHigh();  		//Spi cs release

    SPIM_checkStatus();
}

void SPIM_Close()
{
    SPIM_CSHigh();  		//Spi cs release
    SPI1->RXDNR = 0x1; //����1���ֽ�
}






