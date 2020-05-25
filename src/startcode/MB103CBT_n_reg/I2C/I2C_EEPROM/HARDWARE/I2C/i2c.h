#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

void I2C_Master_Init(void);
void I2CTXEmptyCheck(void);
void I2CRXFullCheck(void);
void I2CTXByte(u8 data);
unsigned char I2CRXByte(void);
void I2CMasterWrite(unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data);
void I2CMasterRead(unsigned short rx_count);
void I2CMasterTest(void);


#endif

















