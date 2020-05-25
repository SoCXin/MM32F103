/**
******************************************************************************
* @file    usbreg.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the usbreg firmware functions.
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
#include "sys.h"

volatile uint8 usb_ep_stall[5];
volatile uint8 usb_ep_in_stall[5];
volatile uint8 usb_ep_out_stall[5];
volatile uint8 usb_setup_flag;
volatile uint8 usb_out_flag;
volatile uint8 usb_in_flag;
volatile uint8 usb_finish_flag[5];
volatile uint8 usb_reset_flag;
volatile uint8 usb_suspend_flag;
volatile uint8 usb_resume_flag;
volatile uint8 usb_state_suspend = 0;
volatile uint8 usb_state_resume = 0;
volatile USB_STATE_t usb_state;
volatile uint8 usb_ep_flag;
//
volatile uint8 usb_send_epn_stall[5];
extern uint32 debug_flag;

void usb_reset()
{
    /*
    uint32 rstval;
    rstval = read_mreg32(SYS_SOFT_RST);
    rstval &= 0xFDF;   //bit5 1111 1101 1111
    write_mreg32(SYS_SOFT_RST,rstval);
    DelayMS(200);
    rstval = read_mreg32(SYS_SOFT_RST);
    rstval |= 0x020;   //bit5
    write_mreg32(SYS_SOFT_RST,rstval);
    DelayMS(200);
    write_mreg32(0xFFFF4014,0x01); //usb mode
    */
}


uint32 temp;
uint32 temp_ep;
uint32 temp_epn;


void USB_HP_CAN1_TX_IRQHandler(void)
#ifdef USB_INTR_MODE
//__irq
#endif
{
    temp = read_mreg32(USB_INT_STATE);
    write_mreg32(USB_INT_STATE, temp);							//��USB�жϣ�д1����
    if(temp & USB_INT_DMA)										//DMA�ж�
    {
        if(temp & USB_INT_DMA_CH0)
        {
            dma_finished_flag[0] = 1; 							//dma transfer done
        }
        if(temp & USB_INT_DMA_CH1)
        {
            dma_finished_flag[1] = 1; 							//dma transfer done
        }
    }
    else if(temp & USB_INTEP)										//�˵��ж�
    {
        temp_ep = read_mreg32(EP_INT_STATE);
        write_mreg32(EP_INT_STATE, temp_ep);					//��˵��жϣ�д1����
        if(temp_ep & EP0_INT)										//�˵�0�ж�
        {
            //GPIO_SetBits(GPIOA, GPIO_Pin_5);					//���ڴ���
            usb_ep_flag = USB_EP0;
            temp_epn = read_mreg32(EP0_INT_STATE);
            write_mreg32(EP0_INT_STATE, temp_epn);				//��˵�0�жϣ�д1����
            if(temp_epn & EP_SETUP)								//SETUP�ж�
            {
                usb_setup_flag = TRUE;
                if(usb_send_epn_stall[0] == TRUE)					//�˵�ֹͣ
                {
                }
                usb_setup_handle();								//setup������
            }
            if(temp_epn & EP_IN_NACK)								//���յ�IN����ʼ׼������
            {
                usb_in_flag = TRUE;
                usb_in_handle();								//IN������
                write_mreg32(EPn_INT_STATE(EPIN), EP_IN_NACK);	//��NACK�ж�
                write_mreg32(EPn_INT_EN(EPIN), (EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL)); //ʹ������˵��ж�
            }
            if(temp_epn & EP_OUT_ACK)
            {
                usb_out_flag = TRUE;
                usb_out_handle();								//OUT������
            }
            if(temp_epn & EP_STALL)
            {
                usb_ep_stall[0] = TRUE;
            }
            if(temp_epn & EP_IN_STALL)
            {
                usb_ep_in_stall[0] = TRUE;
            }
            if(temp_epn & EP_OUT_STALL)
            {
                usb_ep_out_stall[0] = TRUE;
            }
        }
#if ((EPOUT!=EPIN))	 											//if epin != epout,
        if(temp_ep & EPn_INT(EPOUT))								//����˵���ж�
        {
            usb_ep_flag = USB_EPn(EPOUT);
            temp_epn = read_mreg32(EPn_INT_STATE(EPOUT));
            write_mreg32(EPn_INT_STATE(EPOUT), temp_epn);		//��ָ���˵��ж�
            if(temp_epn & EP_OUT_ACK)								//�˵�����ж�
            {
                usb_out_flag = TRUE;
                usb_out_handle();
            }
            if(temp_epn & EP_FINISH)								//��������ж�
            {
                usb_finish_flag[EPOUT] = TRUE;
            }
            if(temp_epn & EP_IN_NACK)
            {
                usb_in_flag = TRUE;
                usb_in_handle();
                write_mreg32(EPn_INT_STATE(EPIN), EP_IN_NACK);
                write_mreg32(EPn_INT_EN(EPIN), (EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL));
            }
            if(temp_epn & EP_STALL)
            {
                usb_ep_stall[EPOUT] = TRUE;
            }
            if(temp_epn & EP_IN_STALL)
            {
                usb_ep_in_stall[EPOUT] = TRUE;
            }
            if(temp_epn & EP_OUT_STALL)
            {
                usb_ep_out_stall[EPOUT] = TRUE;
            }
        }
#endif
        if(temp_ep & EPn_INT(EPIN))
        {
            usb_ep_flag = USB_EPn(EPIN);
            temp_epn = read_mreg32(EPn_INT_STATE(EPIN));
            write_mreg32(EPn_INT_STATE(EPIN), temp_epn);			//clear interrupt
            if(temp_epn & EP_IN_NACK)
            {
                usb_in_flag = TRUE;
                usb_in_handle();
                write_mreg32(EPn_INT_EN(EPIN), (EP_FINISH | EP_IN_NACK | EP_OUT_ACK | EP_STALL));
            }
            if(temp_epn & EP_OUT_ACK)
            {
                usb_out_flag = TRUE;
                usb_out_handle();
            }
            if(temp_epn & EP_FINISH)
                usb_finish_flag[EPIN] = TRUE;
            if(temp_epn & EP_STALL)
            {
                usb_ep_stall[EPIN] = TRUE;
            }
            if(temp_epn & EP_IN_STALL)
            {
                usb_ep_in_stall[EPIN] = TRUE;
            }
            if(temp_epn & EP_OUT_STALL)
            {
                usb_ep_out_stall[EPIN] = TRUE;
            }
        }
    }
    else if(temp & USB_INTHOSTRESET)
    {
        //got host reset interrupt
        usb_reset_flag = TRUE;
        usb_reset_handle();
    }
    else if(temp & USB_INTSUSPEND)
    {
        usb_suspend_flag = TRUE;
        usb_suspend_handle();
    }
    else if(temp & USB_INTRESUME)
    {
        usb_resume_flag = TRUE;
        usb_resume_handle();
    }
}

uint32 read_mreg32( uint32 mreg)
{
    return(*(volatile uint32 *)mreg);
}

void write_mreg32( uint32 mreg, uint32 val)
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

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/