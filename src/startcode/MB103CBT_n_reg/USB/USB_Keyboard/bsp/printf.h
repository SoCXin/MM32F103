#ifndef __PRINTF_H_
#define __PRINTF_H_

#include "HAL_device.h"
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

/*ʵ�ִ�ӡ�������������ͷ�ļ�,Ҳ���Զ���void outbyte(char c),���͵�����˿�
**unsigned char inbyte(UART_TypeDef* UARTx)���պ���
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

/*���ڴ�ӡ����,Ĭ�ϴ���1,����uart_test.c�ļ���outbyte(char c)������ѡ�񴮿�*/
void uart_printf( const charptr ctrl1, ...);
#endif
