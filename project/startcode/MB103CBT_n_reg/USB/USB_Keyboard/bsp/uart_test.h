#ifndef __USRT_TEST_H_
#define __USRT_TEST_H_
#include "HAL_device.h"

void          UartInit(UART_TypeDef* UARTx);
void          uart_send(UART_TypeDef* UARTx, char c);
unsigned int  Uart1RxTest(UART_TypeDef* UARTx);
void          outbyte (char);
unsigned char inbyte(UART_TypeDef* UARTx);
#endif
