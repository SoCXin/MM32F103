#include "sys.h"
#include "spi.h"
#include "uart.h"

unsigned char tmpdata[256];
unsigned char rxtmpdata[256];


//清除数组里的数据
void data_clear(u8 *data, u8 size)
{
    for(size = size; size > 0; size--)
    {
        *data = 0;
        data++;
    }
}
//数据对比
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
    SPI2->SCSR &= 0xe; //片选拉低
}

void SPIM_CSHigh()
{
    SPI2->SCSR |= 0x1; //片选拉低
}
void SPIM_TXEn()
{
    SPI2->GCTL |= SPI_GCTL_TXEN;//使能能SPI2发送
}

void SPIM_TXDisable()
{
    SPI2->GCTL &= ~SPI_GCTL_TXEN;//失能SPI2发送
}

void SPIM_RXEn()
{
    SPI2->GCTL |= SPI_GCTL_RXEN;//使能能SPI1接收
}

void SPIM_RXDisable()
{
    SPI2->GCTL &= ~SPI_GCTL_RXEN;//失能SPI1接收
}

void SPIMSendByte(unsigned char tx_data)
{
    SPI2->TXREG = tx_data;
    while(1)
    {
        if(SPI2->CSTAT & SPI_CSTAT_TXEPT) //SPI1发送完成标志位
        {
            break;
        }
    }
}

unsigned char SPIM_Readbyte()
{

    while(1)
    {
        if(SPI2->CSTAT & SPI_CSTAT_RXAVL) //SPI1接收完成标志位
        {
            break;
        }
    }

    return SPI2->RXREG;

}

void SPIM_Init()
{

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; //开启SPI1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //开启GPIOA时钟

    SPIM_CSHigh();
    GPIOB->CRH &= 0x0000FFFF; //设置PB12,PB13,PB14,PB15为缺省值

    GPIOB->CRH |= 0xB0BB0000; //PB15,PB13推挽复用输出
    GPIOB->CRH |= 0x08000000; //PB14上拉输入
    GPIOB->ODR |= GPIO_ODR_ODR14;
    SPI2->GCTL |= SPI_GCTL_MM; //主机模式
    SPI2->CCTL |= SPI_CCTL_SPILEN; //8位数据宽度

    SPI2->CCTL &= ~SPI_CCTL_CPOL; //SPI_CPOL_Low
    SPI2->CCTL |= SPI_CCTL_CPHA; //SPI_CPHA_1Edge

    SPI2->GCTL &= ~SPI_GCTL_NSS_SEL; //软件控制NSS
    SPI2->CCTL |= 0x10; //开启高速模式
    SPI2->SPBRG = 0x2; //2分频，波特率应该为72M/2=36M
    SPI2->CCTL &= ~SPI_CCTL_LSBFE; //MSB
    SPI2->GCTL |= SPI_GCTL_SPIEN; //使能SPI

}

u8 SPIM_ReadID()
{
    unsigned char temp;
    unsigned int i;
    SPIM_CSLow();																			//Spi cs assign to this pin,select
    SPIM_TXEn(); 																			//Transmit Enable bit TXEN
    SPIMSendByte(RDID);
    SPIM_TXDisable(); 	//disable TXEN

    SPI2->RXDNR = 0x1; //接收1个字节

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

    SPI2->RXDNR = 0x1; //接收1个字节
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

    SPI2->RXDNR = 0x1; //接收1个字节
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
    SPI2->RXDNR = 0x1; //接收1个字节
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
    SPIM_PageRead(256, rxtmpdata, 256); 		//把写进去的一页256字节读出来

    for(i = 0; i < 10; i++)
    {
        printf("rx[%d]=0x%x\r\n", i, rxtmpdata[i]);
    }
    printf("spi2 test over\r\n");

}



