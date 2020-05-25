
#include "usbprop.h"
#include "usb.h"

unsigned char *pucClassDrcData;


//���⴮�ڲ���
LINE_CODING linecoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
};


/********************************************************************************************************
**������Ϣ ��USBCDC_senddata(unsigned char *data,unsigned short length)
**�������� ������ת��
**������� ��unsigned char *data(ת��������ָ��),unsigned short length(ת�����ݳ���)
**������� ��
**��    ע ��
********************************************************************************************************/
void USBCDC_senddata(unsigned char *data, unsigned short length)
{
    if(linecoding.datatype == 7)
    {
        while(length)
        {
            *data &= 0x7f;
            length--;
            data++;
        }
    }
    else if(linecoding.datatype == 8)							//������
    {
        while(length)
        {

        }
    }
}

/********************************************************************************************************
**������Ϣ ��Class_Get_Line_Coding(void)
**�������� ��CDC Abstract Control Model(���⴮��) ������GET_LINE_CODING
**������� ��
**������� ��
**��    ע ��
********************************************************************************************************/
void Class_Get_Line_Coding(void)      							// bRequest = 0x21
{
    int count;
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            if(req_dir == USB_REQ_DIR_IN)
            {
                pucClassDrcData = (u8*)&linecoding;
                usb_running_ctrl_state = USB_CTRL_IN;
            }
            else
            {
                usb_running_ctrl_state = USB_IDLE;
            }
            break ;

        case    USB_CTRL_IN :

            while(USB->rEP0_CTRL & 0x80);
            for( count = 0; count < 7; count++ )
            {
                USB->rEP0_FIFO = *pucClassDrcData;
                pucClassDrcData ++;
            }
            USB->rEP0_CTRL = 0x87;

            usb_running_ctrl_state = USB_IDLE;
            break ;

        default :
            //		usb_running_ctrl_state = USB_IDLE ;
            break ;
    }
}
unsigned char baud_read [64];
/********************************************************************************************************
**������Ϣ ��Class_Get_Line_Coding(void)
**�������� ��CDC Abstract Control Model(���⴮��) ������SET_LINE_CODING
**������� ��
**������� ��
**��    ע ��
********************************************************************************************************/
void Class_Set_Line_Coding(void)  // bRequest = 0x20
{
    int count, i;

    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            if(req_dir == USB_REQ_DIR_OUT)
            {
                usb_running_ctrl_state = USB_CTRL_OUT;
            }
            else
            {
                usb_running_ctrl_state = USB_IDLE;
            }
            break ;

        case    USB_CTRL_OUT :
            count = USB->rEP0_AVIL;
            for(i = 0; i < count ; i++)
            {
                baud_read[i] = USB->rEP0_FIFO;
            }
            linecoding.bitrate = ((u32)baud_read[0] << 0) | ((u32)baud_read[1] << 8) | ((u32)baud_read[2] << 16) | ((u32)baud_read[3] << 24);
            uart_init(48, linecoding.bitrate);

            usb_running_ctrl_state = USB_CTRL_STATUS;
            break;

        default :
            //			usb_running_ctrl_state = USB_IDLE ;
            break ;
    }
}
/********************************************************************************************************
**������Ϣ ��Class_Get_Line_Coding(void)
**�������� ��CDC Abstract Control Model(���⴮��) ������SET_LINE_CODING
**������� ��
**������� ��
**��    ע ��
********************************************************************************************************/

void Class_Set_Control_Line_State(void)      				// bRequest = 0x22
{
    int count, i;
    unsigned char state_temp;
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :							//���ƴ���״̬�׶�
            if(req_dir == USB_REQ_DIR_OUT)
            {
                usb_running_ctrl_state = USB_CTRL_OUT;
            }
            else
            {
                usb_running_ctrl_state = USB_IDLE;
            }
            break ;
        case    USB_CTRL_OUT :
            count = USB->rEP0_AVIL;
            for(i = 0; i < count ; i++)
            {
                state_temp = USB->rEP0_FIFO;				//�˴�Ϊ������(���ƴ������ݽ׶�)
            }
            usb_running_ctrl_state = USB_CTRL_STATUS;
            break;

        default :
            {
                //	        usb_running_ctrl_state = USB_IDLE ;
                break ;
            }
    }
}

unsigned char ep1_Tx_dataBuf[2048];			//USB ���ͻ�����,���Զ����С
unsigned int ui_curTxLen = 0;				//ʵʱ���ͳ���
unsigned int ep1_Tx_dataLen = 0;			//���������ܳ���(С���Զ��建������С)
unsigned char ep1_tx_flag = 0;				//���ͱ�־

/********************************************************************************************************
**������Ϣ ��EP1_USB_IN_Data(void)
**�������� ��USB�ӷ��ͻ�����������ͨ��IN�˵㷢��
**������� ��
**������� ��
**��    ע ��USB���ݷ���ʱ�����ݳ������˵�sizeʱ����Ҫ�������
********************************************************************************************************/
void EP1_USB_IN_Data(void)      			// bRequest = 0x22
{
    int count, i;

    if(ep1_tx_flag != 0)						//tx_buf�ǿ�
    {
        while(USB->rEP1_CTRL & 0x80);
        if(ep1_Tx_dataLen > 64)				//USB���ݲ�ַ���
        {
            count = 64;
            ep1_Tx_dataLen -= 64;
        }
        else
        {
            count = ep1_Tx_dataLen;
            ep1_Tx_dataLen = 0;
        }
        for(i = 0; i < count; i++)
        {
            USB->rEP1_FIFO = *(ep1_Tx_dataBuf + ui_curTxLen + i);
        }
        USB->rEP1_CTRL = 0x80 | count;
        if(ep1_Tx_dataLen == 0)
        {
            ep1_tx_flag = 0;
            ui_curTxLen = 0;
            usb_running_ctrl_state = USB_IDLE;
        }
        else
        {
            ui_curTxLen += count;
        }
    }

}
/********************************************************************************************************
**������Ϣ ��UsbVcomSend(unsigned char*Info,unsigned int *infoLenth)
**�������� ����Ҫ���͵�����д��USB���ͻ�����
**������� ��unsigned char*Info(��������ָ��),unsigned int *infoLenth(�������ݳ���ָ��)
**������� ��return status(0��ʾ�������,1��ʾ���ڷ���);
**��    ע ��
********************************************************************************************************/
char UsbVcomSend(unsigned char*Info, unsigned int *infoLenth)
{
    unsigned int i, status = 0;
    if((ep1_tx_flag == 0) && (*infoLenth != 0))
    {
        ep1_Tx_dataLen = *infoLenth;
        *infoLenth = 0;
        for(i = 0; i < ep1_Tx_dataLen; i++)
        {
            ep1_Tx_dataBuf[i] = *(Info + i);
        }
        ep1_tx_flag = 1;
    }
    else
    {
        status = 1;
    }
    return status;
}
unsigned char ep3_rx_flag = 0;				//�˵�3���ձ�־
/********************************************************************************************************
**������Ϣ ��EP3_USB_OUT_Data(void)
**�������� ��USB_OUT�˵���յ���Ч����,���ձ�־λ��1
**������� ��
**������� ��
**��    ע ��
********************************************************************************************************/
void EP3_USB_OUT_Data(void)      			// bRequest = 0x22
{
    if (ep3_rx_flag == 0)					//rx_buf��
    {
        ep3_rx_flag = 1;
    }
    usb_running_ctrl_state = USB_IDLE;
}
/********************************************************************************************************
**������Ϣ ��UsbVcomRec(unsigned char*rxInfo)
**�������� ����USB device�� OUT�˵��������
**������� ��unsigned char*rxInfo(������Ϣbuf)
**������� ��
**��    ע ��
********************************************************************************************************/
unsigned int UsbVcomRec(unsigned char*rxInfo)
{
    unsigned int i, temp_len;
    if(ep3_rx_flag == 1)					//����buf�ǿ�
    {
        temp_len = USB->rEP3_AVIL & 0x7f;
        for(i = 0; i < temp_len; i++)
        {
            *(rxInfo + i) = USB->rEP3_FIFO;
        }
        ep3_rx_flag = 0;
    }
    else
    {
        return 0;
    }
    return temp_len;
}

