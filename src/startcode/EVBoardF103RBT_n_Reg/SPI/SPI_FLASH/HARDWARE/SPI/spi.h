#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define READ        	0x03
#define FAST_READ   	0x0B
#define RDID        	0x9F
#define WREN 			0x06
#define WRDI 			0x04
#define SE 				0xD8
#define BE 				0xC7
#define PP 				0x02
#define RDSR 			0x05
#define WRSR 			0x01
#define DP 				0xB9
#define RES 			0xAB

void data_clear(u8 *data, u8 size);
void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count);
void SPIM_CSLow(void);
void SPIM_CSHigh(void);
void SPIM_TXEn(void);
void SPIM_TXDisable(void);
void SPIM_RXEn(void);
void SPIM_RXDisable(void);
void SPIMSendByte(unsigned char tx_data);
unsigned char SPIM_Readbyte(void);
void SPIM_Init(void);
u8 SPIM_ReadID(void);
void SPIM_WriteEnable(void);
void SPIM_checkStatus(void);
void SPIM_WriteDisable(void);
void SPIM_PageRead(unsigned long address, unsigned char *p, unsigned int number); //page = 256 bytes
void SPIM_PageProgram(unsigned long address, unsigned char *p, unsigned int number);
void SPIM_SectorErase(unsigned long address);
void SPIM_BlockErase(void);
void SPIM_Close(void);

#endif

















