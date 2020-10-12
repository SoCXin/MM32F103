//********************************************************************
//(Copyright 2017) MindMotion All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//MindMotion  Co.,Ltd
//
//File name 	: usbreg.c
//Author		  :
//Verision		:
//Created		  :
//Modified		: AE 2017.08.21
//Description	:
//
//********************************************************************
#include "usb.h"
#include "HAL_device.h"

volatile uint8 USB_EP_STALL[5];
volatile uint8 USB_EP_IN_STALL[5];
volatile uint8 USB_EP_OUT_STALL[5];
volatile uint8 usb_setup_flag;
volatile uint8 usb_out_flag;
volatile uint8 usb_in_flag;
volatile uint8 USB_FINISH_Flag[5];
volatile uint8 usb_reset_flag;
volatile uint8 usb_suspend_flag;
volatile uint8 usb_resume_flag;
volatile uint8 usb_state_suspend = 0;
volatile uint8 usb_state_resume = 0;
volatile USB_STATE_t usb_state;
volatile uint8 usb_ep_flag;
//
volatile uint8 USB_SEND_OPEN_STALL[5];
extern uint32 debug_flag;

void usb_reset()
{

}


uint32 temp;
uint32 temp_ep;
uint32 temp_epn;

void USB_HP_CAN1_TX_IRQHandler(void)
#ifdef USB_INTR_MODE
//__irq
#endif
{
    temp = USB->rINT_STATE ;
    USB->rINT_STATE = temp;
    if(temp & USB_INT_STATE_EPINTF)										//端点中断
    {

        temp_ep = USB->rEP_INT_STATE;

        USB->rEP_INT_STATE = temp_ep;
        if(temp_ep & EP_INT_STATE_EP0F )									//端点0中断
        {
            //GPIO_SetBits(GPIOA, GPIO_Pin_5);					//用于触发
            usb_ep_flag = USB_EP0;

            temp_epn = USB->rEP0_INT_STATE;

            USB->rEP0_INT_STATE = temp_epn;

            if(temp_epn & EPn_INT_STATE_SETUP)							//SETUP中断
            {
                usb_setup_flag = TRUE;
                if(USB_SEND_OPEN_STALL[0] == TRUE)						//端点停止
                {
                }
                usb_setup_handle();										//setup包处理
            }
            if(temp_epn & EPn_INT_STATE_INNACK)							//IN包非应答中断，准备写入数据
            {
                usb_in_flag = TRUE;
                usb_in_handle();										//IN包处理

                USB->rEP0_INT_STATE = EPn_INT_STATE_INNACK;				//清端点中断
                USB->rEP0_INT_EN = EPn_INT_STATE_END | EPn_INT_STATE_INNACK | EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTSTALL | EPn_INT_STATE_INSTALL; // EP_IN_NACK;
            }
            if(temp_epn & EPn_INT_STATE_OUTACK)							//OUT包应答中断，收到数据
            {
                usb_out_flag = TRUE;
                usb_out_handle();										//OUT包处理
            }
            if(temp_epn & (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE))
            {
                USB_EP_STALL[0] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_INSTALL)
            {
                USB_EP_IN_STALL[0] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_OUTSTALL)
            {
                USB_EP_OUT_STALL[0] = TRUE;
            }
        }
#if ((EPOUT!=EPIN))	 													//if epin != epout,
        if(temp_ep & EPn_INT(EPOUT))										//输出端点号中断
        {
            usb_ep_flag = USB_EPn(EPOUT);

            temp_epn = USB->rEP2_INT_STATE;
            USB->rEP2_INT_STATE = temp_epn;
            if(temp_epn & EPn_INT_STATE_OUTACK)							//端点输出应答中断
            {
                usb_out_flag = TRUE;
                usb_out_handle();
            }
            if(temp_epn & EPn_INT_STATE_END)								//传输完成中断
            {
                USB_FINISH_Flag[EPOUT] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_INNACK)
            {
                usb_in_flag = TRUE;
                usb_in_handle();
                USB->rEP1_INT_STATE = EPn_INT_STATE_INNACK;
                USB->rEP1_INT_EN = EPn_INT_STATE_END | EPn_INT_STATE_INNACK | EPn_INT_STATE_OUTACK | (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE);
            }
            if(temp_epn & (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE))
            {
                USB_EP_STALL[EPOUT] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_INSTALL)
            {
                USB_EP_IN_STALL[EPOUT] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_OUTSTALL)
            {
                USB_EP_OUT_STALL[EPOUT] = TRUE;
            }
        }
#endif
        if(temp_ep & EPn_INT(EPIN))
        {
            usb_ep_flag = USB_EPn(EPIN);

            temp_epn = USB->rEP1_INT_STATE;
            USB->rEP1_INT_STATE = temp_epn;
            if(temp_epn & EPn_INT_STATE_INNACK)
            {
                usb_in_flag = TRUE;
                usb_in_handle();
                USB->rEP1_INT_EN = EPn_INT_STATE_END | EPn_INT_STATE_INNACK | EPn_INT_STATE_OUTACK | (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE);
            }
            if(temp_epn & EPn_INT_STATE_OUTACK)
            {
                usb_out_flag = TRUE;
                usb_out_handle();
            }
            if(temp_epn & EPn_INT_STATE_END)
                USB_FINISH_Flag[EPIN] = TRUE;
            if(temp_epn & (EPn_INT_EN_OUTSTALLIE | EPn_INT_EN_INSTALLIE))
            {
                USB_EP_STALL[EPIN] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_INSTALL)
            {
                USB_EP_IN_STALL[EPIN] = TRUE;
            }
            if(temp_epn & EPn_INT_STATE_OUTSTALL)
            {
                USB_EP_OUT_STALL[EPIN] = TRUE;
            }
        }
    }
    else if(temp & USB_INT_STATE_RSTF)
    {
        //got host reset interrupt
        usb_reset_flag = TRUE;
        usb_reset_handle();
    }
    else if(temp & USB_INT_STATE_SUSPENDF )
    {
        usb_suspend_flag = TRUE;
        usb_suspend_handle();
    }
    else if(temp & USB_INT_STATE_RESUMF  )
    {
        usb_resume_flag = TRUE;
        usb_resume_handle();
    }
}

uint32 Read_Mreg32( uint32 mreg)
{
    return(*(volatile uint32 *)mreg);
}

void Write_Mreg32( uint32 mreg, uint32 val)
{
    *(volatile uint32 *)mreg = (uint32)val;
}


uint8 read_mreg8( uint32 mreg)
{
    return(*(volatile uint8 *)mreg);
}

void write_mreg8( uint32 mreg, uint8 val)
{
    *(volatile uint8 *)mreg = (uint8)val;
}

void usb_delay1ms(uint32 dly)
{
    uint32 cnt, i, j, k;

    for(cnt = 0; cnt < dly; cnt++)
    {
        for(i = 0; i < 24; i++)										//wait bus wakeup
        {
            for(j = 0; j < 2; j++)
            {
                for(k = 0; k < 100; k++);
            }
        }
    }

}

