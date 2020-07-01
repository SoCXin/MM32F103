#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"
int main(void)
{
    u16 ADCVAL;
    float fValue;
    delay_init();	    	 //延时函数初始化
    uart_init(SystemCoreClock / 1000000, 115200);
    Adc_Init();
    while(1)
    {
        ADCVAL = Get_Adc_Average(5);
        fValue = ((float)ADCVAL / 4095) * 3.3;
        printf("ADC1_CH_1=%.2fV\r\n", fValue);
        delay_ms(1000);
    }
}

























