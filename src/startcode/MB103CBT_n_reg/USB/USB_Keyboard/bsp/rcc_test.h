#ifndef __RCC_TEST_H_
#define __RCC_TEST_H_
#include "HAL_device.h"
void SystemClk_HSEInit(void);
void SystemClk_HSIInit(void);
void SystemClkPll(unsigned int pllmul);
void HSE_Pll_test(unsigned int pllmul);
void HSI_Pll_test(unsigned int pllmul);
void LSI_clk(void);
void LSE_clk(void);
void HSE_HSI_Change(void);
#endif
