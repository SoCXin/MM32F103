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
volatile unsigned char rx_flag = 0;
volatile unsigned char tx_flag = 0;

static u8 i2cData;
/********************************************************************************************************
**������Ϣ ��I2C1_EV_IRQHandler (void)
**�������� ��I2C1�жϺ�����ͨ���жϵķ�ʽ�ж��Ƿ���/��������
**������� ����
**������� ����
********************************************************************************************************/
void I2C1_EV_IRQHandler (void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY))
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_TX_EMPTY);
        I2C_ITConfig(I2C1, I2C_IT_TX_EMPTY, DISABLE);
        tx_flag = 1;
    }
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL))
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_RX_FULL);
        i2cData = I2C_ReceiveData(I2C1);
        rx_flag = 1;
    }
}
/********************************************************************************************************
**������Ϣ ��I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned long apb_mhz,unsigned long i2c_baud_rate) //unit is Khz
**�������� ����ʼ��I2C
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CInitMasterMode(I2C_TypeDef *I2Cx, unsigned int uiI2C_speed, unsigned char TarAddress)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    if(I2Cx == I2C1)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);//����I2C1 IO��
        GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);//I2C1��ӳ��
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1��ӳ��IO��
    }
    if(I2Cx == I2C2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;
    }
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//�������߿��м�CLK&DATAΪ��
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
    I2C_InitStructure.I2C_OwnAddress = 0;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = uiI2C_speed;

    I2C_Init( I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;				//��Ҫ�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    if(I2Cx == I2C1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    if(I2Cx == I2C2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    I2C_ITConfig(I2Cx, I2C_IT_RX_FULL, ENABLE);
}
/********************************************************************************************************
**������Ϣ ��I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)
**�������� ��ѡ��ӻ���ַ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2;deviceaddr:�ӻ���ַ
**������� ����
********************************************************************************************************/
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx, unsigned char deviceaddr)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    if(I2Cx == I2C1)
    {
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1��ӳ��IO��
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
**������Ϣ ��I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��cmd��temp
**������� ����
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx, unsigned char data)

{

    I2C_SendData(I2Cx, data);
    I2C_ITConfig(I2Cx, I2C_IT_TX_EMPTY, ENABLE);
    while(1)
    {
        if(	tx_flag == 1)
        {
            tx_flag = 0;
            break;
        }
    }
}

void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{

    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL))
        {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��I2CRXByte(I2C_TypeDef *I2Cx)
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ��temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    unsigned short temp;

    //	while(1)
    //	{
    //	if(	rx_flag == 1)
    //	{
    rx_flag = 0;
    //	break;
    //	}
    //	I2CRXFullCheck(I2Cx);
    I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL);

    while(rx_flag == 0);
    temp = i2cData;
    return (unsigned char)temp;
    //}
}

/********************************************************************************************************
**������Ϣ ��I2CRXGroup()
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx(I2C1,I2C2)
u16 rx_count(���ݸ���)
u8 *data_buf(��������ָ��)
**������� ��temp
********************************************************************************************************/
void I2CRXGroup(I2C_TypeDef *I2Cx, u16 rx_count, u8 *data_buf)
{
    int i;
    u32 j;
    //	char flag = 0;
    //	u16 delay=0;

    I2C_ITConfig(I2Cx, I2C_IT_RX_FULL, ENABLE);
    for(i = 0; i < rx_count; i++)
    {
        data_buf[i] = I2CRXByte(I2Cx);
    }

    for(j = 0; j < 0x5000; j++);

}

/********************************************************************************************************
**������Ϣ ��	I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
**�������� ��	I2C��������
**������� ��
I2C_TypeDef *I2Cx(I2C1,I2C2)
unsigned short mem_byte_addr(E2PROM��ַ)
unsigned short tx_count(���ݳ���)
unsigned char *tx_data(����bufָ��)
**������� ����
********************************************************************************************************/
void E2PROM_Write(	I2C_TypeDef *I2Cx, \
                    unsigned short mem_byte_addr, \
                    unsigned short tx_count, \
                    unsigned char *tx_data  )
{
    u32 i;
    unsigned char *p;
    p = tx_data;
    I2CTXByte(I2Cx, mem_byte_addr);

    for(i = 0; i < tx_count; i++)
    {
        I2CTXByte(I2Cx, *p); //��������
        p++;
        //		if(i==(tx_count-1))
        //		I2Cx->IC_DATA_CMD =mem_byte_addr| 0x200; // //muaul set stop flag
        //		else
        //		I2CTXByte(I2Cx,mem_byte_addr); //tx data
    }
    i = 80000; while(i--);
    I2C_GenerateSTOP(I2Cx, ENABLE);

    //while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);
}

/********************************************************************************************************
**������Ϣ ��	I2CMasterRead()
**�������� ��	I2C��������
**������� ��
I2C_TypeDef *I2Cx(I2C1,I2C2)
unsigned short mem_byte_addr(E2PROM��ַ)
unsigned char rx_count(���ݳ���)
unsigned char *rx_data(����bufָ��)
**������� ����
********************************************************************************************************/
//void  E2PROM_Read(	I2C_TypeDef *I2Cx,\
//					unsigned short mem_byte_addr, \
//					unsigned char rx_count, \
//					unsigned char *rx_data )
//{
//	u32 i;
//	E2PROM_Write(I2Cx, mem_byte_addr, 0, rx_data);//д�豸��ַ
//	I2CRXGroup(I2Cx,rx_count,rx_data);
//	i =80000;while(i--);
//	//while((I2C_GetITStatus(I2Cx, I2C_IT_STOP_DET))==0);
//}
void  E2PROM_Read(I2C_TypeDef *I2Cx, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{

    unsigned char temp = 0;
    unsigned short i;
    // 	u32 k;
    I2C_Send7bitAddress(I2Cx, FLASH_DEVICE_ADDR, I2C_Direction_Transmitter);

    I2C_Cmd(I2Cx, ENABLE);

    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(I2Cx, temp);

    for(i = 0; i < rx_count; i++)
    {
        //	if(i==(rx_count-1))
        //			I2Cx->IC_DATA_CMD = 0x300;  //muaul set stop flag
        //	else
        rx_data[i] = I2CRXByte(I2Cx);
        //
        //	UartSendGroup((u8*)printBuf, sprintf(printBuf,"RX data%d is  : %x \r\n",i,rx_data[i]));
    }
    //	k =80000;while(k--);
    I2C_GenerateSTOP(I2Cx, ENABLE);
}
/********************************************************************************************************
**������Ϣ ��I2CMasterTest(I2C_TypeDef *I2Cx)
**�������� ��I2C���շ������ݲ��Գ���
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CMasterTest(I2C_TypeDef *I2Cx)
{
    unsigned int i;
    unsigned char chI2c_Txdata[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    unsigned char chI2c_Rxdata[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    printf("i2c test start ...\r\n");

    I2CInitMasterMode(I2Cx, 400000, FLASH_DEVICE_ADDR);
    I2CSetDeviceAddr(I2Cx, FLASH_DEVICE_ADDR);

    E2PROM_Write( I2Cx, 0, 8, chI2c_Txdata );

    i = 80000; while(i--);						//e2prom ���������ʱ

    E2PROM_Read( I2Cx, 0, 8, chI2c_Rxdata );

    for(i = 0; i < 8 ; i ++)
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
