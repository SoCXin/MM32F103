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
