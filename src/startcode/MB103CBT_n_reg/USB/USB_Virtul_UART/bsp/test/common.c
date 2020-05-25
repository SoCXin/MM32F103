#include "all.h"

void Delay(unsigned long count)
{
    unsigned long i;

    for(i = 0; i < count; i++)
        ;
}

void DataCompare(unsigned char *p1, unsigned char *p2, unsigned int count)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {
        if(p1[i] != p2[i])
            uart_printf("tx[%d]=0x%x,rx[%d]=0x%x\r\n", i, p1[i], i, p2[i]);
    }
}

