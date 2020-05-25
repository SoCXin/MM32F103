//********************************************************************
//(Copyright 2015) Betterlife All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Betterlife  Co.,Ltd
//
//File name 	: Bulkusb.c
//Author		  :
//Verision		:
//Created		  :
//Modified		: sfw 2015.01.30
//Description	:
//
//********************************************************************
#include "usb.h"

volatile uint8   dCBWSig[4] ;
volatile uint8   dCBWTag[4] ;
volatile uint8  bmCBWDataLength[4] ;
volatile uint8   bmCBWFlag ;
volatile uint8  bCBWLUN ;
volatile uint8   bCBWCBLength ;
volatile uint8   CBWCB[16];
volatile char usb_cbw_buf[CBW_BUF_SIZE];

void Deal_Trans_Items( void)
{
    switch( CBWCB[0] )
    {
        case    INQUIRY_OPCODE :    							// 0x12
            {
                ScsiCmdInquiry( ) ;
                break ;
            }
        case    REQUEST_SENSE_OPCODE:    					// 0x03
            {
                ScsiCmdRequestSense( ) ;
                break ;
            }
        case    _23_OPCODE :        							// 0x23
            {
                _op_code_23_handle( ) ;
                break ;
            }
        case    READ_CAPACITY10_OPCODE :    					// 0x25
            {
                _read_capacity_handle( ) ;
                break ;
            }
        case    READ10_OPCODE :             					// 0x28
            {
                _read10_handle(TRUE) ;
                break ;
            }
        case    MODE_SENSE6_OPCODE :        					// 0x1A
            {
                _sense6_handle( ) ;
                break ;
            }
        case    WRITE10_OPCODE :            					// 0x2A
            {
                _write10_handle(TRUE);
                break ;
            }
        case    READTEST_OPCODE :             					// 0xA8
            {
                _read10_handle(FALSE) ;
                break ;
            }
        case    WRITETEST_OPCODE :            				// 0xAA
            {
                _write10_handle(FALSE);
                break ;
            }
        case    STALLOUT_OPCODE :            				// 0xB8
            {
                USB->rEP_HALT = 1 << EPOUT;
                USB_SEND_OPEN_STALL[EPOUT] = TRUE;
#ifdef UART_PRINTF
                uart_printf( "EP enable out stall Cmd, 0x%x\r\n", CBWCB[0] ) ;
#endif
                usb_running_state = USB_IDLE ;
                p_recv_buf = (uint32)usb_cbw_buf;
                recv_residue = sizeof(usb_cbw_buf);
                break ;
            }

        case    STALLIN_OPCODE :            				// 0xBA
            {
                USB_SEND_OPEN_STALL[EPIN] = TRUE;
#ifdef UART_PRINTF
                uart_printf( "EP enable In stall Cmd, 0x%x\r\n", CBWCB[0] ) ;
#endif
                usb_running_state = USB_IDLE ;
                p_recv_buf = (uint32)usb_cbw_buf;
                recv_residue = sizeof(usb_cbw_buf);

                break ;
            }

        case    TEST_UNIT_READY_OPCODE :    					// 0x00
        case    PREVENT_REMOVAL_OPCODE :    					// 0x1E
        case    VERIFY10_OPCODE :           					// 0x2F
            {
                _cmd_common_handle( ) ;
                break ;
            }
        default :
            {
#ifdef UART_PRINTF
                uart_printf( "Unknown Cmd, 0x%x\r\n", CBWCB[0] ) ;
#endif
#ifdef USB_EP_STALL_TEST

                USB->rEP_HALT = (1 << EPOUT);
                USB_SEND_OPEN_STALL[EPOUT] = TRUE;
#endif

                usb_running_state = USB_IDLE ;
                p_recv_buf = (uint32)usb_cbw_buf;
                recv_residue = sizeof(usb_cbw_buf);
                break;
            }
    }
}
// ==================================== Low Sub Feature ===================================
void usb_cbw_handle( void)
{

    uint32 cnt = MIN(recv_residue, EPn_MAX_PACKAGE_SIZE);
#ifdef USB_CBWCSW_DMA_MODE
    p_recv_buf = device_rx_fifo(p_recv_buf, cnt, TRUE);
#else
    p_recv_buf = device_rx_fifo(p_recv_buf, cnt, FALSE);
#endif
    recv_residue -= cnt;
    if(recv_residue == 0)
    {
        usb_deal_cbw();
    }
}

void usb_deal_cbw(void)
{
    uint8   cbw_ix;
    uint8   dCBWSigCont[4] = { 0x55, 0x53, 0x42, 0x43 } ;   	// USBC dCBWSignature
    uint8   error_flag = 0 ;
    //***********************************************//
    //Table 5.1 Command Block Wrapper
    //***********************************************//
    //read dCBWSignature byte[3]--byte[0]
    for( cbw_ix = 0; cbw_ix <= 3; cbw_ix++ )
    {
        dCBWSig[cbw_ix - 0] = usb_cbw_buf[cbw_ix];
    }
    //read dCBWTag byte[7]--byte[4]
    for( cbw_ix = 4; cbw_ix <= 7; cbw_ix++ )
    {
        dCBWTag[cbw_ix - 4] = usb_cbw_buf[cbw_ix];
    }
    //read dCBWDataTRansferLength byte[11]--byte[8]
    for( cbw_ix = 8; cbw_ix <= 11; cbw_ix++ )
    {
        bmCBWDataLength[cbw_ix - 8] = usb_cbw_buf[cbw_ix];
    }
    //read bmCBWFlags byte[12]
    bmCBWFlag = usb_cbw_buf[12];
    //read bCBWLUN byte[13]
    bCBWLUN = usb_cbw_buf[13];
    //read bCBWCBLength byte[14]
    bCBWCBLength = usb_cbw_buf[14];								//valid data is 1--16
    //read CBWCB byte[30]--byte[15]
    for( cbw_ix = 15; cbw_ix <= 30; cbw_ix++ )
    {
        CBWCB[cbw_ix - 15] = usb_cbw_buf[cbw_ix];
    }
    //judge correct or wrong
    for( cbw_ix = 0; cbw_ix < 4; cbw_ix++ )
    {
        if( dCBWSigCont[cbw_ix] != dCBWSig[cbw_ix] )
            error_flag = 1 ;
    }
    if( bCBWLUN != 0x00 )
    {
        error_flag = 1 ;
    }
    if( error_flag )
    {
#ifdef UART_PRINTF
        uart_printf("CBW error!\r\n");
#endif
        usb_running_state = USB_IDLE ;
        p_recv_buf = (uint32)usb_cbw_buf;
        recv_residue = sizeof(usb_cbw_buf);
        return ;
    }
    usb_running_state = USB_BULK_CBW ;
    Deal_Trans_Items( ) ;
}

void usb_bulk_in_token_handle( void)
{
    Deal_Trans_Items( ) ;
}
void usb_bulk_out_handle( void)
{
    Deal_Trans_Items( ) ;
}
void usb_bulk_trans_finish_handle( void)
{
    Deal_Trans_Items( ) ;
}

//extern unsigned char ep1_tx_flag;
// =============================== usb IN OUT main fature ==========================
void usb_in_handle( void)       // IN Request Received
{
    if( usb_ep_flag == USB_EP0 )
    {
        ctrl_in_token_handle() ;
    }
    else
    {

        EP1_USB_IN_Data();

    }

}


void usb_out_handle( void)      								// OUT Packet Received
{
    if( usb_ep_flag == USB_EP0 )
    {
        ctrl_out_handle();
    }
    else
    {
        EP3_USB_OUT_Data();
    }

}

uint32 device_rx_fifo(uint32 dstAddr, uint32 byteCnt, uint32 bDMAmode)
{
    uint32 count;
    for( count = 0; count < byteCnt; count++ )
    {
        uint8 val = USB->rEP2_FIFO;
        write_mreg8(dstAddr, val);
        dstAddr++;
    }

    return dstAddr;
}

uint32 device_tx_fifo(uint32 srcAddr, uint32 byteCnt, uint32 bDMAmode)
{
    uint32 count;


    while(USB->rEP1_CTRL & 0x80);
    for( count = 0; count < byteCnt; count++)
    {
        uint8 val = read_mreg8(srcAddr);
        USB->rEP2_FIFO = val;
        srcAddr++;
    }
    USB->rEP1_CTRL = 0x80 | byteCnt;
    return srcAddr;
}

#ifdef DEVICE_HID

void usb_hid_report_handle()
{
#ifdef USB_EP_STALL_TEST
    static uint32 hid_cnt = 1;
    if(((hid_cnt++) % 0x80 == 0x00))
    {

        USB->rEP_HALT = (1 << (EPIN);
#ifndef SCC5_VER_0302
                         USB->rEP1_CTRL = 0x80;
#endif
                         USB_SEND_OPEN_STALL[EPIN] = TRUE;
    }
                    else
#endif
    {
#ifdef USB_DMA_MODE

        uint32 ch;
        uint32 *hid_val = (uint32*)0x68000000; 					//because it's place at sram which dam can access
        hid_val[0] = 0x00010100;
        usb_dma_tx_start((uint32)hid_val, USB->rEP1_FIFO, sizeof(hid_val));

        ch = (EPIN == USB_EPn(1)) ? (0) : (1);
        dma_finished_flag[ch] = 0;
#ifdef USB_INTR_MODE
        while(dma_finished_flag[ch] == 0x00);						//wait dma transfer done
#else
        do
        {

            uint32 temp = USB->rINT_STATE;
            if(ch == 0)
            {

            }
            else
            {

            }
        }
        while(1);												//wait dma transfer done
#endif
        dma_finished_flag[ch] = 0;
        usb_dma_close(&usb_dma_tx_cmd);
#else

        while(USB->rEP1_CTRL & 0x80);

#endif
    }
}

#endif

