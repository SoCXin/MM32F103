/**
******************************************************************************
* @file    printf.h
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file contains all the printf inc file for the library.
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
#ifndef __PRINTF_H_
#define __PRINTF_H_

#include "sys.h"
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

/*实现打印功能须包含串口头文件,也可自定义void outbyte(char c),发送到任意端口
**unsigned char inbyte(UART_TypeDef* UARTx接收函数
*/
#include "uart_test.h"

/*----------------------------------------------------*/
/* Use the following parameter passing structure to   */
/* make uart_printf re-entrant.                        */
/*----------------------------------------------------*/
typedef struct params_s
{
    int len;
    int num1;
    int num2;
    char pad_character;
    int do_padding;
    int left_flag;
} params_t;

/*---------------------------------------------------*/
/* The purpose of this routine is to output data the */
/* same as the standard printf function without the  */
/* overhead most run-time libraries involve. Usually */
/* the printf brings in many kilobytes of code and   */
/* that is unacceptable in most embedded systems.    */
/*---------------------------------------------------*/

typedef char* charptr;
typedef int (*func_ptr)(int c);

/*串口打印函数,默认串口1,更改uart_test.c文件中outbyte(char c)函数可选择串口*/
void uart_printf( const charptr ctrl1, ...);
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

