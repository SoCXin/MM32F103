#ifndef __ADC_TEST_H_
#define __ADC_TEST_H_
#include "sys.h"
void ADC1_SingleChannel(uint8_t ADC_Channel_x);
unsigned char ADC1_SingleChannel_Get(uint8_t ADC_Channel_x, unsigned int *puiADData);
#endif
