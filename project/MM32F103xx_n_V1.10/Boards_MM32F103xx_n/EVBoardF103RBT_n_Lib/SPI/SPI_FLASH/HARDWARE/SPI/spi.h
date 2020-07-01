/**
******************************************************************************
* @file    spi.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the spi inc file for the library.
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

#ifndef __SPI_H__
#define __SPI_H__
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

//#define SPI_INT_MODE
//#define SPI_DMA
void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div);
u8 SPIM_ReadID(SPI_TypeDef* SPIx);
void SPIM_WriteEnable(SPI_TypeDef* SPIx);
void SPIM_checkStatus(SPI_TypeDef* SPIx);
void SPIM_WriteDisable(SPI_TypeDef* SPIx);
void SPIM_PageRead(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number);
void SPIM_PageProgram(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number);
void SPIM_SectorErase(SPI_TypeDef* SPIx, unsigned long address);
void SPIM_BlockErase(SPI_TypeDef* SPIx);
void SPIM_Close(SPI_TypeDef* SPIx);
void data_clear(u8 *data, u8 size);

#endif


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
