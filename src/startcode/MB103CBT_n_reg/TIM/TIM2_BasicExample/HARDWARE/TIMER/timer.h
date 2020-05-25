#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern unsigned int ucTim2Flag;

void TIM2_Int_Init(u16 arr, u16 psc);
void Tim2_UPStatusOVCheck_test(void);

#endif
