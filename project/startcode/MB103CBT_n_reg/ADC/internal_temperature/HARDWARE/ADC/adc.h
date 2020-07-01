#ifndef __ADC_H
#define __ADC_H
#include "sys.h"


void Adc_Init(void);
u16 ADC1_SingleChannel_Get(void);
u16 Get_Adc_Average(uint8_t times);

#endif
