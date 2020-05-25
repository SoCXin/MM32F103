/**
******************************************************************************
* @file    i2c.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the i2c inc file for the library.
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
#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

void I2CInitMasterMode(I2C_TypeDef *I2Cx, unsigned int uiI2C_speed);
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx, unsigned char deviceaddr);
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx);
void I2CRXFullCheck(I2C_TypeDef *I2Cx);
void I2CTXByte(I2C_TypeDef *I2Cx, unsigned char data);
unsigned char I2CRXByte(I2C_TypeDef *I2Cx);
void I2CRXGroup(I2C_TypeDef *I2Cx, u16 rx_count, u8 *data_buf);
void E2PROM_Write(	I2C_TypeDef *I2Cx, \
                    unsigned short mem_byte_addr, \
                    unsigned short tx_count, \
                    unsigned char *tx_data  );

void  E2PROM_Read(	I2C_TypeDef *I2Cx, \
                    unsigned short mem_byte_addr, \
                    unsigned char rx_count, \
                    unsigned char *rx_data );



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
