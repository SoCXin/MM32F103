#include "sys.h"
#include "i2c.h"
#include "uart.h"

#define FLASH_DEVICE_ADDR 0xA0
unsigned char tx_buffer0[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};


void I2C_Master_Init()
{

    I2C2->IC_ENABLE = 0x0;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //使能GPIOB时钟
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; //使能I2C1时钟

    GPIOB->CRH &= ~0x0000ff00;
    GPIOB->CRH |= 0x00008800; //PB10和PB11上拉输入
    GPIOB->ODR |= 3 << 10; //PB10和PB11上拉输入

    I2C2->IC_CON = 0x163;
    I2C2->IC_SS_SCL_HCNT = 0x77;
    I2C2->IC_SS_SCL_LCNT = 0x55;
    I2C2->IC_RX_TL = 0x0; //set Rx FIFO threshold level
    I2C2->IC_TX_TL = 0x0; //set Tx FIFO threschold level
    I2C2->IC_INTR_MASK = 0; //disable all inttrupt
    I2C2->IC_ENABLE = 0x3;

    GPIOB->CRH &= ~0x0000ff00;
    GPIOB->CRH |= 0x0000ff00; //PB10和PB11复用开漏输入
}

void I2CSetDeviceAddr(unsigned char deviceaddr)
{

    GPIOB->CRH &= ~0x0000ff00;
    GPIOB->CRH |= 0x00008800; //PB10和PB11上拉输入
    GPIOB->ODR |= 3 << 10; //PB10和PB11上拉输入

    I2C2->IC_ENABLE &= ~0x1;
    I2C2->IC_TAR = deviceaddr >> 1;
    I2C2->IC_ENABLE |= 0x1;

    GPIOB->CRH &= ~0x0000ff00;
    GPIOB->CRH |= 0x0000ff00; //PB10和PB11复用开漏输入

}
void I2CTXEmptyCheck()
{
    while(1)
    {
        if((I2C2->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY))//发送缓冲区空
            break;
    }
}

void I2CRXFullCheck()
{
    I2C2->IC_DATA_CMD = I2C_DATA_CMD_CMD;//读数据指令，不可省略
    while(1)
    {
        if((I2C2->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_RX_FULL))//接收缓冲区非空
            break;
    }
}



void I2CTXByte(u8 data)
{
    I2C2->IC_DATA_CMD = data; //发送数据
    I2CTXEmptyCheck();
}


unsigned char I2CRXByte()
{
    unsigned char temp;
    I2CRXFullCheck();
    temp = I2C2->IC_DATA_CMD;
    return temp;
}

void I2CRXGroup(u16 rx_count, u8 *data_buf)
{
    int i, data_cnt = 0;
    char flag = 0;
    for(i = 0; i < rx_count; i++)
    {
        while(1)
        {
            if(((I2C2->IC_STATUS & 0x2) != 0) && (flag == 0))
            {
                I2C2->IC_DATA_CMD = 0x0100;
                data_cnt++;
                if(data_cnt == rx_count)
                    flag = 1;
            }
            if((I2C2->IC_STATUS & 0x8))
            {
                data_buf[i] = I2C2->IC_DATA_CMD;
                break;
            }
        }
    }
}

void E2PROM_Write(unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data)
{
    unsigned short i;
    unsigned char *p;
    p = tx_data;

    I2CTXByte(mem_byte_addr);
    for(i = 0; i < tx_count; i++)
    {
        I2CTXByte(*p); //发送数据
        p++;
    }

    I2C2->IC_ENABLE |= 0x02;
    while(I2C2->IC_ENABLE & 0x02);
    I2C2->IC_CLR_TX_ABRT;
}


void E2PROM_Read( unsigned short mem_byte_addr, unsigned char rx_count, unsigned char *rx_data )
{
    u32 temp;
    E2PROM_Write(mem_byte_addr, 0, rx_data);//写设备地址
    I2CRXGroup(rx_count, rx_data);

    I2C2->IC_ENABLE |= 0x02;
    while(I2C2->IC_ENABLE & 0x02);
    I2C2->IC_CLR_TX_ABRT;

}







