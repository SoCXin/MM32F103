/*----------------------------------------------------------------------------
 *	  RL-ARM - USB
 *----------------------------------------------------------------------------
 *	  Name:	usbd_MM32F103.c
 *	  Purpose: Hardware Layer module for ST MM32F103
 *	  Rev.:	V4.70
 *----------------------------------------------------------------------------
 *	  This code is part of the RealView Run-Time Library.
 *	  Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/* Double Buffering is not yet supported									  */

#define __STM32

#include <RTL.h>
#include <rl_usb.h>
#include "usbreg.h"
#include "HAL_device.h"

#include "DAP_config.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USB_DBL_BUF_EP	0x0000

#define EP_BUF_ADDR		(sizeof(EP_BUF_DSCR) * (USBD_EP_NUM + 1))	/* Endpoint Buf Adr */



volatile uint8_t   usb_running_state;  							// running stage
volatile uint8_t   usb_running_ctrl_state;  						// running stage
volatile uint8_t   bmRequestType;
volatile uint8_t   bRequest;
volatile uint16_t  wValue;
volatile uint16_t  wIndex;
volatile uint16_t  wLength;
volatile uint8_t   req_dir;										//bmRequestType[7];
volatile uint8_t   req_type;										//bmRequestType[6:5];
volatile uint8_t   req_recip;										//bmRequestType[4:0];

// Endpoint Free Buffer Address
U16 FreeBufAddr;

U8 EP2ReceiveFlag = 0;
U8 EP2TransferFlag  = 0;
U8 RxBufLen = 0;
U8 TxBufLen = 0;
U8 EP2RXBuff[256];
U8 EP2TXBuff[256];

/**
 * @brief	USB Device Interrupt enable
 *			Called by USBD_Init to enable the USB Interrupt
 * @return	None
 */
#ifdef __RTX
void __svc(1) USBD_IntrEna	(void);
void __SVC_1				(void)
{
#else
	
void	USBD_IntrEna (void)
{
#endif
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
}


/**
 * @brief	USB Device Initialize Function
 *			Called by the User to initialize USB
 * @return	None
 */
//USB初始化
void USBD_Init (void)
{
//	int i,temp;
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;					//使能USB时钟
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);				//USB中断使能
	PORT_USB_CONNECT_SETUP();	
	USB->rADDR = 0;										//USB设备地址默认为0
	
}

/*
 *  USB Device Connect Function
 *	Called by the User to Connect/Disconnect USB Device
 *	Parameters:	  con:	Connect/Disconnect
 *	Return Value:	None
 */
//开启模拟单元，使USB处于复位状态，直到模拟单元准备完毕
/*必须配置CNTR寄存器PDWN位开启USB收发器相关的模拟部分,使能复位中断,主机将会检测到USB设备的复位，给出复位事件给USB设备*/
void USBD_Connect (BOOL con)
{
	if (con)
	{
		PIN_USB_CONNECT_ON();
		
		USB->rTOP  = USB_TOP_RESET;//reset usb
		USB->rTOP  &= ~USB_TOP_RESET;
		USB->rTOP  &= ~USB_TOP_CONNECT ;//usb disconnect
		
		USB->rINT_EN = USB_INT_EN_RSTIE | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE | USB_INT_EN_EPINTIE ;//使能USB相关中断：复位，挂起，恢复，端点中断   
		
		USB->rTOP = USB_TOP_CONNECT|((~USB_TOP_SPEED)&0x01);//enter work
		USB->rPOWER = USB_POWER_SUSPEN | USB_POWER_SUSP;
	}
	else
	{

		PIN_USB_CONNECT_OFF();	
	}
}

/*
 *  USB Device Reset Function
 *	Called automatically on USB Device Reset
 *	Return Value:	None
 */
//USB设备功能初始化
void USBD_Reset (void)
{

	USB->rTOP  |= USB_TOP_RESET;//reset usb
	USB->rTOP  &= ~USB_TOP_RESET;
		
    USB->rINT_STATE |= 0;//清USB状态
    USB->rEP_INT_STATE |= 0;
    USB->rEP0_INT_STATE |= 0;
    USB->rEP1_INT_STATE |= 0;
    USB->rEP2_INT_STATE |= 0;
    USB->rEP3_INT_STATE |= 0;
    USB->rEP4_INT_STATE |= 0;
    
    USB->rEP0_CTRL = 0;//清数据
    USB->rEP1_CTRL = 0;
    USB->rEP2_CTRL = 0;
    USB->rEP3_CTRL = 0;
    USB->rEP4_CTRL = 0;
	
	USBD_ClearEPBuf (0);
	USBD_ClearEPBuf (1);
	USBD_ClearEPBuf (2);
	USBD_ClearEPBuf (3);
	USBD_ClearEPBuf (4);
	
	USB->rINT_EN = USB_INT_EN_RSTIE | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE | USB_INT_EN_EPINTIE ;//使能USB相关中断
	USB->rEP_INT_EN = EP_INT_EN_EP0IE | EP_INT_EN_EP1IE | EP_INT_EN_EP2IE | EP_INT_EN_EP3IE | EP_INT_EN_EP4IE;//使能USB端点中断
	
	USB ->rEP0_INT_EN = EPn_INT_EN_SETUPIE | EPn_INT_EN_ENDIE | EPn_INT_EN_INACKIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE;//使能USB端点0中断
//	USB->rEP1_INT_EN = EPn_INT_EN_ENDIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_INACKIE | EPn_INT_EN_INSTALLIE;		//CDC Interrupt transfer
	USB->rEP2_INT_EN = EPn_INT_EN_ENDIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_INACKIE | EPn_INT_EN_INSTALLIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE;		//CDC Bulk transfer
	USB->rEP3_INT_EN = EPn_INT_EN_ENDIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_INACKIE | EPn_INT_EN_INSTALLIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE;		//HID Interrupt transfer

	USB->rEP_EN = EP_EN_EP0EN | EP_EN_EP1EN | EP_EN_EP2EN | EP_EN_EP3EN;	//开端点0&1&2&3中断
	
	USB->rTOP = USB_TOP_CONNECT|((~USB_TOP_SPEED)&0x01);//enter work
	USB->rPOWER = USB_POWER_SUSPEN | USB_POWER_SUSP;
}

/*
 *  USB Device Suspend Function
 *	Called automatically on USB Device Suspend
 *	Return Value:	None
 */
//USB挂起
void USBD_Suspend (void)
{
//	uint32_t i = 50000;
//	USB->rPOWER = USB_POWER_SUSPEN;
//	while (i--)   __nop();
}

/*
 *  USB Device Resume Function
 *	Called automatically on USB Device Resume
 *	Return Value:	None
 */
void USBD_Resume (void)
{
	/* Performed by Hardware	*/
}

/*
 *  USB Device Remote Wakeup Function
 *	Called automatically on USB Device Remote Wakeup
 *	Return Value:	None
 */
void USBD_WakeUp (void)
{
    USB->rPOWER = USB_POWER_WKUP|USB_POWER_SUSPEN|USB_POWER_SUSP;//唤醒
	USB->rPOWER = USB_POWER_SUSPEN|USB_POWER_SUSP;//进入工作模式
}

/*
 *  USB Device Remote Wakeup Configuration Function
 *	Parameters:	  cfg:	Device Enable/Disable
 *	Return Value:	None
 */
void USBD_WakeUpCfg (BOOL cfg)
{
	/* Not needed */
}

/*
 *  USB Device Set Address Function
 *	Parameters:		adr:	USB Device Address
 *					setup: Called in setup stage (!=0), else after status stage
 *	Return Value:	None
 */
 //2
void USBD_SetAddress (U32 adr, U32 setup)
{
	if (!setup)
	{
		USB->rADDR = adr;
	}
}

/*
 *  USB Device Configure Function
 *	Parameters:		cfg:	Device Configure/Deconfigure
 *	Return Value:	None
 */
 //3
void USBD_Configure (BOOL cfg)
{

}



/*
 *  Configure USB Device Endpoint according to Descriptor
 *	Parameters:	  pEPD:  Pointer to Device Endpoint Descriptor
 *	Return Value:	None
 */
 //4
void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD)
{
	
}

/*
 *  Set Direction for USB Device Control Endpoint
 *	Parameters:		dir:	Out (dir == 0), In (dir <> 0)
 *	Return Value:	None
 */
void USBD_DirCtrlEP (U32 dir)
{
	/* Not needed */
}

/*
 *  Enable USB Device Endpoint
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
 //5
void USBD_EnableEP (U32 EPNum)
{
	U32 num;
	num = EPNum & 0x0F;
	
	num == 0 ? (USB ->rEP0_INT_EN |= EPn_INT_EN_SETUPIE | EPn_INT_EN_ENDIE | EPn_INT_EN_INACKIE 						\
						| EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE ):	\
				(EPxINTEN(num) |= EPn_INT_EN_ENDIE |  EPn_INT_EN_INNACKIE | EPn_INT_EN_INACKIE 							\
						| EPn_INT_EN_INSTALLIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE);
}

/*
 *  Disable USB Endpoint
 *	Parameters:		EPNum: Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
 //6
void USBD_DisableEP (U32 EPNum)
{
	U32 num, val;
	num = EPNum & 0x0F;
	val = (num == 0 ? (USB ->rEP0_INT_EN) : EPxINTEN(num));
	
	num == 0 ? (USB ->rEP0_INT_EN |= val & (EPn_INT_EN_ENDIE | EPn_INT_EN_SETUPIE)) : \
						(EPxINTEN(num) |= val & EPn_INT_EN_ENDIE);
}

/*
 *  Reset USB Device Endpoint
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
void USBD_ResetEP (U32 EPNum)
{
	U32 num, val;
	num = EPNum & 0x0F;
	val = (num == 0 ? (USB ->rEP0_INT_EN) : EPxINTEN(num));
	
	num == 0 ? (USB ->rEP0_INT_EN |= val & (EPn_INT_EN_ENDIE | EPn_INT_EN_SETUPIE)) : \
						(EPxINTEN(num) |= val & EPn_INT_EN_ENDIE);
}


/*
 *  Set Stall for USB Device Endpoint
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
void USBD_SetStallEP (U32 EPNum)
{
	USB ->rEP_HALT = 1 << 0;
}

/*
 *  Clear Stall for USB Device Endpoint
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
void USBD_ClrStallEP (U32 EPNum)
{

}


/*
 *  Clear USB Device Endpoint Buffer
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *	Return Value:	None
 */
void USBD_ClearEPBuf (U32 EPNum)
{
	U32 num,temp,i;
	num = EPNum & 0x0F;
	for(i = 0;i < (EPxAVAIL(num) & 0x7f);i++)
	{
		temp = EPxFIFO(num);
	}
}


/*
 *  Read USB Device Endpoint Data
 *	Parameters:		EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *						pData: Pointer to Data Buffer
 *	Return Value:	Number of bytes read
 */

char usbSetupPack = 0;
char usbEp0InPack = 0;
char usbEp0OutnPack = 0;

U32 USBD_ReadEP (U32 EPNum, U8 *pData)
{
	/* Double Buffering is not yet supported	*/
	U32 num, cnt = 0, *pv, n;

	num = EPNum & 0x0F;

	if((num == 0) && (usbSetupPack ==1))
	{
		cnt = 8;
		pv = (U32*)(USB->rSETUP);
		for (n = 0; n < 8 ; n++)
		{
			*(pData) = (*pv++) & 0xff;
			pData ++;
		}
	}
	else
	{
		usbEp0InPack = 0;
		cnt = EPxAVAIL(num);
		for (n = 0; n < cnt ; n++)
		{
			*(pData) = EPxFIFO(num);
			pData ++;
		}
	}
	usbSetupPack = 0;
	usbEp0OutnPack = 0;
	return (cnt);
}

/*
 *  Write USB Device Endpoint Data
 *	Parameters:	  EPNum: Device Endpoint Number
 *						EPNum.0..3: Address
 *						EPNum.7:	Dir
 *					 pData: Pointer to Data Buffer
 *					 cnt:	Number of bytes to write
 *	Return Value:	Number of bytes written
 */

U32 USBD_WriteEP (U32 EPNum, U8 *pData, U32 cnt)
{
	/* Double Buffering is not yet supported									*/
	U32 num,  n;
	num = EPNum & 0x0F;
	while(EPxCTRL(num) & 0x80);
	
	for (n = 0; n < cnt; n++)
	{
		EPxFIFO(num)= *(pData + n);
	}
	EPxCTRL(num)=  0x80 | cnt;
	return (cnt);
}

/*
 *  Get USB Device Last Frame Number
 *	Parameters:		None
 *	Return Value:	Frame Number
 */
U32 USBD_GetFrame (void)
{
	return 0;
}

#ifdef __RTX
U32 LastError;						  /* Last Error						 */

/*
 *  Get USB Last Error Code
 *	Parameters:	  None
 *	Return Value:	Error Code
 */
U32 USBD_GetError (void)
{
	return (LastError);
}
#endif

/*
 *  USB Device Interrupt Service Routine
 */
//USB主机发送给USB设备复位事件
//void USB_LP_CAN1_RX0_IRQHandler(void)
extern u8 usb_buf_busy_flag ;
void USB_HP_CAN1_TX_IRQHandler(void)
{

	int i = 0;
	u8 temp;
  	U32  num = 0;

	U32 intistr, temp_ep,temp_epn;
	
	intistr = USB->rINT_STATE;		//读取USB中断状态
	USB->rINT_STATE = intistr;		//清USB中断状态,写1清零
	
	if(intistr & USB_INT_STATE_RSTF)//got host reset interrupt
	{															
		USBD_Reset();
		usbd_reset_core();
		if (USBD_P_Reset_Event)
		{
			USBD_P_Reset_Event();
		}
	}	
	else if(intistr & USB_INT_STATE_SUSPENDF )
	{
		USBD_Suspend();
		if (USBD_P_Suspend_Event)
		{
			USBD_P_Suspend_Event();
		}
	}
	else if(intistr & USB_INT_STATE_RESUMF  )
	{
		USBD_WakeUp();
		if (USBD_P_Resume_Event)
		{
			USBD_P_Resume_Event();
		}
	}
	else if(intistr & USB_INT_STATE_EPINTF)
	{	
		temp_ep = USB->rEP_INT_STATE;			//读取端点中断号
		USB->rEP_INT_STATE = temp_ep;			//清端点号中断
		temp_ep &= USB->rEP_INT_EN;				//避免未始能端点影响
		
		if(temp_ep & EP_INT_STATE_EP0F)		//端点0中断判断		
		{
			num = 0;
			temp_epn = USB->rEP0_INT_STATE;		//读取端点0中断状态位
			USB->rEP0_INT_STATE = temp_epn;		//清端点0中断	
			temp_epn &= USB->rEP0_INT_EN;
			
			if(temp_epn & EPn_INT_STATE_SETUP)	//SETUP中断
			{
				usbSetupPack = 1;
				USBD_P_EP[num](USBD_EVT_SETUP);	//tup包处理
			}
			
			if(temp_epn & EPn_INT_STATE_INACK)	//IN包应答中断，准备写入数据
			{
				usbEp0InPack = 1;
				if(USBD_P_EP[num])
				{
					USBD_P_EP[0](USBD_EVT_IN);
				}		
			}
			
			if(temp_epn & EPn_INT_STATE_OUTACK)	//OUT包应答中断，收到数据
			{
				usbEp0OutnPack = 1;
				if(USBD_P_EP[num])
				{
					USBD_P_EP[num](USBD_EVT_OUT);
				}
				//OUT包处理	
			}
			
			if(temp_epn & (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE))
			{			
			
			}
			if(temp_epn & EPn_INT_STATE_INSTALL)
			{			

			}
			if(temp_epn & EPn_INT_STATE_OUTSTALL)
			{			
					
			}
		}
		if(temp_ep & (~EP_INT_STATE_EP0F))	//非端点0
		{
			for(i = 1;i < 5 ;i++)
			{
				if((temp_ep >> i) & 0x01)
				{
					num = i;
					break;
				}
			}

			temp_epn = EPxINTSTATE(num);
			EPxINTSTATE(num) = temp_epn ;
			temp_epn &= EPxINTEN(num);

			if(temp_epn & EPn_INT_STATE_INNACK)
			{
				if(num == 2)
				{
					if(EP2ReceiveFlag == 0)
						EP2TransferFlag = 1;
				}
			}
			if(temp_epn & EPn_INT_STATE_INACK)							//IN包非应答中断，准备写入数据
			{
				if(num == 2)
				{
					usb_buf_busy_flag = 0;
//					if(EP2ReceiveFlag == 0)
//						EP2TransferFlag = 1;
				}
				else if (USBD_P_EP[num])
				{
					USBD_P_EP[num](USBD_EVT_IN);
				}		
			}
			if(temp_epn & EPn_INT_STATE_OUTACK)							//OUT包应答中断，收到数据
			{
				if(num == 2)
				{
					usbEp0InPack = 0;
					if(usb_buf_busy_flag == 0)
					{
						RxBufLen = EPxAVAIL(2) & 0x7f;
						for (i = 0; i < RxBufLen ; i++)
						{
							*(EP2RXBuff + i) = EPxFIFO(2);
						}
						EP2ReceiveFlag = 1;
					}
					else
					{
						USB->rTOP |= 1<<3;
						usb_buf_busy_flag = 2;
						USB->rTOP &= ~(1<<3);
					
					}
				}
				else if (USBD_P_EP[num])
				{
					USBD_P_EP[num](USBD_EVT_OUT);
				}

				//OUT包处理	
			}			
			if(temp_epn & (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE))
			{			
				
			}
			if(temp_epn & EPn_INT_STATE_INSTALL)
			{			

			}
			if(temp_epn & EPn_INT_STATE_OUTSTALL)
			{			
					
			}
		}
	}
}
