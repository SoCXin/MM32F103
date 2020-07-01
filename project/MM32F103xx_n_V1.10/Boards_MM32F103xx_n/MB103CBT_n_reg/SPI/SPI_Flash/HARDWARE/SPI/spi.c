#include "sys.h"
#include "spi.h"
#include "uart.h"

unsigned char tmpdata[256];
unsigned char rxtmpdata[256];


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
    SPI2->SCSR &= 0xe; //Ƭѡ����
}

void SPIM_CSHigh()
{
    SPI2->SCSR |= 0x1; //Ƭѡ����
}
void SPIM_TXEn()
{
    SPI2->GCTL |= SPI_GCTL_TXEN;//ʹ����SPI2����
}

void SPIM_TXDisable()
{
    SPI2->GCTL &= ~SPI_GCTL_TXEN;//ʧ��SPI2����
}

void SPIM_RXEn()
{
    SPI2->GCTL |= SPI_GCTL_RXEN;//ʹ����SPI1����
}

void SPIM_RXDisable()
{
    SPI2->GCTL &= ~SPI_GCTL_RXEN;//ʧ��SPI1����
}

void SPIMSendByte(unsigned char tx_data)
{
    SPI2->TXREG = tx_data;
    while(1)
    {
        if(SPI2->CSTAT & SPI_CSTAT_TXEPT) //SPI1������ɱ�־λ
        {
            break;
        }
    }
}

unsigned char SPIM_Readbyte()
{

    while(1)
    {
        if(SPI2->CSTAT & SPI_CSTAT_RXAVL) //SPI1������ɱ�־λ
        {
            break;
        }
    }

    return SPI2->RXREG;

}

void SPIM_Init()
{

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; //����SPI1ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //����GPIOAʱ��

    SPIM_CSHigh();
    GPIOB->CRH &= 0x0000FFFF; //����PB12,PB13,PB14,PB15Ϊȱʡֵ

    GPIOB->CRH |= 0xB0BB0000; //PB15,PB13���츴�����
    GPIOB->CRH |= 0x08000000; //PB14��������
    GPIOB->ODR |= GPIO_ODR_ODR14;
    SPI2->GCTL |= SPI_GCTL_MM; //����ģʽ
    SPI2->CCTL |= SPI_CCTL_SPILEN; //8λ���ݿ��

    SPI2->CCTL &= ~SPI_CCTL_CPOL; //SPI_CPOL_Low
    SPI2->CCTL |= SPI_CCTL_CPHA; //SPI_CPHA_1Edge

    SPI2->GCTL &= ~SPI_GCTL_NSS_SEL; //�������NSS
    SPI2->CCTL |= 0x10; //��������ģʽ
    SPI2->SPBRG = 0x2; //2��Ƶ��������Ӧ��Ϊ72M/2=36M
    SPI2->CCTL &= ~SPI_CCTL_LSBFE; //MSB
    SPI2->GCTL |= SPI_GCTL_SPIEN; //ʹ��SPI

}

u8 SPIM_ReadID()
{
    unsigned char temp;
    unsigned int i;
    SPIM_CSLow();																			//Spi cs assign to this pin,select
    SPIM_TXEn(); 																			//Transmit Enable bit TXEN
    SPIMSendByte(RDID);
    SPIM_TXDisable(); 	//disable TXEN

    SPI2->RXDNR = 0x1; //����1���ֽ�

    for(i = 0; i < 3; i++)
    {
        SPIM_RXEn();      //enable RXEN
        temp = SPIM_Readbyte();
        SPIM_RXDisable();     //disable RXEN
        printf("temp=0x%x\r\n", temp);
    }

    SPIM_CSHigh();
    if(temp == 0x13)
        return 0;
    else
        return 1;
}

void SPIM_WriteEnable()
{
    SPIM_CSLow(); 																 //Spi cs assign to this pin,select
    SPIM_TXEn(); 																 //Transmit Enable bit TXEN
    SPIMSendByte(WREN); 													 //SPDAT = WREN;CheckStatus();
    SPIM_TXDisable(); 														 //disable TXEN
    SPIM_CSHigh(); 															 //Spi cs release
}


void SPIM_checkStatus()
{
    unsigned char temp;
    SPIM_CSLow(); 															 //Spi cs assign to this pin,select
    SPIM_TXEn(); 															//Transmit Enable bit TXEN
    SPIMSendByte(RDSR);            						//SPDAT = RDSR; CheckStatus();

    SPIM_TXDisable(); 	//disable TXEN

    SPI2->RXDNR = 0x1; //����1���ֽ�
    while(1)
    {
        SPIM_RXEn(); 												//enable RXEN
        temp = SPIM_Readbyte();
        SPIM_RXDisable();										//disable RXEN
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();  											//Spi cs release
}

void SPIM_WriteDisable()
{
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIM_TXEn(); 									//Transmit Enable bit TXEN
    SPIMSendByte(WRDI); 						//SPDAT = WREN;CheckStatus();
    SPIM_TXDisable();						 //disable TXEN
    SPIM_CSHigh();  							//Spi cs release
}

void SPIM_PageRead(unsigned long address, unsigned char *p, unsigned int number) //page = 256 bytes
{
    unsigned char addr0, addr1, addr2;
    unsigned int i;
    address = address & 0xffffff00; //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow(); 								 //Spi cs assign to this pin,select
    SPIM_TXEn();  								//Transmit Enable bit TXEN
    SPIMSendByte(READ); 					//SPDAT = PP;CheckStatus();
    SPIMSendByte(addr0);
    SPIMSendByte(addr1);
    SPIMSendByte(addr2);
    SPIM_TXDisable();					 //disable TXEN

    SPI2->RXDNR = 0x1; //����1���ֽ�
    for(i = 0; i < number; i++)
    {
        SPIM_RXEn(); 							//enable RXEN
        *p = SPIM_Readbyte();
        SPIM_RXDisable();				 //disable RXEN
        p++;
    }


    SPIM_CSHigh();  					//Spi cs release
}

void SPIM_PageProgram(unsigned long address, unsigned char *p, unsigned int number)
{
    unsigned int j;
    unsigned char addr0, addr1, addr2;
    address = address & 0xffffff00; //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable();

    SPIM_CSLow();  				//Spi cs assign to this pin,select
    SPIM_TXEn(); 				  //Transmit Enable bit TXEN
    SPIMSendByte(PP); 			//SPDAT = PP;CheckStatus();
    SPIMSendByte(addr0);
    SPIMSendByte(addr1);
    SPIMSendByte(addr2);

    for(j = 0; j < number; j++)
    {
        SPIMSendByte(*(p++));
    }


    SPIM_TXDisable(); 			//disable TXEN
    SPIM_CSHigh();  				//Spi cs release

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

    SPIM_CSLow();  			//Spi cs assign to this pin,select
    SPIM_TXEn(); 				//Transmit Enable bit TXEN
    SPIMSendByte(SE); 		//SPDAT = PP;CheckStatus();
    SPIMSendByte(addr0);
    SPIMSendByte(addr1);
    SPIMSendByte(addr2);
    SPIM_TXDisable(); 		//disable TXEN
    SPIM_CSHigh();  			//Spi cs release

    SPIM_checkStatus();
}

void SPIM_BlockErase()
{
    SPIM_WriteEnable();

    SPIM_CSLow();  		//Spi cs assign to this pin,select
    SPIM_TXEn(); 			//Transmit Enable bit TXEN
    SPIMSendByte(BE); 	//SPDAT = PP;CheckStatus();
    SPIM_TXDisable(); 	//disable TXEN
    SPIM_CSHigh();  		//Spi cs release

    SPIM_checkStatus();
}
void SPIM_Close()
{
    SPIM_CSHigh();  		//Spi cs release
    SPI2->RXDNR = 0x1; //����1���ֽ�
}


void SPIM_Test()
{
    u32 i;
    for(i = 0; i < 256; i++)
    {
        tmpdata[i] = i * 2;
    }

    printf("spi2 test\r\n");
    SPIM_Init();//36M

    SPIM_ReadID();

    SPIM_SectorErase(0);

    SPIM_PageProgram(256, tmpdata, 256);

    for(i = 0; i < 256; i++)
    {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(256, rxtmpdata, 256); 		//��д��ȥ��һҳ256�ֽڶ�����

    for(i = 0; i < 10; i++)
    {
        printf("rx[%d]=0x%x\r\n", i, rxtmpdata[i]);
    }
    printf("spi2 test over\r\n");

}



