//********************************************************************
//(Copyright 2017) MindMotion All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Verisilicon  Co.,Ltd
//
//File name 	: ctrlitem.c
//Author		:
//Verision		:
//Created	    :
//Modified		: AE 2017.08.21
//Description	:
//
//********************************************************************
#include "usb.h"

#ifdef DEVICE_HID

#ifdef  KEYBOARD_HID
#define MY_VID  		0x0808
#define MY_PID  		0x0606
#else
#define MY_VID  		0x0461
#define MY_PID  		0x4D15
#endif

#else 															//CBW/CSW
#ifdef USB_DEVICE_MASS_STORAGE
#define MY_VID  		0x05E2
#define MY_PID  		0x0101
#define IFCLASS			IFCLASS_MASS
#define IFSUBCLASS		IFSUBCLASS_MASS
#define IFPROTOCOL      IFPROTOCOL_MASS
#else
#define MY_VID  		0x05E4
#define MY_PID  		0x0101
#define IFCLASS			IFCLASS_TEST
#define IFSUBCLASS		IFSUBCLASS_TEST
#define IFPROTOCOL      IFPROTOCOL_TEST
#endif 															// MASS STORAGE
#endif 															//HID

volatile uint8    ctrl_buf[256] ;
volatile uint8    *p_ctrl_buf = 0;
volatile uint8    ctrl_count ;
#ifdef DEVICE_HID												//HID 
/*(Mouse) information from Appendix E: Example USB Descriptors for HID Class Devices, in Page76 @ HID1.1*/
static uint8    DeviceDesc[] =
{
    0x12/*bLength*/, 0x01/*bDescrType=DEVICE*/, 0x10/*bcdUSB*/, 0x01/*bcdUSB*/, 0x00/*bDevClass*/, 0x00/*bDevSubClass*/, 0x00/*bDevProtocol*/,
    EP0_MAX_PACKAGE_SIZE/*bMaxPacketSize*/, (MY_VID >> 0) & 0xFF/*idVendor*/, (MY_VID >> 8) & 0xFF/*idVendor*/, (MY_PID >> 8) & 0xFF/*idProduct*/, (MY_PID >> 8) & 0xFF/*idProduct*/, 0x00/*bcdDev*/, 0x02/*bcdDev*/,
    0x00/*iManugacturer*/, 0x02/*iProduct*/, 0x00/*iSerialNum*/, 0x01/*bNumConfig*/
};

#ifdef  KEYBOARD_HID

static uint8    ConfigDesc[] =
{
    0x09/*bLength*/, 0x02/*bDescrType=CONFIGURATION*/, 0x22/*wTotalLength*/, 0x00/*wTotalLength*/, 0x01/*bNumIF*/, 0x01/*bCfgValue*/, 0x00/*iConfig*/, 0xA0/*bmAttribute=bus Power, remote wakeup*/, 0x32/*MaxPower*/, /*device configuration*/
    0x09/*bLength*/, 0x04/*bDescrType=INTERFACES*/, 0x00/*bIFNum*/, 0x00/*bAlternateSetting*/, 0x01/*bNumEP*/, IFCLASS_HID/*bIFClass=HID*/, IFSUBCLASS_HID/*bIFSubClass=boot IF*/, 0x01/*bIFProtocol=keyboard*/, 0x00/*biIF*/, /*interface configuration*/
    0x09/*bLength*/, 0x21/*bDescrType=HID*/, 0x10/*bcdHID*/, 0x01/*bcdHID*/, 0x21/*bcountryCode*/, 0x01/*bNumDescriptors=number of HID class Desriptors*/, 0x22/*bDescriptorType=report*/, 0x41/*wDescriptorLength=sizeof(IntefaceDes)*/, 0x00/*wDescriptorLength*/, /*HID Descriptor*/
    0x07/*bLength*/, 0x05/*bDescrType=ENDPOINT*/, EPn_INPORT(EPIN)/*bEPAddr*/, 0x03/*bmAttributes=Interrupt*/, EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/, 0x00/*bInterval*/, 0x05/*bInterval*/
};/*EndPoint Configuration Descriptor*/

/*Report Descriptor (keyboard) */
static uint8    IntefaceDes[] = {  	0x05, 0x01, 0x09, 0x06, 0xa1, 0x01, 0x05, 0x07,
                                    0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01,
                                    0x95, 0x08, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01,
                                    0x75, 0x08, 0x81, 0x03, 0x95, 0x06, 0x75, 0x08,
                                    0x15, 0x00, 0x25, 0xff, 0x05, 0x07, 0x19, 0x00,
                                    0x29, 0x65, 0x81, 0x00, 0x25, 0x01, 0x95, 0x05,
                                    0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05,
                                    0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x03,
                                    0xc0
                                };
#else
static uint8    ConfigDesc[] =
{
    0x09/*bLength*/, 0x02/*bDescrType=CONFIGURATION*/, 0x22/*wTotalLength*/, 0x00/*wTotalLength*/, 0x01/*bNumIF*/, 0x01/*bCfgValue*/, 0x00/*iConfig*/, 0xA0/*bmAttribute=bus Power, remote wakeup*/, 0x32/*MaxPower*/, /*device configuration*/
    0x09/*bLength*/, 0x04/*bDescrType=INTERFACES*/, 0x00/*bIFNum*/, 0x00/*bAlternateSetting*/, 0x01/*bNumEP*/, IFCLASS_HID/*bIFClass=HID*/, IFSUBCLASS_HID/*bIFSubClass=boot IF*/, IFPROTOCOL_HID/*bIFProtocol=Mouse*/, 0x00/*biIF*/, /*interface configuration*/
    0x09/*bLength*/, 0x21/*bDescrType=HID*/, 0x11/*bcdHID*/, 0x01/*bcdHID*/, 0x00/*bcountryCode*/, 0x01/*bNumDescriptors=number of HID class Desriptors*/, 0x22/*bDescriptorType=report*/, 0x34/*wDescriptorLength=sizeof(IntefaceDes)*/, 0x00/*wDescriptorLength*/, /*HID Descriptor*/
    0x07/*bLength*/, 0x05/*bDescrType=ENDPOINT*/, EPn_INPORT(EPIN)/*bEPAddr*/, 0x03/*bmAttributes=Interrupt*/, EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/, 0x00/*bInterval*/, 0x0a/*bInterval*/
};/*EndPoint Configuration Descriptor*/

/*Report Descriptor (Mouse)*/
static uint8    IntefaceDes[] = {  0x05, 0x01, 0x09, 0x02, 0xa1, 0x01, 0x09, 0x01,
                                   0xa1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03,
                                   0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x03,
                                   0x81, 0x02, 0x75, 0x05, 0x95, 0x01, 0x81, 0x01,
                                   0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x38,
                                   0x15, 0x81, 0x25, 0x7f, 0x75, 0x08, 0x95, 0x03,
                                   0x81, 0x06, 0xc0, 0xc0
                                };
#endif

static uint8    Str0Desc[] = {0x04, 0x03, 0x09, 0x04 };
static uint8    Str1Desc[] = {0x04, 0x03, ' ', 0x00};
static uint8    Str2Desc[] = {0x24, 0x03, 0x55, 0x00, 0x53, 0x00, 0x42, 0x00,
                              0x20, 0x00, 0x4f, 0x00, 0x70, 0x00, 0x74, 0x00,
                              0x69, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6c, 0x00,
                              0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x75, 0x00,
                              0x73, 0x00, 0x65, 0x00
                             };

static uint8    Str3Desc[] = {0x16, 0x03, '2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
                              '0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00
                             };



#define InterfaceDesLen		(sizeof(IntefaceDes))
#else // Mass-storage
//=========================== Common Para  =============================
static uint8    DeviceDesc[] =
{
    0x12/*bLength*/, 0x01/*bDescrType=DEVICE*/, 0x10/*bcdUSB*/, 0x01/*bcdUSB*/, 0x00/*bDevClass*/, 0x00/*bDevSubClass*/, 0x00/*bDevProtocol*/,  // USB Version 1.10
    EP0_MAX_PACKAGE_SIZE/*bMaxPacketSize*/, (MY_VID >> 0) & 0xFF/*idVendor*/, (MY_VID >> 8) & 0xFF/*idVendor*/, (MY_PID >> 8) & 0xFF/*idProduct*/, (MY_PID >> 8) & 0xFF/*idProduct*/, 0x00/*bcdDev*/, 0x01/*bcdDev*/,
    0x01/*iManugacturer*/, 0x02/*iProduct*/, 0x03/*iSerialNum*/, 0x01/*bNumConfig*/
} ;

static uint8    ConfigDesc[] =
{
    0x09/*bLength*/, 0x02/*bDescrType=CONFIGURATION*/, 0x20/*wTotalLength*/, 0x00/*wTotalLength*/, 0x01/*bNumIF*/, 0x01/*bCfgValue*/, 0x00/*iConfig*/, 0xA0/*bmAttribute=bus Power, remote wakeup*/, 0x32/*MaxPower*/, /*device configuration*/
    0x09/*bLength*/, 0x04/*bDescrType=INTERFACES*/, 0x00/*bIFNum*/, 0x00/*bAlternateSetting*/, 0x02/*bNumEP*/, IFCLASS/*bIFClass=Mass Storage*/, IFSUBCLASS/*bIFSubClass*/, IFPROTOCOL/*bIFProtocol=Bulk-Only*/, 0x00/*biIF*/, /*interface configuration*/
    0x07/*bLength*/, 0x05/*bDescrType=ENDPOINT*/, EPn_INPORT(EPIN)/*bEPAddr*/, 0x02/*bmAttribute=Bulk*/, EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/, 0x00/*wMaxPacketSize*/, 0x00/*bInterval*/, /*ep configuration*/
    0x07/*bLength*/, 0x05/*bDescrType=ENDPOINT*/, EPn_OUTPORT(EPOUT)/*bEPAddr*/, 0x02/*bmAttribute=Bulk*/, EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/, 0x00/*wMaxPacketSize*/, 0x00/*bInterval*/
}/*ep configuration*/;

static uint8    Str0Desc[] = {0x04/*bLength*/, 0x03/*bDescrType=STRING*/, 0x09, 0x04 };
/*manugacturer*/
static uint8    Str1Desc[] = {0x04/*bLength*/, 0x03/*bDescrType=STRING*/, ' '/*' '*/, 0x00};
/*Product*/
static uint8    Str2Desc[] = {0x1a/*bLength*/, 0x03/*bDescrType=STRING*/,
                              'S', 0x00, 'C', 0x00, 'C', 0x00, '5',
                              0x00, '1', 0x00, '.', 0x00, '1', 0x00, ' ', 0x00,
                              'D',  0x00, 'i', 0x00, 's', 0x00, 'k', 0x00
                             };
/*serial number at least 12 characters @4.1.1 */
static uint8    Str3Desc[] = {0x16/*bLength*/, 0x03/*bDescrType=STRING*/,
                              '2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
                              '0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00
                             };
#endif
#define DeviceDescLen       (sizeof(DeviceDesc))
#define ConfigDescLen       (sizeof(ConfigDesc))
#define Str0DescLen         (sizeof(Str0Desc))
#define Str1DescLen         (sizeof(Str1Desc))
#define Str2DescLen         (sizeof(Str2Desc))
#define Str3DescLen         (sizeof(Str3Desc))
static uint8    StrDescLenGroup[] = { Str0DescLen, Str1DescLen, Str2DescLen, Str3DescLen } ;
//=========================== Each Request =============================
void Chap9_SetConfiguration( void)								//0x09
{
    if(req_dir == USB_REQ_DIR_OUT)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}
void Chap9_SetAddress( void)	    							//0x05
{
    if(req_dir == USB_REQ_DIR_OUT)
    {
        EP0_Send_Empty_Packet();
        usb_state = USB_STATE_ADDRESS;
        printf("设置地址\r\n");
    }
    else
    {

    }
}

void Get_Desc_Prepare( void)
{
    uint32 type = wValue & 0xFF00;
    uint32 index = wValue & 0x00FF;
    switch(type)
    {
        case    DEVICE_DESC_TYPE :
            {
                if( wLength > DeviceDescLen )   wLength = DeviceDescLen ;
                p_ctrl_buf = DeviceDesc ;

                printf("获取设备描述符\r\n");

                break ;
            }
        case    CONFIGURATION_DESC_TYPE :
            {
                if( wLength > ConfigDescLen )   wLength = ConfigDescLen ;
                p_ctrl_buf = ConfigDesc ;
                printf("获取配置描述符\r\n");
                break ;
            }
        case    STRING_DESC_TYPE :
            {
                if( index < 4 )
                {
                    if( wLength > StrDescLenGroup[index] )
                        wLength = StrDescLenGroup[index] ;
                    switch(index)
                    {
                        case 0:
                            p_ctrl_buf = Str0Desc;
                            break;
                        case 1:
                            p_ctrl_buf = Str1Desc;
                            break;
                        case 2:
                            p_ctrl_buf = Str2Desc;
                            break;
                        case 3:
                            p_ctrl_buf = Str3Desc;
                            break;
                    }
                    printf("获取字符串描述符\r\n");
                }
                else
                {
                    wLength = 0 ;
                }
                break ;
            }
#ifdef DEVICE_HID
        case HID_REPORT_TYPE:  									// class des
            {
                if( wLength > ConfigDescLen )   wLength = InterfaceDesLen;
                p_ctrl_buf = IntefaceDes;
                printf("返回HID报告\r\n");
                break;
            }
#endif
        default :
            {
                wLength = 0 ;
                break ;
            }
    }
}
void Chap9_GetDescriptor( void)     							//0x06
{
    static uint16    ctrl_buf_ix = 0;

    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            {
                Get_Desc_Prepare( ) ;								//发送BUF指针指向待发送数据
                usb_running_ctrl_state = USB_CTRL_IN ;
                ctrl_buf_ix = 0;
                printf( "set up获取描述符\n\r") ;
                break ;
            }
        case    USB_CTRL_IN :
            {
                uint16    count = 0;
                uint16    ix = 0 ;

                printf( "写入缓冲区：") ;

                if(wLength > 0)										//数据分批发送
                {
                    count = MIN(EP0_MAX_PACKAGE_SIZE, wLength);

                    while(USB->rEP0_CTRL & 0x80);

                    for(ix = 0; ix < count; ix++)
                    {
                        printf("%x ", p_ctrl_buf[ctrl_buf_ix]);
                        USB->rEP0_FIFO = p_ctrl_buf[ctrl_buf_ix];

                        ctrl_buf_ix++;
                    }
                    USB->rEP0_CTRL = count | 0x80;
                    wLength -= count;
                }
                if( wLength == 0)
                {
                    if(count == EP0_MAX_PACKAGE_SIZE)					//发送数据总数为包最大值的整数倍时在末尾帧发送空数据
                    {
                        EP0_Send_Empty_Packet();
                    }
                    else
                    {
                    }
                    usb_running_ctrl_state = USB_IDLE;				//数据发送完成
                    ctrl_buf_ix = 0;
                }
                printf( "\r\n") ;
                break ;
            }
        default :
            {
                usb_running_ctrl_state = USB_IDLE ;
                break ;
            }
    }
    usb_state = USB_STATE_CONFIGURED;
}
void Class_Send_Max_Lun( void)      							// bRequest = 0xFE
{
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            {
                if(req_dir == USB_REQ_DIR_IN)
                    usb_running_ctrl_state = USB_CTRL_IN;
                else
                    usb_running_ctrl_state = USB_IDLE;
                break ;
            }
        case    USB_CTRL_IN :
            {

#ifdef USB_EP_STALL_TEST
                {
                    USB->rEP_HALT = 0X01;
#ifndef SCC5_VER_0302
                    USB->rEP0_CTRL = 0X80;
#endif
                    USB_SEND_OPEN_STALL[0] = TRUE;
                }
#else
                {
                    while(USB->rEP0_CTRL & 0x80);
                    USB->rEP0_FIFO = 0;
                    USB->rEP0_CTRL = 0x81;
                }
#endif
                usb_running_ctrl_state = USB_IDLE;
                break ;
            }
        default :
            {
                usb_running_ctrl_state = USB_IDLE ;
                break ;
            }
    }
}


void Chap9_GetStatus( void)         							// 0x00
{
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            {
                usb_running_ctrl_state = USB_CTRL_IN;
                break;
            }
        case USB_CTRL_IN:
            {
                if(req_dir == USB_REQ_DIR_IN)
                {
                    if(req_recip == USB_REQ_RECIP_EP)
                    {
                        char ep = wIndex & 0x0F;

                        USB->rEP0_FIFO = USB_EP_STALL[ep];
                        USB->rEP0_FIFO = 0x00;
                    }
                    else if(req_recip == USB_REQ_RECIP_DEV)
                    {

                        USB->rEP0_FIFO = 0x02;
                        USB->rEP0_FIFO = 0x00;
                    }
                    else
                    {
                        USB->rEP0_FIFO = 0x00;
                        USB->rEP0_FIFO = 0x00;
                    }
                    USB->rEP0_CTRL = 0x82;
                }
                else
                {

                }
                usb_running_ctrl_state = USB_IDLE;
                break;
            }
        default :
            {
                usb_running_ctrl_state = USB_IDLE ;
                break ;
            }
    }
}

void Chap9_ClearFeature( void)      							// 0x01
{
    switch(usb_running_ctrl_state)
    {
        case    USB_CTRL_SETUP :
            {
                if(req_dir == USB_REQ_DIR_OUT)
                {
                    uint8 ix;
                    if((req_recip == USB_REQ_RECIP_EP) && (wValue == ENDPOINT_HALT)) //ENDPOINT_HALT
                    {
                        USB->rEP_HALT = 0x00;


                        printf( "Clear EP stall 0x%02X\r\n", USB->rEP_HALT);
                        for(ix = 0; ix < 5; ix++)
                        {
                            USB_EP_STALL[ix] = FALSE;
                            USB_SEND_OPEN_STALL[ix] = FALSE;
                        }
                    }
                    else if((req_recip == USB_REQ_RECIP_DEV) && (wValue == DEVICE_REMOTE_WAKEUP)) //DEVICE_REMOTE_WAKEUP
                    {


                    }
                }
                else
                {

                }
                usb_running_ctrl_state = USB_CTRL_IN;
                break;
            }
        case USB_CTRL_IN: 											//status stage
            {
                EP0_Send_Empty_Packet();
            }
        default:
            {
                usb_running_ctrl_state = USB_IDLE;
                break;
            }
    }
}
void MLsup_StallEP0( void)          							// reserve      0x02
{
    if(req_dir == USB_REQ_DIR_IN)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}
void MLsup_StallEP1( void)          							// reserve      0x04
{
    if(req_dir == USB_REQ_DIR_IN)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}
void Chap9_SetFeature( void)	    							// 0x03
{
    if(req_dir == USB_REQ_DIR_OUT)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}

void Chap9_SetDescriptor( void)     							//0x07
{
    if(req_dir == USB_REQ_DIR_OUT)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}
void Chap9_GetConfiguration( void)								//0x08
{
    if(req_dir == USB_REQ_DIR_IN)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}

void Chap9_GetInterface( void)									//0x0a
{
    if(req_dir == USB_REQ_DIR_IN)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}
void Chap9_SetInterface( void)									//0x0b
{
    if(req_dir == USB_REQ_DIR_OUT)
    {
        EP0_Send_Empty_Packet();
    }
    else
    {

    }
}


void EP0_Send_Empty_Packet()
{
    printf("发送空数据\r\n");
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
            {
                usb_running_ctrl_state = USB_CTRL_IN;
                break ;
            }
        case    USB_CTRL_IN :
            {

                while(USB->rEP0_CTRL & 0x80);
                USB->rEP0_CTRL = 0x80;
                usb_running_ctrl_state = USB_IDLE;
                break ;
            }
        default :
            {
                usb_running_ctrl_state = USB_IDLE ;
                break ;
            }
    }
}

