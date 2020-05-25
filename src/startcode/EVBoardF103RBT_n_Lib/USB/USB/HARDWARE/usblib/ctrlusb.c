/**
******************************************************************************
* @file    ctrlusb.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the ctrlusb functions.
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
#include "usb.h"
#include "printf.h"

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
        bmRequestType = read_mreg32(USB_EP_SETUPn(0));
        //brequest one byte
        bRequest = read_mreg32(USB_EP_SETUPn(1));
        //wvalue two bytes
        temp = read_mreg32(USB_EP_SETUPn(2));
        wValue = read_mreg32(USB_EP_SETUPn(3));
        wValue = (wValue << 8) + temp;
        //windex two bytes
        temp = read_mreg32(USB_EP_SETUPn(4));
        wIndex = read_mreg32(USB_EP_SETUPn(5));
        wIndex = (wIndex << 8) + temp;
        //wlength two bytes
        temp = read_mreg32(USB_EP_SETUPn(6));
        wLength = read_mreg32(USB_EP_SETUPn(7));
        wLength = (wLength << 8) + temp;
        //direction
        req_dir = bmRequestType & USB_REQ_DIR_MASK ;
        //recipient
        req_recip = bmRequestType & USB_REQ_RECIP_MASK ;
        //type
        req_type = bmRequestType & USB_REQ_TYPE_MASK ;
        //#ifdef UART_PRINTF

        uart_printf("����set up���ݣ�");

        for(i = 0; i < 8 ; i ++)
        {
            uart_printf("%x ", read_mreg32(USB_EP_SETUPn(i)));
        }
        uart_printf("\r\n");
        //#endif
#if 0
        uart_printf("bmRequestType = 0x%x\r\n", bmRequestType);
        uart_printf("bRequest = 0x%x\r\n", bRequest);
        uart_printf("wValue = 0x%x\r\n", wValue);
        uart_printf("wIndex = 0x%x\r\n", wIndex);
        uart_printf("wLength = 0x%x\r\n", wLength);
#endif

        usb_running_ctrl_state = USB_CTRL_SETUP ;
    }
    ctrl_deal_handle();
}

void ctrl_deal_handle(void)
{

    if( req_type == USB_REQ_TYPE_STANDARD )     				// standard request type
    {
#ifdef UART_PRINTF
        uart_printf("USB��׼����");
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

void usb_init( void)
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
        usb_ep_stall[ix] = FALSE;
        usb_ep_in_stall[ix] = FALSE;
        usb_ep_out_stall[ix] = FALSE;
        usb_finish_flag[ix] = FALSE;
        usb_send_epn_stall[ix] = FALSE;
        write_mreg32(USB_EPn_CTRL(ix), 0x00);					//clear EP fifo data
    }
    write_mreg32(USB_TOP, USB_USBRST );							//reset usb
    write_mreg32(USB_TOP, USB_DISCONNECT ); 					//usb disconnect

    write_mreg32(USB_INT_STATE, read_mreg32(USB_INT_STATE));		//��USB�ж�״̬
    write_mreg32(EP_INT_STATE, read_mreg32(EP_INT_STATE));		//��USB�˵��ж�״̬
    write_mreg32(EP0_INT_STATE, read_mreg32(EP0_INT_STATE));		//��USB�˵�0�ж�״̬
    write_mreg32(EPn_INT_STATE(1), read_mreg32(EPn_INT_STATE(1))); //��USB�˵�1�ж�״̬
    write_mreg32(EPn_INT_STATE(2), read_mreg32(EPn_INT_STATE(2))); //��USB�˵�2�ж�״̬
    write_mreg32(EPn_INT_STATE(3), read_mreg32(EPn_INT_STATE(3))); //��USB�˵�3�ж�״̬
    write_mreg32(EPn_INT_STATE(4), read_mreg32(EPn_INT_STATE(4))); //��USB�˵�4�ж�״̬

    write_mreg32(USB_EP0_CTRL, 0x00000000);						 //��˵���ƼĴ���
    write_mreg32(USB_EPn_CTRL(1), 0x00000000);
    write_mreg32(USB_EPn_CTRL(2), 0x00000000);
    write_mreg32(USB_EPn_CTRL(3), 0x00000000);
    write_mreg32(USB_EPn_CTRL(4), 0x00000000);

    write_mreg32(USB_INT_EN, USB_INTHOSTRESET | USB_INTSUSPEND | USB_INTRESUME | USB_INTEP | USB_INT_DMA); // enable interrupt and clear interrupt
    write_mreg32(EP_INT_EN, 0x1F);								//ʹ�����ж˵��ж�
    write_mreg32(EP0_INT_EN, EP_SETUP | EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL); //ʹ�ܶ˵�0����ж�
#if 0
    tmpdata = read_mreg32(USB_INT_EN);
    tmpdata = read_mreg32(EP_INT_EN);
    tmpdata = read_mreg32(EP0_INT_EN);

    tmpdata = read_mreg32(USB_TOP);
    uart_printf("USB_TOP=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_INT_STATE);
    uart_printf("USB_INT_STATE=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_EP0_CTRL);
    uart_printf("USB_EP0_CTRL=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(EP0_INT_STATE);
    uart_printf("EP0_INT_STATE=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_INT_EN);
    uart_printf("USB_INT_EN=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(EP_INT_EN);
    uart_printf("EP_INT_EN=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(EP0_INT_EN);
    uart_printf("EP0_INT_EN=0x%lx\r\n", tmpdata);
#endif
#if (POUT!=EPIN)	 											//if epin != epout,    
    write_mreg32(EPn_INT_EN(EPIN), EP_FINISH | EP_IN_NACK | EP_STALL);
    write_mreg32(EPn_INT_EN(EPOUT), EP_FINISH | EP_OUT_ACK | EP_STALL);
#if 0
    tmpdata = read_mreg32(EPn_INT_EN(EPIN));
    uart_printf("EPn_INT_EN(EPIN)=0x%lx\r\n", tmpdata);
    tmpdata = read_mreg32(EPn_INT_EN(EPOUT));
    uart_printf("EPn_INT_EN(EPOUT)=0x%lx\r\n", tmpdata);
#endif
#else
    write_mreg32(EPn_INT_EN(EPIN), EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL);
    write_mreg32(EPn_INT_EN(EPOUT), EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL);

    tmpdata = read_mreg32(EPn_INT_EN(EPIN));
#ifdef UART_PRINTF
    uart_printf("EPn_INT_EN(EPIN)11=0x%lx\r\n", tmpdata);
#endif
    tmpdata = read_mreg32(EPn_INT_EN(EPOUT));
#ifdef UART_PRINTF
    uart_printf("EPn_INT_EN(EPOUT)11=0x%lx\r\n", tmpdata);
#endif
#endif
    write_mreg32(USB_EP_EN, 0x1F);

    write_mreg32(USB_ADDR, 0x0000);								// initial address is 0
    write_mreg32(USB_EP_DMA_DIR, EPn_DIR_IN(EPIN) | EPn_DIR_OUT(EPOUT)); //ep1 is D2H(IN), ep2 is H2D(out)
    tmpdata = read_mreg32(USB_EP_DMA_DIR);

    write_mreg32(USB_PACKAGE_SIZE0, EPn_MAX_PACKAGE_SIZE);		//for bullk
    write_mreg32(USB_PACKAGE_SIZE1, EPn_MAX_PACKAGE_SIZE);		//���ð��ߴ�

    write_mreg32(USB_EP_DMA_SIZE, 0x00);							//EP1 EP2 DMA sizeѡ��PACKAGE_SIZE0
    tmpdata = read_mreg32(USB_EP_DMA_SIZE);

    write_mreg32(USB_AHB_DMA, USB_DMA_BURST_1BYTE(0) | USB_DMA_BURST_1BYTE(1) | USB_DMA_TIMEOUT);

#if 0
    tmpdata = read_mreg32(USB_EP_EN);
    uart_printf("USB_EP_EN=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_ADDR);
    uart_printf("USB_ADDR=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_EP_DMA_DIR);
    uart_printf("USB_EP_DMA_DIR=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_PACKAGE_SIZE0);
    uart_printf("USB_PACKAGE_SIZE0=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_PACKAGE_SIZE1);
    uart_printf("USB_PACKAGE_SIZE1=0x%lx\r\n", tmpdata);

    tmpdata = read_mreg32(USB_EP_DMA_SIZE);
    uart_printf("USB_EP_DMA_SIZE=0x%lx\r\n", tmpdata);

#endif
#ifdef USB_DMA_MODE
    write_mreg32(USB_EP_DMA, 0x00);								//disable dma mode
    write_mreg32(DMAC_Ch_Cfg(0), 0x00000000);
    write_mreg32(DMAC_Ch_Cfg(1), 0x00000000);
    usb_dma_init();
#endif

    write_mreg32(USB_TOP, USB_CONNECT | USB_FS);

    write_mreg32(USB_AHB_POWER, 0x03); 							//enter work
    tmpdata = read_mreg32(USB_AHB_POWER);
#if 0
    tmpdata = read_mreg32(USB_TOP);
    uart_printf("USB_TOP=0x%lx\r\n", tmpdata);
    tmpdata = read_mreg32(USB_AHB_POWER);
    uart_printf("USB_AHB_POWER=0x%lx\r\n", tmpdata);
#endif
}
void usb_reset_handle(void)
{
    uint32 val;
    val = val;
    usb_init();
    val = read_mreg32(USB_TOP);
    usb_state = USB_STATE_DEFAULT;
#ifdef UART_PRINTF
    uart_printf("USB Reset 0x%lx\r\n", val);
#endif
}
void usb_suspend_handle(void)
{
    uint32 val;
    val = val;
    if(usb_state > USB_STATE_ADDRESS)//note it
    {
        write_mreg32(USB_AHB_POWER, 0x01); //enter suspend state, will shutdown phy clock	 //wsf modify 20131030
        val = read_mreg32(USB_TOP);
#ifdef UART_PRINTF
        uart_printf("USB enter suspend 0x%lx\r\n", val);
#endif
        /***************JUST FOR RESUME TEST START*******************/
        usb_delay1ms(10000);//10s
#ifdef UART_PRINTF
        uart_printf("USB timeout, wakeup now!\r\n");
#endif
        usb_wakeup_handle();
#ifdef UART_PRINTF
        uart_printf("USB exit suspend\r\n");
#endif
        /***************JUST FOR RESUME TEST END*********************/
        usb_state = USB_STATE_SUSPENDED;
    }
}

void usb_resume_handle(void)
{
    uint32 val;
    val = val;
    val = read_mreg32(USB_TOP);
#ifdef UART_PRINTF
    uart_printf("USB got resume 0x%lx\r\n", val);
#endif
}

void usb_wakeup_handle(void)
{
    uint32 val;
    val = val;
    val = read_mreg32(USB_TOP);
#ifdef UART_PRINTF
    uart_printf("USB before wakeup 0x%lx\r\n", val);
#endif
    write_mreg32(USB_AHB_POWER, 0x03); 							//0011 enter work state
    write_mreg32(USB_AHB_POWER, 0x0B); 							//1011 wakeup from suspend
    usb_delay1ms(3);											//3ms wait bus wakeup
    write_mreg32(USB_AHB_POWER, 0x03); 							//0011 wakeup from suspend stete
    val = read_mreg32(USB_TOP);
    usb_state = USB_STATE_CONFIGURED;
#ifdef UART_PRINTF
    uart_printf("USB after wakeup 0x%lx\r\n", val);
#endif
}

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/