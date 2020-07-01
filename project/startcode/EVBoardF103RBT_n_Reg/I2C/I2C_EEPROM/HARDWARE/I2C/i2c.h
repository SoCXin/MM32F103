#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

void I2C_Master_Init(void);
void I2CSetDeviceAddr(unsigned char deviceaddr);
void I2CTXEmptyCheck(void);
void I2CRXFullCheck(void);
void I2CTXByte(u8 data);
unsigned char I2CRXByte(void);
void I2CRXGroup(u16 rx_count, u8 *data_buf);
void E2PROM_Write(unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data);
void E2PROM_Read( unsigned short mem_byte_addr, unsigned char rx_count, unsigned char *rx_data );

#endif

















