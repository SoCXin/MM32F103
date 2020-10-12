//********************************************************************
//(Copyright 2017) Betterlife  All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Betterlife  Co.,Ltd
//
//File name 	: ctrlusb.c
//Author		  :
//Verision		:
//Created		  :
//Modified		:sfw 2017.08.21
//Description	:
//
//********************************************************************
#include "usb.h"
#include "uart.h"

volatile uint8   usb_running_state;  							// running stage
volatile uint8   usb_running_ctrl_state;  						// running stage
volatile uint8   bmRequestType;
volatile uint8   bRequest;
volatile uint16  wValue;
volatile uint16  wIndex;
volatile uint16  wLength;
volatile uint8   req_dir;										//bmRequestType[7];
volatile uint8   req_type;										//bmRequestType[6:5];
volatile uint8   req_recip;										//bmRequestType[4:0];
//=========================== Each Request =============================

//=================================== Handler================================
#if 1
void (*StandardDeviceRequest[MAX_STD_REQUEST])(void) =
{
    Chap9_GetStatus,		//0x00
    Chap9_ClearFeature,		//0x01
    MLsup_StallEP0,			//reserve
    Chap9_SetFeature,		//0x03
    MLsup_StallEP1,			//reserve
    Chap9_SetAddress,		//0x05
    Chap9_GetDescriptor,	//0x06
    Chap9_SetDescriptor,	//0x07
    Chap9_GetConfiguration,	//0x08
    Chap9_SetConfiguration,	//0x09
    Chap9_GetInterface,		//0x0a
    Chap9_SetInterface,		//0x0b
};
#endif

void usb_setup_handle( void)
{
    uint8    temp ;
    u8 i ;
    if(usb_running_ctrl_state == USB_IDLE)
    {
        //bmrequest type one byte

        bmRequestType = USB->rSETUP[0];
        //brequest one byte

        bRequest = USB->rSETUP[1];
        //wvalue two bytes

        temp = USB->rSETUP[2];

        wValue = USB->rSETUP[3];
        wValue = (wValue << 8) + temp;
        //windex two bytes

        temp = USB->rSETUP[4];


        wIndex = USB->rSETUP[5];

        wIndex = (wIndex << 8) + temp;
        //wlength two bytes

        temp = USB->rSETUP[6];

        wLength = USB->rSETUP[7];
        wLength = (wLength << 8) + temp;
        //direction
        req_dir = bmRequestType & USB_REQ_DIR_MASK ;
        //recipient
        req_recip = bmRequestType & USB_REQ_RECIP_MASK ;
        //type
        req_type = bmRequestType & USB_REQ_TYPE_MASK ;
        //#ifdef printf

        printf("接收set up数据：");

        for(i = 0; i < 8 ; i ++)
        {

            printf("%x ", USB->rSETUP[i]);
        }
        printf("\r\n");
        //#endif
#if 0
        printf("bmRequestType = 0x%x\r\n", bmRequestType);
        printf("bRequest = 0x%x\r\n", bRequest);
        printf("wValue = 0x%x\r\n", wValue);
        printf("wIndex = 0x%x\r\n", wIndex);
        printf("wLength = 0x%x\r\n", wLength);
#endif

        usb_running_ctrl_state = USB_CTRL_SETUP ;
    }
    ctrl_deal_handle();
}

void ctrl_deal_handle(void)
{
    if( req_type == USB_REQ_TYPE_STANDARD )     				// standard request type
    {
#ifdef printf
        printf("USB标准请求");
#endif
        if( bRequest < MAX_STD_REQUEST )    					// handle 0--12
        {
            (*StandardDeviceRequest[bRequest])( ) ;
        }
    }
    else if( req_type == USB_REQ_TYPE_CLASS )  					// class request type
    {
        switch(bRequest)
        {
            case GET_MAX_LUN:
                {
                    Class_Send_Max_Lun( ) ;
                    break;
                }
            default:
                {
                    if(req_dir == USB_REQ_DIR_IN)
                    {
                        EP0_Send_Empty_Packet();
                    }
                    break;
                }
        }
    }
    else
    {
        if(req_dir == USB_REQ_DIR_IN)
        {
            EP0_Send_Empty_Packet();
        }
    }
}

void ctrl_in_token_handle( void)
{

    if(req_dir == USB_REQ_DIR_IN)									// data stage when control read
    {
        ctrl_deal_handle();
    }
    else 														//status stage when control write (like setAddress) or no-data control
    {
        EP0_Send_Empty_Packet();
    }

}

void ctrl_out_handle( void)
{

    if(req_dir == USB_REQ_DIR_OUT)								//data stage when control write
    {

        ctrl_deal_handle();
    }
    else 														// status stage when control read, IP will send ack auto
    {

    }
}

void USB_Init( void)
{
    uint8 ix;
    uint32 tmpdata = 0x0;
    tmpdata = tmpdata;
    usb_setup_flag = 0;
    usb_out_flag = 0;
    usb_in_flag = 0;
    usb_reset_flag = 0;
    usb_suspend_flag = 0;
    usb_resume_flag = 0;

    usb_ep_flag = USB_EP0;
    write_finish = 1;
    dma_finished_flag[0] = 0;
    dma_finished_flag[1] = 0;
    usb_state = USB_STATE_POWERED;
    usb_running_state = USB_IDLE;
    usb_running_ctrl_state = USB_IDLE;
    for(ix = 0; ix < 5; ix++)
    {
        USB_EP_STALL[ix] = FALSE;
        USB_EP_IN_STALL[ix] = FALSE;
        USB_EP_OUT_STALL[ix] = FALSE;
        USB_FINISH_Flag[ix] = FALSE;
        USB_SEND_OPEN_STALL[ix] = FALSE;
    }

    USB->rTOP  = USB_TOP_RESET;//reset usb
    USB->rTOP  &= ~USB_TOP_RESET;
    USB->rTOP  &= ~USB_TOP_CONNECT ;//usb disconnect

    USB->rINT_STATE |= 0;//清USB状态
    USB->rEP_INT_STATE |= 0;
    USB->rEP0_INT_STATE |= 0;
    USB->rEP1_INT_STATE |= 0;
    USB->rEP2_INT_STATE |= 0;
    USB->rEP3_INT_STATE |= 0;
    USB->rEP4_INT_STATE |= 0;

    USB->rEP0_CTRL |= 0;//清数据
    USB->rEP1_CTRL |= 0;
    USB->rEP2_CTRL |= 0;
    USB->rEP3_CTRL |= 0;
    USB->rEP4_CTRL |= 0;

    USB->rINT_EN = USB_INT_EN_RSTIE | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE | USB_INT_EN_EPINTIE ; //使能USB相关中断
    USB->rEP_INT_EN = EP_INT_EN_EP0IE | EP_INT_EN_EP1IE | EP_INT_EN_EP2IE | EP_INT_EN_EP3IE | EP_INT_EN_EP4IE; //使能USB端点中断
    USB->rEP0_INT_EN = EPn_INT_EN_SETUPIE | EPn_INT_EN_ENDIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE; //使能USB端点0中断

    USB->rEP1_INT_EN = EPn_INT_EN_ENDIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_INSTALLIE | EPn_INT_EN_OUTSTALLIE; //EPIN = EP_1，使能输入端点中断;
    USB->rEP2_INT_EN = EPn_INT_EN_ENDIE | EPn_INT_EN_OUTACKIE | EPn_INT_EN_INSTALLIE | EPn_INT_EN_OUTSTALLIE; //EPOUT = 2，使能输出端点中断;

    USB->rEP_EN = EP_EN_EP0EN | EP_EN_EP1EN | EP_EN_EP2EN | EP_EN_EP3EN | EP_EN_EP4EN;
    USB->rADDR = 0;

    USB->rTOP = USB_TOP_CONNECT | ((~USB_TOP_SPEED) & 0x01); //enter work
    USB->rPOWER = USB_POWER_SUSPEN | USB_POWER_SUSP;
}
void usb_reset_handle(void)
{
    uint32 val;
    val = val;
    USB_Init();

    val = USB->rTOP;
    usb_state = USB_STATE_DEFAULT;
#ifdef printf
    printf("USB Reset 0x%lx\r\n", val);
#endif
}
void usb_suspend_handle(void)
{
    uint32 val;
    val = val;
    if(usb_state > USB_STATE_ADDRESS)//note it
    {
        USB->rPOWER = USB_POWER_SUSPEN; //enter suspend state, will shutdown phy clock
        val = USB->rTOP;
#ifdef printf
        printf("USB enter suspend 0x%lx\r\n", val);
#endif
        /***************JUST FOR RESUME TEST START*******************/
        usb_delay1ms(10000);//10s
#ifdef printf
        printf("USB timeout, wakeup now!\r\n");
#endif
        usb_wakeup_handle();
#ifdef printf
        printf("USB exit suspend\r\n");
#endif
        /***************JUST FOR RESUME TEST END*********************/
        usb_state = USB_STATE_SUSPENDED;
    }
}

void usb_resume_handle(void)
{
    uint32 val;
    val = val;

    val = USB->rTOP;
#ifdef printf
    printf("USB got resume 0x%lx\r\n", val);
#endif
}

void usb_wakeup_handle(void)
{
    uint32 val;
    val = val;

    val = USB->rTOP;
#ifdef printf
    printf("USB before wakeup 0x%lx\r\n", val);
#endif

    USB->rPOWER = USB_POWER_SUSPEN | USB_POWER_SUSP; //进入工作模式
    USB->rPOWER = USB_POWER_WKUP | USB_POWER_SUSPEN | USB_POWER_SUSP; //唤醒
    usb_delay1ms(3);											//3ms wait bus wakeup

    USB->rPOWER = USB_POWER_SUSPEN | USB_POWER_SUSP; //进入工作模式
    val = USB->rTOP;
    usb_state = USB_STATE_CONFIGURED;
#ifdef printf
    printf("USB after wakeup 0x%lx\r\n", val);
#endif
}

