//********************************************************************
//(Copyright 2012) Verisilicon Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Verisilicon Microelectronics Co.,Ltd
//
//File name 	: usb.h
//Author		:
//Verision		: V0.1
//Created		:
//Modified		: Houyue.Shi 2012.02.24
//Description	:
//
//********************************************************************
#ifndef		_USB_REG_H
#define		_USB_REG_H
#include "HAL_device.h"

#define USB_INTR_MODE


#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))


#define     USB_PAGE_SIZE           			(512)
#define     EPn_MAX_PACKAGE_SIZE    			(64)
#define     EP0_MAX_PACKAGE_SIZE    			(64)

#define     IRQ_USB_NUM										(12)
#define     USB_EP0                 			(0)
#define     USB_EPn(x)              			(x)
/*********************************************************************************/
/***************please note: only EP1 and EP2 support DMA mode********************/
/***************please note: EP1&EP2 don't support bi-direction @DMA mode*********/
/*********************************************************************************/
#define 	EPIN														(USB_EPn(1))
#define 	EPOUT														(USB_EPn(3))
/*********************************************************************************/
/***************for DMA, channel can be assigned anyway***************************/
/*********************************************************************************/
//#define 	DMA_TX_CH 											(0)
//#define 	DMA_RX_CH 											(1)

#define 	EPn_INT(x)										(1<<(x))

#define     EP_DIR_IN											(0x01) //read ep to host
#define     EP_DIR_OUT										(0x00) //write host to ep
//
#define     EPn_DIR_IN(x)									(EP_DIR_IN<<((x)-1)) //read ep to host
#define     EPn_DIR_OUT(x)								(EP_DIR_OUT<<((x)-1)) //write host to ep
//

#define 	USB_INT_ENABLE 								0x01

#define     USB_IDLE                			0xFF

#define     USB_CTRL_SETUP          			0x00
#define     USB_CTRL_IN       					0x01
#define     USB_CTRL_OUT      					0x02
#define     USB_CTRL_STATUS      				0x03


#define     MAX_STD_REQUEST         			13


#define 		EPn_INPORT(x)									(0x80|(x))
#define 		EPn_OUTPORT(x)								(0x00|(x))


//===================code that is from usb1.1 protocal===========================//
// Usb Request Type
//transfer dircetion
#define 		USB_REQ_DIR_MASK							0x80
#define     USB_REQ_DIR_OUT								0x00 //host to device
#define     USB_REQ_DIR_IN								0x80 //device to host
//type
#define     USB_REQ_TYPE_MASK     				0x60
#define     USB_REQ_TYPE_STANDARD 				0x00
#define     USB_REQ_TYPE_CLASS    				0x20
#define     USB_REQ_TYPE_VENDOR   				0x40
#define     USB_REQ_TYPE_RESV	    				0xE0
//recipient
#define 		USB_REQ_RECIP_MASK						0x1F
#define 		USB_REQ_RECIP_DEV							0x00
#define 		USB_REQ_RECIP_IF							0x01
#define 		USB_REQ_RECIP_EP							0x02
#define 		USB_REQ_RECIP_OTHER						0x03
#define 		USB_REQ_RECIP_RESV						0x04
//Feature
#define 		DEVICE_REMOTE_WAKEUP					0x01
#define 		ENDPOINT_HALT									0x00
// Standard Request Codes
#define     GET_STATUS             				0
#define     CLEAR_FEATURE          				1
#define     SET_FEATURE            				3
#define     SET_ADDRESS            				5
#define     GET_DESCRIPTOR         				6
#define     SET_DESCRIPTOR         				7
#define     GET_CONFIGURATION      				8
#define     SET_CONFIGURATION      				9
#define     GET_INTERFACE         				10
#define     SET_INTERFACE         				11
#define     SYNCH_FRAME           				12


// Descriptor Types
#define     DEVICE_DESC_TYPE            	0x0100
#define     CONFIGURATION_DESC_TYPE     	0x0200
#define     STRING_DESC_TYPE            	0x0300
#define     INTERFACE_DESC_TYPE         	0x0400
#define     ENDPOINT_DESC_TYPE          	0x0500
//Class
#define 		HID_REPORT_TYPE				  			0x2200

////for CSW
//#define   	CSW_CMD_PASSED								0x00
//#define   	CSW_CMD_FAILED								0x01
//#define   	CSW_PHASE_ERR									0x02



typedef unsigned long int 								uint32;
typedef unsigned short int  							uint16;
typedef unsigned char  										uint8;
typedef signed long int  									int32;
typedef signed short int 									int16;
typedef signed char  	 										int8;


typedef enum
{
    USB_STATE_ATTACHED = 0,
    USB_STATE_POWERED = 1,
    USB_STATE_DEFAULT = 2,
    USB_STATE_ADDRESS = 3,
    USB_STATE_CONFIGURED = 4,
    USB_STATE_SUSPENDED = 5,
} USB_STATE_t;


extern volatile uint8 		usb_running_state ;  // running stage
extern volatile uint8 		usb_running_ctrl_state ;  // running stage

extern volatile uint8 		USB_SEND_OPEN_STALL[5];
extern volatile uint8 		USB_EP_STALL[5];
extern volatile uint8 		USB_EP_IN_STALL[5];
extern volatile uint8 		USB_EP_OUT_STALL[5];

extern volatile uint8 		USB_FINISH_Flag[5];
extern volatile uint8 		usb_ep_flag;
extern volatile USB_STATE_t usb_state;

extern volatile uint8    	bmRequestType ;
extern volatile uint8    	bRequest ;
extern volatile uint16   	wValue ;
extern volatile uint16   	wIndex ;
extern volatile uint16   	wLength ;
extern volatile uint8    	req_dir;//bmRequestType[7];
extern volatile uint8    	req_type;//bmRequestType[6:5];
extern volatile uint8    	req_recip;//bmRequestType[4:0];


void usb_deal_cbw(void);
void usb_hid_report_handle(void);
void EP0_Send_Empty_Packet(void);
void Chap9_GetStatus( void);        // 0x00
void Chap9_ClearFeature( void);     // 0x01
void MLsup_StallEP0( void);         // reserve
void Chap9_SetFeature( void);	    // 0x03
void MLsup_StallEP1( void);         // reserve
void Chap9_SetAddress( void);	    // 0x05
void Chap9_GetDescriptor( void);    // 0x06
void Chap9_SetDescriptor( void);    // 0x07
void Chap9_GetConfiguration( void);	// 0x08
void Chap9_SetConfiguration( void);	// 0x09
void Chap9_GetInterface( void);		// 0x0a
void Chap9_SetInterface( void);		// 0x0b

void ctrl_in_token_handle( void) ;
void ctrl_out_handle( void) ;
void ctrl_trans_finish_handle( void ) ;
void ctrl_deal_handle(void);

void USB_Init( void);
void usb_setup_handle( void);
void usb_in_handle( void);
void usb_out_handle( void) ;
void usb_trans_finish( void);
void usb_reset_handle( void) ;
void usb_suspend_handle( void) ;
void usb_resume_handle( void) ;
void usb_wakeup_handle( void) ;
void USBTest(void);
void usb_test(void);
void usb_reset(void);

//CDC
void Class_Get_Line_Coding(void);
void Class_Set_Control_Line_State(void);
void Class_Set_Line_Coding(void);
void EP1_USB_IN_Data(void);
char UsbVcomSend(unsigned char*Info, unsigned int *infoLenth);
void EP3_USB_OUT_Data(void);
unsigned int UsbVComRecRec(unsigned char*rxInfo);


uint32 Read_Mreg32( uint32 mreg);
void Write_Mreg32( uint32 mreg, uint32 val);
uint8 read_mreg8( uint32 mreg);
void write_mreg8( uint32 mreg, uint8 val);
void usb_delay1ms(uint32 dly);
#endif

