#include "sys.h"
#include "i2c.h"
#include "uart.h"

#define FLASH_DEVICE_ADDR 0xA0
unsigned char tx_buffer0[8] = {0, 1, 2, 3, 4, 5, 6, 7};
unsigned char rx_buffer0[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void I2C_Master_Init()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //使能GPIOB时钟
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //使能I2C1时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //RCC->APB2ENR|=1<<3;

    AFIO->MAPR |= 1 << 1;
    GPIOB->CRH = 0x000000ff; //PB8和PB9复用开漏输出

    I2C1->IC_CON = 0x163;
    I2C1->IC_TAR = FLASH_DEVICE_ADDR >> 1;
    I2C1->IC_SS_SCL_HCNT = 0x77;
    I2C1->IC_SS_SCL_LCNT = 0x55;
    I2C1->IC_RX_TL = 0x0; //set Rx FIFO threshold level
    I2C1->IC_TX_TL = 0x1; //set Tx FIFO threschold level
    I2C1->IC_INTR_MASK = 0; //disable all inttrupt
    I2C1->IC_ENABLE = 0x1;

}

void I2CTXEmptyCheck()
{
    while(1)
    {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY))//发送缓冲区空
            break;
    }
}

void I2CRXFullCheck()
{

    I2C1->IC_DATA_CMD = I2C_DATA_CMD_CMD;//读数据指令，不可省略

    while(1)
    {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_RX_FULL))//接收缓冲区非空
            break;
    }

}



void I2CTXByte(u8 data)
{


    I2C1->IC_DATA_CMD = data;//发送数据
    while( (I2C1->IC_STATUS & 0x0004) == 0 );
    //I2CTXEmptyCheck();
}


unsigned char I2CRXByte()
{
    unsigned char temp;
    I2CRXFullCheck();
    temp = I2C1->IC_DATA_CMD;
    return temp;
}

void i2cStop(void)
{
    u16 overTime = 3000;

    I2C1->IC_ENABLE |= 0x02;

    while(I2C1->IC_ENABLE & 0x02)
    {
        if(0 == overTime-- )
        {
            break;
        }
    }

    I2C1->IC_CLR_TX_ABRT;
}



void I2CMasterWrite(unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data)
{
    unsigned short temp;
    unsigned short i;
    unsigned char *p;
    p = tx_data;
    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(temp);  //tx memory addr

    for(i = 0; i < tx_count; i++)
    {
        temp = *p;
        p++;
        I2CTXByte(temp); //tx data

    }

    i2cStop();
}

void I2CMasterRead(unsigned short rx_count )
{
    unsigned short i;

    I2CTXByte(8);
    for(i = 0; i < rx_count; i++)
    {
        rx_buffer0[i] = I2CRXByte();
    }
    i2cStop();
    for(i = 0; i < rx_count; i++)
    {
        printf("RX data is  : %x \r\n", rx_buffer0[i]);
    }
}
void I2CMasterTest()
{
    u32 i;
    I2C_Master_Init();
    I2CMasterWrite(8 * 1, 8, tx_buffer0);
    for(i = 0; i < 8 ; i ++)
    {
        printf("TX data is: %x \r\n", tx_buffer0[i]);
    }
    i = 80000; while(i--);						//e2prom 操作间隔延时

    I2CMasterRead(8);
    printf("i2c test over\r\n");
}






