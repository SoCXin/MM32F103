#ifndef __usb_prop_H
#define __usb_prop_H


typedef struct
{
    unsigned int bitrate;
    unsigned char format;
    unsigned char paritytype;
    unsigned char datatype;
} LINE_CODING;

unsigned int UsbVcomRec(unsigned char*rxInfo);


#endif
