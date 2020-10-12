/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbreg.h
 *      Purpose: Hardware Layer Definitions for ST STM32F10x
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef __USBREG_H
#define __USBREG_H
#include <stdint.h>

#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
typedef unsigned int      uint32_t;

#define REG(x)  (*((volatile unsigned int *)(x)))

#define USB_BASE_ADDR   0x40005C00  /* USB Registers Base Address */
/* EPX_INT_STAT Registers */
#define EPxINTSTATE(x)	REG(USB_BASE_ADDR + 0x20 + 4*(x-1))
#define EPxINTEN(x)	REG(USB_BASE_ADDR + 0x40 + 4*(x-1))
#define EPxAVAIL(x)	REG(USB_BASE_ADDR + 0x100 + 4*(x))
#define EPxCTRL(x)	REG(USB_BASE_ADDR + 0x140 + 4*(x))
#define EPxFIFO(x)	REG(USB_BASE_ADDR + 0x160 + 4*(x))

//request type
#define     USB_REQ_TYPE_MASK     				0x60     
#define     USB_REQ_TYPE_STANDARD 				0x00
#define     USB_REQ_TYPE_CLASS    				0x20
#define     USB_REQ_TYPE_VENDOR   				0x40
#define     USB_REQ_TYPE_RESV	    			0xE0

/* Common Registers */

#define EP_OUT_STAT			0x001C		/* OUT Status */
#define EP_IN_STAT			0x00E0		/* IN Status */

#endif  /* __USBREG_H */
